/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <string.h>
#include "glowtx.h"
#include "emberinternal.h"


// ====================================================================
//
// Import internals
//
// ====================================================================

bool glow_assertIdentifierValid(pcstr pIdentifier, bool isRx);


// ====================================================================
//
// GlowOutput locals
//
// ====================================================================

static void writeFramingHeader(GlowOutput *pThis, bool isFirstPackage, bool isLastPackage)
{
   byte flags = 0;

   if(isFirstPackage)
      flags |= EmberFramingFlag_FirstPackage;
   if(isLastPackage)
      flags |= EmberFramingFlag_LastPackage;

   berFramingOutput_writeHeader(&pThis->base, (EmberFramingFlags)flags);
   pThis->hasLastPackage = isLastPackage;
}


// ====================================================================
//
// GlowOutput globals
//
// ====================================================================

void glowOutput_init(GlowOutput *pThis,
                     byte *pMemory,
                     unsigned int size,
                     byte slotId)
{
   static const byte s_appBytes[2] =
   {
      (byte)((GLOW_SCHEMA_VERSION >> 0) & 0xFF),
      (byte)((GLOW_SCHEMA_VERSION >> 8) & 0xFF),
   };

   berFramingOutput_init(&pThis->base, pMemory, size, slotId, EMBER_DTD_GLOW, s_appBytes, sizeof(s_appBytes));
   pThis->hasLastPackage = false;
   pThis->packageCount = 0;
#ifdef _DEBUG
   pThis->positionHint = 0;
#endif
}

void glowOutput_initWithoutEscaping(GlowOutput *pThis, byte *pMemory, unsigned int size, byte slotId)
{
    static const byte s_appBytes[2] =
    {
        (byte)((GLOW_SCHEMA_VERSION >> 0) & 0xFF),
        (byte)((GLOW_SCHEMA_VERSION >> 8) & 0xFF),
    };

    berFramingOutput_initWithoutEscaping(&pThis->base, pMemory, size, slotId, EMBER_DTD_GLOW, s_appBytes, sizeof(s_appBytes));
    pThis->hasLastPackage = false;
    pThis->packageCount = 0;
#ifdef _DEBUG
    pThis->positionHint = 0;
#endif
}

void glowOutput_beginPackage(GlowOutput *pThis, bool isLastPackage)
{
   bool isFirstPackage = pThis->packageCount == 0;

   writeFramingHeader(pThis, isFirstPackage, isLastPackage);

   if(isFirstPackage)
      ember_writeContainerBegin(&pThis->base.base.base, &glowTags.root, GlowType_RootElementCollection);

   pThis->packageCount++;
}

void glowOutput_beginStreamPackage(GlowOutput *pThis, bool isLastPackage)
{
   bool isFirstPackage = pThis->packageCount == 0;

   writeFramingHeader(pThis, isFirstPackage, isLastPackage);

   if(isFirstPackage)
      ember_writeContainerBegin(&pThis->base.base.base, &glowTags.root, GlowType_StreamCollection);

   pThis->packageCount++;
}

unsigned int glowOutput_finishPackage(GlowOutput *pThis)
{
   if(pThis->hasLastPackage)
   {
      ember_writeContainerEnd(&pThis->base.base.base);

      pThis->hasLastPackage = false;
      pThis->packageCount = 0;
   }

   return berFramingOutput_finish(&pThis->base);
}


// ====================================================================
//
// Writer locals
//
// ====================================================================

static void writeMinMax(BerOutput *pOut, const BerTag *pTag, const GlowMinMax *pMinMax)
{
   switch(pMinMax->flag)
   {
      case GlowParameterType_Integer:
         ember_writeLong(pOut, pTag, pMinMax->choice.integer);
         break;

      case GlowParameterType_Real:
         ember_writeReal(pOut, pTag, pMinMax->choice.real);
         break;

      default:
         throwError(505, "GlowMinMax object has unsupported type!");
         break;
   }
}

static void writeValue(BerOutput *pOut, const BerTag *pTag, const GlowValue *pValue)
{
   switch(pValue->flag)
   {
      case GlowParameterType_Integer:
         ember_writeLong(pOut, pTag, pValue->choice.integer);
         break;

      case GlowParameterType_Real:
         ember_writeReal(pOut, pTag, pValue->choice.real);
         break;

      case GlowParameterType_String:
         ember_writeString(pOut, pTag, pValue->choice.pString);
         break;

      case GlowParameterType_Boolean:
         ember_writeBoolean(pOut, pTag, pValue->choice.boolean);
         break;

      case GlowParameterType_Octets:
         ember_writeOctetString(pOut, pTag, pValue->choice.octets.pOctets, pValue->choice.octets.length);
         break;

      case GlowParameterType_None:
          ember_writeNull(pOut, pTag);
          break;

      default:
         throwError(506, "GlowValue object has unsupported type!");
         break;
   }
}

static void writeDoubleContainerEnd(BerOutput *pOut)
{
   ember_writeContainerEnd(pOut);
   ember_writeContainerEnd(pOut);
}

static void writeInvocation(BerOutput *pOut, const BerTag *pTag, const GlowInvocation *pInvocation)
{
   int index;

   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);
   ASSERT(pInvocation != NULL);

   ember_writeContainerBegin(pOut, pTag, GlowType_Invocation);
   ember_writeInteger(pOut, &glowTags.invocation.invocationId, pInvocation->invocationId);

   if(pInvocation->pArguments != NULL)
   {
      ember_writeSequenceBegin(pOut, &glowTags.invocation.arguments);

      for(index = 0; index < pInvocation->argumentsLength; index++)
         writeValue(pOut, &glowTags.collection.item, &pInvocation->pArguments[index]);

      ember_writeContainerEnd(pOut);
   }

   ember_writeContainerEnd(pOut);
}

static void writeTupleItemDescription(BerOutput *pOut, const BerTag *pTag, const GlowTupleItemDescription *pTupleItem)
{
   ember_writeContainerBegin(pOut, pTag, GlowType_TupleItemDescription);
   ember_writeInteger(pOut, &glowTags.tupleItemDescription.type, pTupleItem->type);

   if(pTupleItem->pName != NULL)
      ember_writeString(pOut, &glowTags.tupleItemDescription.name, pTupleItem->pName);

   ember_writeContainerEnd(pOut);
}


// ====================================================================
//
// Writer internals
//
// ====================================================================

void glow_writeCommandImpl(BerOutput *pOut, const BerTag *pTag, const GlowCommand *pCommand)
{
   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);
   ASSERT(pCommand != NULL);

   ember_writeContainerBegin(pOut, pTag, GlowType_Command);
   ember_writeInteger(pOut, &glowTags.command.number, pCommand->number);

   if(pCommand->number == GlowCommandType_GetDirectory
   && pCommand->options.dirFieldMask != 0)
      ember_writeInteger(pOut, &glowTags.command.dirFieldMask, pCommand->options.dirFieldMask);

   if(pCommand->number == GlowCommandType_Invoke)
      writeInvocation(pOut, &glowTags.command.invocation, &pCommand->options.invocation);

   ember_writeContainerEnd(pOut); // end node
}

void glow_writeQualifiedNodeImpl(BerOutput *pOut,
                                 const GlowNode *pNode,
                                 GlowFieldFlags fields,
                                 const berint *pPath,
                                 int pathLength)
{
   ASSERT(pOut != NULL);
   ASSERT(pNode != NULL);

   ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedNode);
   ember_writeRelativeOid(pOut, &glowTags.qualifiedNode.path, pPath, pathLength);

   if(fields != 0)
   {
      ember_writeSetBegin(pOut, &glowTags.qualifiedNode.contents);

      if((fields & GlowFieldFlag_Identifier) == GlowFieldFlag_Identifier
      && pNode->pIdentifier != NULL)
      {
         if(glow_assertIdentifierValid(pNode->pIdentifier, false))
            ember_writeString(pOut, &glowTags.nodeContents.identifier, pNode->pIdentifier);
      }

      if((fields & GlowFieldFlag_Description) == GlowFieldFlag_Description
      && pNode->pDescription != NULL)
         ember_writeString(pOut, &glowTags.nodeContents.description, pNode->pDescription);

      if(fields & GlowFieldFlag_IsRoot)
         ember_writeBoolean(pOut, &glowTags.nodeContents.isRoot, pNode->isRoot);

      if(fields & GlowFieldFlag_IsOnline)
         ember_writeBoolean(pOut, &glowTags.nodeContents.isOnline, pNode->isOnline);

      if((fields & GlowFieldFlag_SchemaIdentifier)
      && pNode->pSchemaIdentifiers != NULL)
         ember_writeString(pOut, &glowTags.nodeContents.schemaIdentifiers, pNode->pSchemaIdentifiers);

      if ((fields & GlowFieldFlag_TemplateReference)
      && pNode->pTemplateReference != NULL)
          ember_writeRelativeOid(pOut, &glowTags.nodeContents.templateReference, pNode->pTemplateReference, pNode->templateReferenceLength);

      ember_writeContainerEnd(pOut); // end node.contents
   }

   ember_writeContainerEnd(pOut); // end node
}

void glow_writeQualifiedParameterImpl(BerOutput *pOut,
                                      const GlowParameter *pParameter,
                                      GlowFieldFlags fields,
                                      const berint *pPath,
                                      int pathLength)
{
   ASSERT(pOut != NULL);
   ASSERT(pParameter != NULL);

   ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedParameter);
   ember_writeRelativeOid(pOut, &glowTags.qualifiedParameter.path, pPath, pathLength);

   ember_writeSetBegin(pOut, &glowTags.qualifiedParameter.contents);
   if((fields & GlowFieldFlag_Identifier) == GlowFieldFlag_Identifier
   && pParameter->pIdentifier != NULL)
   {
      if(glow_assertIdentifierValid(pParameter->pIdentifier, false))
         ember_writeString(pOut, &glowTags.parameterContents.identifier, pParameter->pIdentifier);
   }

   if((fields & GlowFieldFlag_Description) == GlowFieldFlag_Description
   && pParameter->pDescription != NULL)
      ember_writeString(pOut, &glowTags.parameterContents.description, pParameter->pDescription);

   if(fields & GlowFieldFlag_Value)
      writeValue(pOut, &glowTags.parameterContents.value, &pParameter->value);

   if (fields & GlowFieldFlag_DefaultValue)
       writeValue(pOut, &glowTags.parameterContents.defaultValue, &pParameter->defaultValue);

   if(fields & GlowFieldFlag_Minimum)
      writeMinMax(pOut, &glowTags.parameterContents.minimum, &pParameter->minimum);

   if(fields & GlowFieldFlag_Maximum)
      writeMinMax(pOut, &glowTags.parameterContents.maximum, &pParameter->maximum);

   if(fields & GlowFieldFlag_Access)
      ember_writeInteger(pOut, &glowTags.parameterContents.access, (berint)pParameter->access);

   if((fields & GlowFieldFlag_Format) && pParameter->pFormat != NULL)
      ember_writeString(pOut, &glowTags.parameterContents.format, pParameter->pFormat);

   if(fields & GlowFieldFlag_Factor)
      ember_writeInteger(pOut, &glowTags.parameterContents.factor, pParameter->factor);

   if(fields & GlowFieldFlag_IsOnline)
      ember_writeBoolean(pOut, &glowTags.parameterContents.isOnline, pParameter->isOnline);

   if(fields & GlowFieldFlag_Step)
      ember_writeInteger(pOut, &glowTags.parameterContents.step, pParameter->step);

   if(fields & GlowFieldFlag_Type)
      ember_writeInteger(pOut, &glowTags.parameterContents.type, (berint)pParameter->type);

   if(fields & GlowFieldFlag_StreamIdentifier)
      ember_writeInteger(pOut, &glowTags.parameterContents.streamIdentifier, pParameter->streamIdentifier);

   if((fields & GlowFieldFlag_Formula) && pParameter->pFormula != NULL)
      ember_writeString(pOut, &glowTags.parameterContents.formula, pParameter->pFormula);

   if((fields & GlowFieldFlag_Enumeration) && pParameter->pEnumeration != NULL)
      ember_writeString(pOut, &glowTags.parameterContents.enumeration, pParameter->pEnumeration);

   if ((fields & GlowFieldFlag_TemplateReference) && pParameter->pTemplateReference != NULL)
       ember_writeRelativeOid(pOut, &glowTags.parameterContents.templateReference, pParameter->pTemplateReference, pParameter->templateReferenceLength);

   if(fields & GlowFieldFlag_StreamDescriptor)
   {
      ember_writeContainerBegin(pOut, &glowTags.parameterContents.streamDescriptor, GlowType_StreamDescription);
      ember_writeInteger(pOut, &glowTags.streamDescription.format, pParameter->streamDescriptor.format);
      ember_writeInteger(pOut, &glowTags.streamDescription.offset, pParameter->streamDescriptor.offset);
      ember_writeContainerEnd(pOut);
   }

   if((fields & GlowFieldFlag_SchemaIdentifier) && pParameter->pSchemaIdentifiers != NULL)
      ember_writeString(pOut, &glowTags.parameterContents.schemaIdentifiers, pParameter->pSchemaIdentifiers);

   ember_writeContainerEnd(pOut); // end parameter.contents

   ember_writeContainerEnd(pOut); // end parameter
}

void glow_writeQualifiedCommandImpl(BerOutput *pOut,
                                    const GlowCommand *pCommand,
                                    const berint *pPath,
                                    int pathLength,
                                    GlowElementType parentType)
{
   if(pathLength > 0)
   {
      switch(parentType)
      {
         case GlowElementType_Node:
            ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedNode);
            ember_writeRelativeOid(pOut, &glowTags.qualifiedNode.path, pPath, pathLength);
            ember_writeContainerBegin(pOut, &glowTags.qualifiedNode.children, GlowType_ElementCollection);
            break;

         case GlowElementType_Parameter:
            ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedParameter);
            ember_writeRelativeOid(pOut, &glowTags.qualifiedParameter.path, pPath, pathLength);
            ember_writeContainerBegin(pOut, &glowTags.qualifiedParameter.children, GlowType_ElementCollection);
            break;

         case GlowElementType_Matrix:
            ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedMatrix);
            ember_writeRelativeOid(pOut, &glowTags.qualifiedMatrix.path, pPath, pathLength);
            ember_writeContainerBegin(pOut, &glowTags.qualifiedMatrix.children, GlowType_ElementCollection);
            break;

         case GlowElementType_Function:
            ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedFunction);
            ember_writeRelativeOid(pOut, &glowTags.qualifiedFunction.path, pPath, pathLength);
            ember_writeContainerBegin(pOut, &glowTags.qualifiedFunction.children, GlowType_ElementCollection);
            break;

         default:
            FAIL();
            break;
      }
   }

   glow_writeCommandImpl(pOut, &glowTags.elementCollection.element, pCommand);

   if(pathLength > 0)
   {
      ember_writeContainerEnd(pOut); // end node/parameter.children
      ember_writeContainerEnd(pOut); // end node/parameter
   }
}

void glow_writeStreamEntryImpl(BerOutput *pOut,
                               const GlowStreamEntry *pEntry)
{
   ember_writeContainerBegin(pOut, &glowTags.streamCollection.streamEntry, GlowType_StreamEntry);
   ember_writeInteger(pOut, &glowTags.streamEntry.streamIdentifier, pEntry->streamIdentifier);
   writeValue(pOut, &glowTags.streamEntry.streamValue, &pEntry->streamValue);
   ember_writeContainerEnd(pOut);
}

void glow_writeQualifiedMatrixImpl(BerOutput *pOut,
                                   const GlowMatrix *pMatrix,
                                   GlowFieldFlags fields,
                                   const berint *pPath,
                                   int pathLength)
{
   const GlowParametersLocation *pParams = &pMatrix->parametersLocation;
   const GlowLabel *pLabel;
   int labelIndex;

   ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedMatrix);
   ember_writeRelativeOid(pOut, &glowTags.qualifiedMatrix.path, pPath, pathLength);

   ember_writeSetBegin(pOut, &glowTags.qualifiedMatrix.contents);

   if((fields & GlowFieldFlag_Identifier) == GlowFieldFlag_Identifier
   && pMatrix->pIdentifier != NULL)
   {
      if(glow_assertIdentifierValid(pMatrix->pIdentifier, false))
         ember_writeString(pOut, &glowTags.matrixContents.identifier, pMatrix->pIdentifier);
   }

   if((fields & GlowFieldFlag_Description) == GlowFieldFlag_Description
   && pMatrix->pDescription != NULL)
      ember_writeString(pOut, &glowTags.matrixContents.description, pMatrix->pDescription);

   if(fields == GlowFieldFlag_All)
   {
      if(pMatrix->type != 0)
         ember_writeInteger(pOut, &glowTags.matrixContents.type, pMatrix->type);

      if(pMatrix->addressingMode != 0)
         ember_writeInteger(pOut, &glowTags.matrixContents.addressingMode, pMatrix->addressingMode);
   }

   ember_writeInteger(pOut, &glowTags.matrixContents.targetCount, pMatrix->targetCount);
   ember_writeInteger(pOut, &glowTags.matrixContents.sourceCount, pMatrix->sourceCount);

   if(fields == GlowFieldFlag_All)
   {
      if(pMatrix->maximumTotalConnects != 0)
         ember_writeInteger(pOut, &glowTags.matrixContents.maximumTotalConnects, pMatrix->maximumTotalConnects);

      if(pMatrix->maximumConnectsPerTarget != 0)
         ember_writeInteger(pOut, &glowTags.matrixContents.maximumConnectsPerTarget, pMatrix->maximumConnectsPerTarget);

      if(glowParametersLocation_isValid(pParams))
      {
         if(pParams->kind == GlowParametersLocationKind_BasePath)
         {
            ember_writeRelativeOid(pOut, &glowTags.matrixContents.parametersLocation, pParams->choice.basePath.ids, pParams->choice.basePath.length);
         }
         else if(pParams->kind == GlowParametersLocationKind_Inline)
         {
            ember_writeInteger(pOut, &glowTags.matrixContents.parametersLocation, pParams->choice.inlineId);
         }
      }

      if(pMatrix->pLabels != NULL
      && pMatrix->labelsLength > 0)
      {
         ember_writeSequenceBegin(pOut, &glowTags.matrixContents.labels);

         for(labelIndex = 0; labelIndex < pMatrix->labelsLength; labelIndex++)
         {
            pLabel = &pMatrix->pLabels[labelIndex];
            ember_writeContainerBegin(pOut, &glowTags.collection.item, GlowType_Label);

            ember_writeRelativeOid(pOut, &glowTags.label.basePath, pLabel->basePath, pLabel->basePathLength);

            if(pLabel->pDescription != NULL)
               ember_writeString(pOut, &glowTags.label.description, pLabel->pDescription);

            ember_writeContainerEnd(pOut); // end matrix.contents
         }

         ember_writeContainerEnd(pOut); // end matrix.contents
      }

      if((fields & GlowFieldFlag_SchemaIdentifier) == GlowFieldFlag_SchemaIdentifier
      && pMatrix->pSchemaIdentifiers != NULL)
         ember_writeString(pOut, &glowTags.matrixContents.schemaIdentifiers, pMatrix->pSchemaIdentifiers);


      if ((fields & GlowFieldFlag_TemplateReference) && pMatrix->pTemplateReference != NULL)
          ember_writeRelativeOid(pOut, &glowTags.matrixContents.templateReference, pMatrix->pTemplateReference, pMatrix->templateReferenceLength);
   }

   ember_writeContainerEnd(pOut); // end matrix.contents
   ember_writeContainerEnd(pOut); // end matrix
}

void glow_writeTargetsPrefixImpl(BerOutput *pOut,
                                 const berint *pMatrixPath,
                                 int matrixPathLength)
{
   ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedMatrix);
   ember_writeRelativeOid(pOut, &glowTags.qualifiedMatrix.path, pMatrixPath, matrixPathLength);
   ember_writeSequenceBegin(pOut, &glowTags.qualifiedMatrix.targets);
}

void glow_writeTargetImpl(BerOutput *pOut, const GlowSignal *pTarget)
{
   ember_writeContainerBegin(pOut, &glowTags.collection.item, GlowType_Target);
   ember_writeInteger(pOut, &glowTags.signal.number, pTarget->number);
   ember_writeContainerEnd(pOut);
}

void glow_writeSourcesPrefixImpl(BerOutput *pOut,
                                 const berint *pMatrixPath,
                                 int matrixPathLength)
{
   ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedMatrix);
   ember_writeRelativeOid(pOut, &glowTags.qualifiedMatrix.path, pMatrixPath, matrixPathLength);
   ember_writeSequenceBegin(pOut, &glowTags.qualifiedMatrix.sources);
}

void glow_writeSourceImpl(BerOutput *pOut,
                          const GlowSignal *pSource)
{
   ember_writeContainerBegin(pOut, &glowTags.collection.item, GlowType_Source);
   ember_writeInteger(pOut, &glowTags.signal.number, pSource->number);
   ember_writeContainerEnd(pOut);
}

void glow_writeConnectionsPrefixImpl(BerOutput *pOut,
                                     const berint *pMatrixPath,
                                     int matrixPathLength)
{
   ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedMatrix);
   ember_writeRelativeOid(pOut, &glowTags.qualifiedMatrix.path, pMatrixPath, matrixPathLength);
   ember_writeSequenceBegin(pOut, &glowTags.qualifiedMatrix.connections);
 }

void glow_writeConnectionImpl(BerOutput *pOut, const GlowConnection *pConnection)
{
   ember_writeContainerBegin(pOut, &glowTags.collection.item, GlowType_Connection);

   ember_writeInteger(pOut, &glowTags.connection.target, pConnection->target);

   if(pConnection->pSources != NULL)
      ember_writeRelativeOid(pOut, &glowTags.connection.sources, pConnection->pSources, pConnection->sourcesLength);

   if(pConnection->operation != 0)
      ember_writeInteger(pOut, &glowTags.connection.operation, pConnection->operation);

   if(pConnection->disposition != 0)
      ember_writeInteger(pOut, &glowTags.connection.disposition, pConnection->disposition);

   ember_writeContainerEnd(pOut);
}

void glow_writeQualifiedFunctionImpl(BerOutput *pOut,
                                     const GlowFunction *pFunction,
                                     GlowFieldFlags fields,
                                     const berint *pPath,
                                     int pathLength)
{
   int index;

   ASSERT(pOut != NULL);
   ASSERT(pFunction != NULL);

   ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedFunction);
   ember_writeRelativeOid(pOut, &glowTags.qualifiedFunction.path, pPath, pathLength);
   ember_writeSetBegin(pOut, &glowTags.qualifiedFunction.contents);

   if((fields & GlowFieldFlag_Identifier) == GlowFieldFlag_Identifier
   && pFunction->pIdentifier != NULL)
   {
      if(glow_assertIdentifierValid(pFunction->pIdentifier, false))
         ember_writeString(pOut, &glowTags.functionContents.identifier, pFunction->pIdentifier);
   }

   if((fields & GlowFieldFlag_Description) == GlowFieldFlag_Description
   && pFunction->pDescription != NULL)
      ember_writeString(pOut, &glowTags.functionContents.description, pFunction->pDescription);


   if ((fields & GlowFieldFlag_TemplateReference) && pFunction->pTemplateReference != NULL)
       ember_writeRelativeOid(pOut, &glowTags.functionContents.templateReference, pFunction->pTemplateReference, pFunction->templateReferenceLength);

   if(fields == GlowFieldFlag_All)
   {
      if(pFunction->pArguments != NULL)
      {
         ember_writeSequenceBegin(pOut, &glowTags.functionContents.arguments);

         for(index = 0; index < pFunction->argumentsLength; index++)
            writeTupleItemDescription(pOut, &glowTags.collection.item, &pFunction->pArguments[index]);

         ember_writeContainerEnd(pOut);
      }

      if(pFunction->pResult != NULL)
      {
         ember_writeSequenceBegin(pOut, &glowTags.functionContents.result);

         for(index = 0; index < pFunction->resultLength; index++)
            writeTupleItemDescription(pOut, &glowTags.collection.item, &pFunction->pResult[index]);

         ember_writeContainerEnd(pOut);
      }
   }

   ember_writeContainerEnd(pOut); // end function.contents
   ember_writeContainerEnd(pOut); // end function
}

void glow_writeInvocationResultImpl(BerOutput *pOut, const BerTag *pTag, const GlowInvocationResult *pRoot)
{
   int index;

   ASSERT(pOut != NULL);
   ASSERT(pRoot != NULL);

   ember_writeContainerBegin(pOut, pTag, GlowType_InvocationResult);
   ember_writeInteger(pOut, &glowTags.invocationResult.invocationId, pRoot->invocationId);

   if(pRoot->hasError)
      ember_writeBoolean(pOut, &glowTags.invocationResult.success, false);

   if(pRoot->pResult != NULL)
   {
      ember_writeSequenceBegin(pOut, &glowTags.invocationResult.result);

      for(index = 0; index < pRoot->resultLength; index++)
         writeValue(pOut, &glowTags.collection.item, &pRoot->pResult[index]);

      ember_writeContainerEnd(pOut);
   }

   ember_writeContainerEnd(pOut);
}


// ====================================================================
//
// Writer globals
//
// ====================================================================

void glow_writeQualifiedNode(GlowOutput *pOut,
                             const GlowNode *pNode,
                             GlowFieldFlags fields,
                             const berint *pPath,
                             int pathLength)
{
   ASSERT(pOut != NULL);
   ASSERT(pNode != NULL);
   ASSERT(pPath != NULL || pathLength == 0);
   ASSERT(pOut->positionHint == 0);

   glow_writeQualifiedNodeImpl(&pOut->base.base.base, pNode, fields, pPath, pathLength);
}


void glow_writeQualifiedTemplate(GlowOutput *pOut,
    const GlowTemplate *pTemplate,
    GlowFieldFlags fields,
    void (*writeElement)(GlowOutput *pOut, const GlowTemplate *pTemplate, const BerTag * elementTag),
    const berint *pPath,
    int pathLength)
{
    BerOutput *pBerOutput;

    ASSERT(pOut != NULL);
    ASSERT(pTemplate != NULL);
    ASSERT(pPath != NULL || pathLength == 0);
    ASSERT(pOut->positionHint == 0);

    ASSERT(pOut != NULL);
    ASSERT(pTemplate != NULL);

    pBerOutput = &pOut->base.base.base;

    ember_writeContainerBegin(pBerOutput, &glowTags.elementCollection.element, GlowType_QualifiedTemplate);
    ember_writeRelativeOid(pBerOutput, &glowTags.qualifiedTemplate.path, pPath, pathLength);

    if (fields != 0)
    {
        if ((fields & GlowFieldFlag_Description) == GlowFieldFlag_Description
            && pTemplate->pDescription != NULL)
            ember_writeString(pBerOutput, &glowTags.qualifiedTemplate.description, pTemplate->pDescription);
    }

    if (writeElement != NULL)
        writeElement(pOut, pTemplate, &glowTags.qualifiedTemplate.element);

    ember_writeContainerEnd(pBerOutput); // end node
}


LIBEMBER_API void glow_writeTemplateElementNodeBegin(
    GlowOutput *pOut,
    const BerTag *tag,
    const GlowNode *pNode,
    GlowFieldFlags fields,
    berint number)
{
    BerOutput *pBerOutput = &pOut->base.base.base;

    if (tag == NULL)
        tag = &glowTags.elementCollection.element;

    ember_writeContainerBegin(pBerOutput, tag, GlowType_Node);
    {
        ember_writeInteger(pBerOutput, &glowTags.node.number, number);

        if (fields != 0)
        {
            ember_writeSetBegin(pBerOutput, &glowTags.node.contents);
            {
                if ((fields & GlowFieldFlag_Identifier) == GlowFieldFlag_Identifier
                    && pNode->pIdentifier != NULL)
                {
                    if (glow_assertIdentifierValid(pNode->pIdentifier, false))
                        ember_writeString(pBerOutput, &glowTags.nodeContents.identifier, pNode->pIdentifier);
                }

                if ((fields & GlowFieldFlag_Description) == GlowFieldFlag_Description
                    && pNode->pDescription != NULL)
                    ember_writeString(pBerOutput, &glowTags.nodeContents.description, pNode->pDescription);

                if (fields & GlowFieldFlag_IsRoot)
                    ember_writeBoolean(pBerOutput, &glowTags.nodeContents.isRoot, pNode->isRoot);

                if (fields & GlowFieldFlag_IsOnline)
                    ember_writeBoolean(pBerOutput, &glowTags.nodeContents.isOnline, pNode->isOnline);

                if ((fields & GlowFieldFlag_SchemaIdentifier)
                    && pNode->pSchemaIdentifiers != NULL)
                    ember_writeString(pBerOutput, &glowTags.nodeContents.schemaIdentifiers, pNode->pSchemaIdentifiers);

                if ((fields & GlowFieldFlag_TemplateReference)
                    && pNode->pTemplateReference != NULL)
                    ember_writeRelativeOid(pBerOutput, &glowTags.nodeContents.templateReference, pNode->pTemplateReference, pNode->templateReferenceLength);
            }
            ember_writeContainerEnd(pBerOutput);
        }
    }
}

void glow_writeTemplateElementNodeEnd(GlowOutput *pOut)
{
    ember_writeContainerEnd(&pOut->base.base.base);
}

void glow_writeTemplateElementNodeChildrenBegin(GlowOutput *pOut)
{
    ember_writeContainerBegin(&pOut->base.base.base, &glowTags.node.children, GlowType_ElementCollection);
}

void glow_writeTemplateElementNodeChildrenEnd(GlowOutput *pOut)
{
    ember_writeContainerEnd(&pOut->base.base.base);
}

void glow_writeTemplateElementNode(
    GlowOutput *pOut,
    const BerTag *tag,
    const GlowNode *pNode,
    GlowFieldFlags fields,
    berint number)
{
    glow_writeTemplateElementNodeBegin(pOut, tag, pNode, fields, number);
    glow_writeTemplateElementNodeEnd(pOut);
}

void glow_writeTemplateElementParameter(
    GlowOutput *pOut,
    const BerTag *tag,
    const GlowParameter *pParameter,
    GlowFieldFlags fields,
    berint number)
{
    BerOutput *pBerOutput;

    ASSERT(pOut != NULL);
    ASSERT(pParameter != NULL);

    pBerOutput = &pOut->base.base.base;

    if (tag == NULL)
        tag = &glowTags.elementCollection.element;

    ember_writeContainerBegin(pBerOutput, tag, GlowType_Parameter);
    ember_writeInteger(pBerOutput, &glowTags.parameter.number, number);

    ember_writeSetBegin(pBerOutput, &glowTags.parameter.contents);

    if ((fields & GlowFieldFlag_Identifier) == GlowFieldFlag_Identifier && pParameter->pIdentifier != NULL)
    {
        if (glow_assertIdentifierValid(pParameter->pIdentifier, false))
            ember_writeString(pBerOutput, &glowTags.parameterContents.identifier, pParameter->pIdentifier);
    }

    if ((fields & GlowFieldFlag_Description) == GlowFieldFlag_Description
        && pParameter->pDescription != NULL)
        ember_writeString(pBerOutput, &glowTags.parameterContents.description, pParameter->pDescription);

    if (fields & GlowFieldFlag_Value)
        writeValue(pBerOutput, &glowTags.parameterContents.value, &pParameter->value);

    if (fields & GlowFieldFlag_DefaultValue)
        writeValue(pBerOutput, &glowTags.parameterContents.defaultValue, &pParameter->defaultValue);

    if (fields & GlowFieldFlag_Minimum)
        writeMinMax(pBerOutput, &glowTags.parameterContents.minimum, &pParameter->minimum);

    if (fields & GlowFieldFlag_Maximum)
        writeMinMax(pBerOutput, &glowTags.parameterContents.maximum, &pParameter->maximum);

    if (fields & GlowFieldFlag_Access)
        ember_writeInteger(pBerOutput, &glowTags.parameterContents.access, (berint)pParameter->access);

    if ((fields & GlowFieldFlag_Format) && pParameter->pFormat != NULL)
        ember_writeString(pBerOutput, &glowTags.parameterContents.format, pParameter->pFormat);

    if (fields & GlowFieldFlag_Factor)
        ember_writeInteger(pBerOutput, &glowTags.parameterContents.factor, pParameter->factor);

    if (fields & GlowFieldFlag_IsOnline)
        ember_writeBoolean(pBerOutput, &glowTags.parameterContents.isOnline, pParameter->isOnline);

    if (fields & GlowFieldFlag_Step)
        ember_writeInteger(pBerOutput, &glowTags.parameterContents.step, pParameter->step);

    if (fields & GlowFieldFlag_Type)
        ember_writeInteger(pBerOutput, &glowTags.parameterContents.type, (berint)pParameter->type);

    if (fields & GlowFieldFlag_StreamIdentifier)
        ember_writeInteger(pBerOutput, &glowTags.parameterContents.streamIdentifier, pParameter->streamIdentifier);

    if ((fields & GlowFieldFlag_Formula) && pParameter->pFormula != NULL)
        ember_writeString(pBerOutput, &glowTags.parameterContents.formula, pParameter->pFormula);

    if ((fields & GlowFieldFlag_Enumeration) && pParameter->pEnumeration != NULL)
        ember_writeString(pBerOutput, &glowTags.parameterContents.enumeration, pParameter->pEnumeration);

    if ((fields & GlowFieldFlag_TemplateReference) && pParameter->pTemplateReference != NULL)
        ember_writeRelativeOid(pBerOutput, &glowTags.parameterContents.templateReference, pParameter->pTemplateReference, pParameter->templateReferenceLength);

    if (fields & GlowFieldFlag_StreamDescriptor)
    {
        ember_writeContainerBegin(pBerOutput, &glowTags.parameterContents.streamDescriptor, GlowType_StreamDescription);
        ember_writeInteger(pBerOutput, &glowTags.streamDescription.format, pParameter->streamDescriptor.format);
        ember_writeInteger(pBerOutput, &glowTags.streamDescription.offset, pParameter->streamDescriptor.offset);
        ember_writeContainerEnd(pBerOutput);
    }

    if ((fields & GlowFieldFlag_SchemaIdentifier) && pParameter->pSchemaIdentifiers != NULL)
        ember_writeString(pBerOutput, &glowTags.parameterContents.schemaIdentifiers, pParameter->pSchemaIdentifiers);

    ember_writeContainerEnd(pBerOutput); // end parameter.contents

    ember_writeContainerEnd(pBerOutput); // end parameter
}

void glow_writeTemplateElementFunction(
    GlowOutput *pOut,
    const BerTag *tag,
    const GlowFunction *pFunction,
    GlowFieldFlags fields,
    berint number)
{
    int index;
    BerOutput *pBerOutput;

    ASSERT(pOut != NULL);
    ASSERT(pFunction != NULL);

    pBerOutput = &pOut->base.base.base;

    if (tag == NULL)
        tag = &glowTags.elementCollection.element;

    ember_writeContainerBegin(pBerOutput, tag, GlowType_Function);
    ember_writeInteger(pBerOutput, &glowTags.function.number, number);
    ember_writeSetBegin(pBerOutput, &glowTags.function.contents);

    if ((fields & GlowFieldFlag_Identifier) == GlowFieldFlag_Identifier && pFunction->pIdentifier != NULL)
    {
        if (glow_assertIdentifierValid(pFunction->pIdentifier, false))
            ember_writeString(pBerOutput, &glowTags.functionContents.identifier, pFunction->pIdentifier);
    }

    if ((fields & GlowFieldFlag_Description) == GlowFieldFlag_Description && pFunction->pDescription != NULL)
        ember_writeString(pBerOutput, &glowTags.functionContents.description, pFunction->pDescription);


    if ((fields & GlowFieldFlag_TemplateReference) && pFunction->pTemplateReference != NULL)
        ember_writeRelativeOid(pBerOutput, &glowTags.functionContents.templateReference, pFunction->pTemplateReference, pFunction->templateReferenceLength);

    if (fields == GlowFieldFlag_All)
    {
        if (pFunction->pArguments != NULL)
        {
            ember_writeSequenceBegin(pBerOutput, &glowTags.functionContents.arguments);

            for (index = 0; index < pFunction->argumentsLength; index++)
                writeTupleItemDescription(pBerOutput, &glowTags.collection.item, &pFunction->pArguments[index]);

            ember_writeContainerEnd(pBerOutput);
        }

        if (pFunction->pResult != NULL)
        {
            ember_writeSequenceBegin(pBerOutput, &glowTags.functionContents.result);

            for (index = 0; index < pFunction->resultLength; index++)
                writeTupleItemDescription(pBerOutput, &glowTags.collection.item, &pFunction->pResult[index]);

            ember_writeContainerEnd(pBerOutput);
        }
    }

    ember_writeContainerEnd(pBerOutput); // end function.contents
    ember_writeContainerEnd(pBerOutput); // end function
}



void glow_writeQualifiedParameter(GlowOutput *pOut,
                                  const GlowParameter *pParameter,
                                  GlowFieldFlags fields,
                                  const berint *pPath,
                                  int pathLength)
{
   ASSERT(pOut != NULL);
   ASSERT(pParameter != NULL);
   ASSERT(pPath != NULL || pathLength == 0);
   ASSERT(pOut->positionHint == 0);

   glow_writeQualifiedParameterImpl(&pOut->base.base.base, pParameter, fields, pPath, pathLength);
}

void glow_writeQualifiedCommand(GlowOutput *pOut,
                                const GlowCommand *pCommand,
                                const berint *pPath,
                                int pathLength,
                                GlowElementType parentType)
{
   ASSERT(pOut != NULL);
   ASSERT(pCommand != NULL);
   ASSERT(pPath != NULL || pathLength == 0);
   ASSERT(pOut->positionHint == 0);

   glow_writeQualifiedCommandImpl(&pOut->base.base.base, pCommand, pPath, pathLength, parentType);
}

void glow_writeStreamEntry(GlowOutput *pOut, const GlowStreamEntry *pEntry)
{
   ASSERT(pOut != NULL);
   ASSERT(pEntry != NULL);
   ASSERT(pOut->positionHint == 0);

   glow_writeStreamEntryImpl(&pOut->base.base.base, pEntry);
}

void glow_writeQualifiedMatrix(GlowOutput *pOut,
                               const GlowMatrix *pMatrix,
                               GlowFieldFlags fields,
                               const berint *pPath,
                               int pathLength)
{
   ASSERT(pOut != NULL);
   ASSERT(pMatrix != NULL);
   ASSERT(pPath != NULL || pathLength == 0);
   ASSERT(pOut->positionHint == 0);

   glow_writeQualifiedMatrixImpl(&pOut->base.base.base, pMatrix, fields, pPath, pathLength);
}

void glow_writeTargetsPrefix(GlowOutput *pOut,
                             const berint *pMatrixPath,
                             int pathLength)
{
   ASSERT(pOut != NULL);
   ASSERT(pMatrixPath != NULL || pathLength == 0);
   ASSERT(pOut->positionHint == 0);
#ifdef _DEBUG
   pOut->positionHint = __GLOWOUTPUT_POSITION_TARGETS;
#endif

   glow_writeTargetsPrefixImpl(&pOut->base.base.base, pMatrixPath, pathLength);
}

void glow_writeTarget(GlowOutput *pOut, const GlowSignal *pTarget)
{
   ASSERT(pOut != NULL);
   ASSERT(pTarget != NULL);
   ASSERT(pOut->positionHint == __GLOWOUTPUT_POSITION_TARGETS);

   glow_writeTargetImpl(&pOut->base.base.base, pTarget);
}

void glow_writeTargetsSuffix(GlowOutput *pOut)
{
   ASSERT(pOut != NULL);
   ASSERT(pOut->positionHint == __GLOWOUTPUT_POSITION_TARGETS);
#ifdef _DEBUG
   pOut->positionHint = 0;
#endif

   writeDoubleContainerEnd(&pOut->base.base.base);
}

void glow_writeSourcesPrefix(GlowOutput *pOut,
                             const berint *pMatrixPath,
                             int pathLength)
{
   ASSERT(pOut != NULL);
   ASSERT(pMatrixPath != NULL || pathLength == 0);
   ASSERT(pOut->positionHint == 0);
#ifdef _DEBUG
   pOut->positionHint = __GLOWOUTPUT_POSITION_SOURCES;
#endif

   glow_writeSourcesPrefixImpl(&pOut->base.base.base, pMatrixPath, pathLength);
}

void glow_writeSource(GlowOutput *pOut, const GlowSignal *pSource)
{
   ASSERT(pOut != NULL);
   ASSERT(pSource != NULL);
   ASSERT(pOut->positionHint == __GLOWOUTPUT_POSITION_SOURCES);

   glow_writeSourceImpl(&pOut->base.base.base, pSource);
}

void glow_writeSourcesSuffix(GlowOutput *pOut)
{
   ASSERT(pOut != NULL);
   ASSERT(pOut->positionHint == __GLOWOUTPUT_POSITION_SOURCES);
#ifdef _DEBUG
   pOut->positionHint = 0;
#endif

   writeDoubleContainerEnd(&pOut->base.base.base);
}

void glow_writeConnectionsPrefix(GlowOutput *pOut,
                                 const berint *pMatrixPath,
                                 int pathLength)
{
   ASSERT(pOut != NULL);
   ASSERT(pMatrixPath != NULL || pathLength == 0);
   ASSERT(pOut->positionHint == 0);
#ifdef _DEBUG
   pOut->positionHint = __GLOWOUTPUT_POSITION_CONNECTIONS;
#endif

   glow_writeConnectionsPrefixImpl(&pOut->base.base.base, pMatrixPath, pathLength);
}

void glow_writeConnection(GlowOutput *pOut, const GlowConnection *pConnection)
{
   ASSERT(pOut != NULL);
   ASSERT(pConnection != NULL);
   ASSERT(pOut->positionHint == __GLOWOUTPUT_POSITION_CONNECTIONS);

   glow_writeConnectionImpl(&pOut->base.base.base, pConnection);
}

void glow_writeConnectionsSuffix(GlowOutput *pOut)
{
   ASSERT(pOut != NULL);
   ASSERT(pOut->positionHint == __GLOWOUTPUT_POSITION_CONNECTIONS);
#ifdef _DEBUG
   pOut->positionHint = 0;
#endif

   writeDoubleContainerEnd(&pOut->base.base.base);
}

void glow_writeQualifiedFunction(GlowOutput *pOut,
                                 const GlowFunction *pFunction,
                                 GlowFieldFlags fields,
                                 const berint *pPath,
                                 int pathLength)
{
   ASSERT(pOut != NULL);
   ASSERT(pFunction != NULL);
   ASSERT(pOut->positionHint == 0);

   glow_writeQualifiedFunctionImpl(&pOut->base.base.base, pFunction, fields, pPath, pathLength);
}

unsigned int glow_writeInvocationResultPackage(GlowOutput *pOut, const GlowInvocationResult *pRoot)
{
   ASSERT(pOut != NULL);
   ASSERT(pRoot != NULL);
   ASSERT(pOut->positionHint == 0);
   ASSERT(pOut->packageCount == 0);

   writeFramingHeader(pOut, true, true);

   glow_writeInvocationResultImpl(&pOut->base.base.base, &glowTags.root, pRoot);

   return berFramingOutput_finish(&pOut->base);
}
