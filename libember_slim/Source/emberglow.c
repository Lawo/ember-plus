/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol
   Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <string.h>
#include "emberglow.h"
#include "emberinternal.h"

const __GlowTags glowTags =
{
   // parameter
   {
      {BerClass_ContextSpecific, 0},   // number
      {BerClass_ContextSpecific, 1},   // contents
      {BerClass_ContextSpecific, 2},   // children
   },

   // qualifiedParameter
   {
      {BerClass_ContextSpecific, 0},   // path
      {BerClass_ContextSpecific, 1},   // contents
      {BerClass_ContextSpecific, 2},   // children
   },

   // parameterContents
   {
      {BerClass_ContextSpecific, 0},   // contents.identifier
      {BerClass_ContextSpecific, 1},   // contents.description
      {BerClass_ContextSpecific, 2},   // contents.value
      {BerClass_ContextSpecific, 3},   // contents.minimum
      {BerClass_ContextSpecific, 4},   // contents.maximum
      {BerClass_ContextSpecific, 5},   // contents.isWriteable
      {BerClass_ContextSpecific, 6},   // contents.format
      {BerClass_ContextSpecific, 7},   // contents.enumeration
      {BerClass_ContextSpecific, 8},   // contents.factor
      {BerClass_ContextSpecific, 9},   // contents.isOnline
      {BerClass_ContextSpecific, 10},  // contents.formula
      {BerClass_ContextSpecific, 11},  // contents.step
      {BerClass_ContextSpecific, 12},  // contents.default
      {BerClass_ContextSpecific, 13},  // contents.type
      {BerClass_ContextSpecific, 14},  // contents.streamIdentifier
      {BerClass_ContextSpecific, 15},  // contents.enumMap
      {BerClass_ContextSpecific, 16},  // contents.streamDescriptor
   },

   // node
   {
      {BerClass_ContextSpecific, 0},   // number
      {BerClass_ContextSpecific, 1},   // contents
      {BerClass_ContextSpecific, 2},   // children
   },

   // qualifiedNode
   {
      {BerClass_ContextSpecific, 0},   // path
      {BerClass_ContextSpecific, 1},   // contents
      {BerClass_ContextSpecific, 2},   // children
   },

   // nodeContents
   {
      {BerClass_ContextSpecific, 0},   // contents.identifier
      {BerClass_ContextSpecific, 1},   // contents.description
      {BerClass_ContextSpecific, 2},   // contents.isRoot
      {BerClass_ContextSpecific, 3},   // contents.isOnline
   },

   // command
   {
      {BerClass_ContextSpecific, 0},   // number
      {BerClass_ContextSpecific, 1},   // dirFieldMask
   },

   // elementCollection
   {
      {BerClass_ContextSpecific, 0},   // element
   },

   // streamEntry
   {
      {BerClass_ContextSpecific, 0},   // streamIdentifier
      {BerClass_ContextSpecific, 1},   // streamValue
   },

   // streamCollection
   {
      {BerClass_ContextSpecific, 0},   // streamEntry
   },

   // stringIntegerPair
   {
      {BerClass_ContextSpecific, 0},   // entryString
      {BerClass_ContextSpecific, 1},   // entryInteger
   },

   // stringIntegerCollection
   {
      {BerClass_ContextSpecific, 0},   // stringIntegerPair
   },

   // streamDescription
   {
      {BerClass_ContextSpecific, 0},   // format
      {BerClass_ContextSpecific, 1},   // offset
   },

   // matrix
   {
      {BerClass_ContextSpecific, 0},   // number
      {BerClass_ContextSpecific, 1},   // contents
      {BerClass_ContextSpecific, 2},   // children
      {BerClass_ContextSpecific, 3},   // targets
      {BerClass_ContextSpecific, 4},   // sources
      {BerClass_ContextSpecific, 5},   // connections
   },

   // qualifiedMatrix
   {
      {BerClass_ContextSpecific, 0},   // path
      {BerClass_ContextSpecific, 1},   // contents
      {BerClass_ContextSpecific, 2},   // children
      {BerClass_ContextSpecific, 3},   // targets
      {BerClass_ContextSpecific, 4},   // sources
      {BerClass_ContextSpecific, 5},   // connections
   },

   // matrixContents
   {
      {BerClass_ContextSpecific, 0},   // identifier
      {BerClass_ContextSpecific, 1},   // description
      {BerClass_ContextSpecific, 2},   // type
      {BerClass_ContextSpecific, 3},   // addressingMode
      {BerClass_ContextSpecific, 4},   // targetCount
      {BerClass_ContextSpecific, 5},   // sourceCount
      {BerClass_ContextSpecific, 6},   // maximumTotalConnects
      {BerClass_ContextSpecific, 7},   // maximumConnectsPerTarget
      {BerClass_ContextSpecific, 8},   // parametersBasePath
      {BerClass_ContextSpecific, 9},   // labels
   },

   // label
   {
      {BerClass_ContextSpecific, 0},   // basePath
      {BerClass_ContextSpecific, 1},   // description
   },

   // signal
   {
      {BerClass_ContextSpecific, 0},   // number
   },

   // connection
   {
      {BerClass_ContextSpecific, 0},   // target
      {BerClass_ContextSpecific, 1},   // sources
      {BerClass_ContextSpecific, 2},   // operation
      {BerClass_ContextSpecific, 3},   // disposition
   },

   // collection
   {
      {BerClass_ContextSpecific, 0},   // item
   },

   // root
   {BerClass_Application, 0},
};


// ====================================================================
//
// DOM support globals
//
// ====================================================================

bool glowParametersLocation_isValid(const GlowParametersLocation *pThis)
{
   return pThis->kind == GlowParametersLocationKind_Inline
       || pThis->basePath.length > 0;
}

void glowNode_free(GlowNode *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->pIdentifier != NULL)
      freeMemory(pThis->pIdentifier);

   if(pThis->pDescription != NULL)
      freeMemory(pThis->pDescription);

   bzero(*pThis);
}

void glowValue_copyTo(const GlowValue *pThis, GlowValue *pDest)
{
   size_t length;

   ASSERT(pThis != NULL);
   ASSERT(pDest != NULL);

   pDest->flag = pThis->flag;

   switch(pThis->flag)
   {
      case GlowParameterType_String:
         if(pThis->pString != NULL)
         {
            length = strlen(pThis->pString) + 1;
            pDest->pString = newarr(char, length);
            memcpy(pDest->pString, pThis->pString, length);
         }
         else
         {
            pDest->pString = NULL;
         }
         break;

      case GlowParameterType_Octets:
         if(pThis->octets.pOctets != NULL)
         {
            pDest->octets.pOctets = newarr(byte, pThis->octets.length);
            pDest->octets.length = pThis->octets.length;
            memcpy(pDest->octets.pOctets, pThis->octets.pOctets, pThis->octets.length);
         }
         else
         {
            pDest->octets.length = 0;
            pDest->octets.pOctets = NULL;
         }
         break;

      default:
         memcpy(pDest, pThis, sizeof(GlowValue));
         break;
   }
}

void glowValue_free(GlowValue *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->flag == GlowParameterType_String)
   {
      if(pThis->pString != NULL)
         freeMemory(pThis->pString);
   }
   else if(pThis->flag == GlowParameterType_Octets)
   {
      if(pThis->octets.pOctets != NULL)
         freeMemory(pThis->octets.pOctets);
   }

   bzero(*pThis);
}

void glowParameter_free(GlowParameter *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->pIdentifier != NULL)
      freeMemory(pThis->pIdentifier);

   if(pThis->pDescription != NULL)
      freeMemory(pThis->pDescription);

   if(pThis->value.flag != 0)
      glowValue_free(&pThis->value);

   bzero(*pThis);
}

void glowMatrix_free(GlowMatrix *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->pIdentifier != NULL)
      freeMemory(pThis->pIdentifier);

   if(pThis->pDescription != NULL)
      freeMemory(pThis->pDescription);

   bzero(*pThis);
}

void glowConnection_free(GlowConnection *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->pSources != NULL)
      freeMemory(pThis->pSources);

   bzero(*pThis);
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
         ember_writeLong(pOut, pTag, pMinMax->integer);
         break;

      case GlowParameterType_Real:
         ember_writeReal(pOut, pTag, pMinMax->real);
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
         ember_writeLong(pOut, pTag, pValue->integer);
         break;

      case GlowParameterType_Real:
         ember_writeReal(pOut, pTag, pValue->real);
         break;

      case GlowParameterType_String:
         ember_writeString(pOut, pTag, pValue->pString);
         break;

      case GlowParameterType_Boolean:
         ember_writeBoolean(pOut, pTag, pValue->boolean);
         break;

      case GlowParameterType_Octets:
         ember_writeOctetString(pOut, pTag, pValue->octets.pOctets, pValue->octets.length);
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

static bool assertIdentifierValid(pcstr pIdentifier, bool isRx)
{
   pcstr pch;
   int error = isRx ? 512 : 511;

   if(pIdentifier == NULL)
   {
      throwError(error, "identifier must not be NULL!");
      return false;
   }

   if(pIdentifier[0] == 0)
   {
      throwError(error, "identifier must not be empty!");
      return false;
   }

   if(*pIdentifier >= '0' && *pIdentifier <= '9')
   {
      throwError(error, "identifier must not begin with a number!");
      return false;
   }

   for(pch = pIdentifier; *pch != 0; pch++)
   {
      if(*pch == '/')
      {
         throwError(error, "identifier must not contain the '/' character!");
         return false;
      }
   }

   return true;
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

   if(pCommand->dirFieldMask != 0)
      ember_writeInteger(pOut, &glowTags.command.dirFieldMask, pCommand->dirFieldMask);

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
         if(assertIdentifierValid(pNode->pIdentifier, false))
            ember_writeString(pOut, &glowTags.nodeContents.identifier, pNode->pIdentifier);
      }

      if((fields & GlowFieldFlag_Description) == GlowFieldFlag_Description
      && pNode->pDescription != NULL)
         ember_writeString(pOut, &glowTags.nodeContents.description, pNode->pDescription);

      if(fields & GlowFieldFlag_IsRoot)
         ember_writeBoolean(pOut, &glowTags.nodeContents.isRoot, pNode->isRoot);

      if(fields & GlowFieldFlag_IsOnline)
         ember_writeBoolean(pOut, &glowTags.nodeContents.isOnline, pNode->isOnline);

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
      if(assertIdentifierValid(pParameter->pIdentifier, false))
         ember_writeString(pOut, &glowTags.parameterContents.identifier, pParameter->pIdentifier);
   }

   if((fields & GlowFieldFlag_Description) == GlowFieldFlag_Description
   && pParameter->pDescription != NULL)
      ember_writeString(pOut, &glowTags.parameterContents.description, pParameter->pDescription);

   if(fields & GlowFieldFlag_Value)
      writeValue(pOut, &glowTags.parameterContents.value, &pParameter->value);

   if(fields & GlowFieldFlag_Minimum)
      writeMinMax(pOut, &glowTags.parameterContents.minimum, &pParameter->minimum);

   if(fields & GlowFieldFlag_Maximum)
      writeMinMax(pOut, &glowTags.parameterContents.maximum, &pParameter->maximum);

   if(fields & GlowFieldFlag_Access)
      ember_writeInteger(pOut, &glowTags.parameterContents.access, (berint)pParameter->access);

   if((fields & GlowFieldFlag_Format) && pParameter->format != NULL)
      ember_writeString(pOut, &glowTags.parameterContents.format, pParameter->format);

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

   if((fields & GlowFieldFlag_Formula) && pParameter->formula != NULL)
      ember_writeString(pOut, &glowTags.parameterContents.formula, pParameter->formula);

   if((fields & GlowFieldFlag_Enumeration) && pParameter->enumeration != NULL)
      ember_writeString(pOut, &glowTags.parameterContents.enumeration, pParameter->enumeration);

   if(fields & GlowFieldFlag_StreamDescriptor)
   {
      ember_writeContainerBegin(pOut, &glowTags.parameterContents.streamDescriptor, GlowType_StreamDescription);
      ember_writeInteger(pOut, &glowTags.streamDescription.format, pParameter->streamDescriptor.format);
      ember_writeInteger(pOut, &glowTags.streamDescription.offset, pParameter->streamDescriptor.offset);
      ember_writeContainerEnd(pOut);
   }
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

         default:
            ASSERT(false);
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

   ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedMatrix);
   ember_writeRelativeOid(pOut, &glowTags.qualifiedMatrix.path, pPath, pathLength);

   ember_writeSetBegin(pOut, &glowTags.qualifiedMatrix.contents);

   if((fields & GlowFieldFlag_Identifier) == GlowFieldFlag_Identifier
   && pMatrix->pIdentifier != NULL)
   {
      if(assertIdentifierValid(pMatrix->pIdentifier, false))
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
            ember_writeRelativeOid(pOut, &glowTags.matrixContents.parametersLocation, pParams->basePath.ids, pParams->basePath.length);
         }
         else if(pParams->kind == GlowParametersLocationKind_Inline)
         {
            ember_writeInteger(pOut, &glowTags.matrixContents.parametersLocation, pParams->inlineId);
         }
      }
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

   glow_writeTargetsPrefixImpl(&pOut->base.base.base, pMatrixPath, pathLength);
}

void glow_writeSource(GlowOutput *pOut, const GlowSignal *pSource)
{
   ASSERT(pOut != NULL);
   ASSERT(pSource != NULL);
   ASSERT(pOut->positionHint == __GLOWOUTPUT_POSITION_SOURCES);

   glow_writeTargetImpl(&pOut->base.base.base, pSource);
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
// NonFramingGlowReader locals
//
// ====================================================================

static void readValue(const BerReader *pBase, GlowValue *pValue)
{
   switch(pBase->type)
   {
      case BerType_Integer:
         pValue->flag = GlowParameterType_Integer;
         pValue->integer = berReader_getLong(pBase);
         break;

      case BerType_Real:
         pValue->flag = GlowParameterType_Real;
         pValue->real = berReader_getReal(pBase);
         break;

      case BerType_UTF8String:
         pValue->flag = GlowParameterType_String;
         pValue->pString = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pValue->pString, pBase->length);
         break;

      case BerType_Boolean:
         pValue->flag = GlowParameterType_Boolean;
         pValue->boolean = berReader_getBoolean(pBase);
         break;

      case BerType_OctetString:
         pValue->flag = GlowParameterType_Octets;
         if(pBase->length > 0)
         {
            pValue->octets.pOctets = newarr(byte, pBase->length);
            pValue->octets.length = berReader_getOctetString(pBase, pValue->octets.pOctets, pBase->length);
         }
         else
         {
            pValue->octets.pOctets = NULL;
            pValue->octets.length = 0;
         }
         break;

      default:
         throwError(507, "BerReader reports unsupported GlowValue type!");
         break;
   }
}

static void readMinMax(const BerReader *pBase, GlowMinMax *pMinMax)
{
   switch(pBase->type)
   {
      case BerType_Integer:
         pMinMax->flag = GlowParameterType_Integer;
         pMinMax->integer = berReader_getLong(pBase);
         break;

      case BerType_Real:
         pMinMax->flag = GlowParameterType_Real;
         pMinMax->real = berReader_getReal(pBase);
         break;

      default:
         throwError(508, "BerReader reports unsupported GlowMinMax type!");
         break;
   }
}

typedef void (*onItemReady_t)(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase);

static void onItemReady_Node(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   berint number;

   if(pBase->isContainer == false)
   {
      if(berTag_equals(&pBase->tag, &glowTags.node.number))
      {
         number = berReader_getInteger(pBase);

         pThis->path[pThis->pathLength] = number;
         pThis->pathLength++;
      }
   }
}

static void onItemReady_QualifiedNode(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   if(pBase->isContainer)
   {
      pThis->pathLength = 0;
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.qualifiedNode.path))
      {
         if(pThis->pathLength != 0)
            throwError(502, "QualifiedNode encountered on non-root level!");

         pThis->pathLength = berReader_getRelativeOid(pBase, pThis->path, GLOW_MAX_TREE_DEPTH);

         if(pThis->pathLength == 0)
            throwError(501, "empty path in QualifiedNode!");
      }
   }
}

static void onItemReady_NodeContents(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   int fields = pThis->fields;
   const BerTag *pTag = &pBase->tag;

   if(pBase->isContainer)
   {
      if(pThis->onNode != NULL)
         pThis->onNode(&pThis->node, pThis->fields, pThis->path, pThis->pathLength, pThis->state);

      glowNode_free(&pThis->node);
      pThis->fields = GlowFieldFlag_None;
   }
   else
   {
      if(berTag_equals(pTag, &glowTags.nodeContents.identifier))
      {
         pThis->node.pIdentifier = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->node.pIdentifier, pBase->length + 1);
         assertIdentifierValid(pThis->node.pIdentifier, true);
         fields |= GlowFieldFlag_Identifier;
      }
      else if(berTag_equals(pTag, &glowTags.nodeContents.description))
      {
         pThis->node.pDescription = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->node.pDescription, pBase->length + 1);
         fields |= GlowFieldFlag_Description;
      }
      else if(berTag_equals(pTag, &glowTags.nodeContents.isRoot))
      {
         pThis->node.isRoot = berReader_getBoolean(pBase);
         fields |= GlowFieldFlag_IsRoot;
      }
      else if(berTag_equals(pTag, &glowTags.nodeContents.isOnline))
      {
         pThis->node.isOnline = berReader_getBoolean(pBase);
         fields |= GlowFieldFlag_IsOnline;
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_NodeContents, pThis->state);
      }

      pThis->fields = (GlowFieldFlags)fields;
   }
}

static void onItemReady_Parameter(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   berint number;

   if(pBase->isContainer == false)
   {
      if(berTag_equals(&pBase->tag, &glowTags.parameter.number))
      {
         number = berReader_getInteger(pBase);

         pThis->path[pThis->pathLength] = number;
         pThis->pathLength++;
      }
   }
}

static void onItemReady_QualifiedParameter(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   if(pBase->isContainer)
   {
      pThis->pathLength = 0;
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.qualifiedParameter.path))
      {
         if(pThis->pathLength != 0)
            throwError(504, "QualifiedParameter encountered on non-root level!");

         pThis->pathLength = berReader_getRelativeOid(pBase, pThis->path, GLOW_MAX_TREE_DEPTH);

         if(pThis->pathLength == 0)
            throwError(503, "empty path in QualifiedParameter!");
      }
   }
}

static void onItemReady_ParameterContents(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   int fields = pThis->fields;
   const BerTag *pTag = &pBase->tag;

   if(pBase->isContainer)
   {
      if(pThis->onParameter != NULL)
         pThis->onParameter(&pThis->parameter, pThis->fields, pThis->path, pThis->pathLength, pThis->state);

      // reset read parameter
      glowParameter_free(&pThis->parameter);
      pThis->fields = GlowFieldFlag_None;
   }
   else
   {
      if(berTag_equals(pTag, &glowTags.parameterContents.identifier))
      {
         pThis->parameter.pIdentifier = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->parameter.pIdentifier, pBase->length + 1);
         assertIdentifierValid(pThis->parameter.pIdentifier, true);
         fields |= GlowFieldFlag_Identifier;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.description))
      {
         pThis->parameter.pDescription = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->parameter.pDescription, pBase->length + 1);
         fields |= GlowFieldFlag_Description;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.value))
      {
         readValue(pBase, &pThis->parameter.value);
         fields |= GlowFieldFlag_Value;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.minimum))
      {
         readMinMax(pBase, &pThis->parameter.minimum);
         fields |= GlowFieldFlag_Minimum;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.maximum))
      {
         readMinMax(pBase, &pThis->parameter.maximum);
         fields |= GlowFieldFlag_Maximum;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.access))
      {
         pThis->parameter.access = (GlowAccess)berReader_getInteger(pBase);
         fields |= GlowFieldFlag_Access;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.factor))
      {
         pThis->parameter.factor = berReader_getInteger(pBase);
         fields |= GlowFieldFlag_Factor;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.isOnline))
      {
         pThis->parameter.isOnline = berReader_getBoolean(pBase);
         fields |= GlowFieldFlag_IsOnline;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.step))
      {
         pThis->parameter.step = berReader_getInteger(pBase);
         fields |= GlowFieldFlag_Step;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.type))
      {
         pThis->parameter.type = (GlowParameterType)berReader_getInteger(pBase);
         fields |= GlowFieldFlag_Type;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.streamIdentifier))
      {
         pThis->parameter.streamIdentifier = berReader_getInteger(pBase);
         fields |= GlowFieldFlag_StreamIdentifier;
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_ParameterContents, pThis->state);
      }

      pThis->fields = (GlowFieldFlags)fields;
   }
}

static void onItemReady_StreamDescription(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   const BerTag *pTag = &pBase->tag;

   if(pBase->isContainer)
   {
      pThis->fields = (GlowFieldFlags)(pThis->fields | GlowFieldFlag_StreamDescriptor);
   }
   else
   {
      if(berTag_equals(pTag, &glowTags.streamDescription.format))
      {
         pThis->parameter.streamDescriptor.format = (GlowStreamFormat)berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.streamDescription.offset))
      {
         pThis->parameter.streamDescriptor.offset = berReader_getInteger(pBase);
      }
   }
}

static void onItemReady_Command(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   if(pBase->isContainer)
   {
      if(pThis->command.number == GlowCommandType_GetDirectory
      && pThis->command.dirFieldMask == 0)
         pThis->command.dirFieldMask = GlowFieldFlag_All;

      if(pThis->onCommand != NULL)
         pThis->onCommand(&pThis->command, pThis->path, pThis->pathLength, pThis->state);
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.command.number))
      {
         pThis->command.number = berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.command.dirFieldMask))
      {
         pThis->command.dirFieldMask = (GlowFieldFlags)berReader_getInteger(pBase);
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_Command, pThis->state);
      }
   }
}

static void onItemReady_StreamEntry(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   if(pBase->isContainer)
   {
      if(pThis->onStreamEntry != NULL)
         pThis->onStreamEntry(&pThis->streamEntry, pThis->state);

      glowValue_free(&pThis->streamEntry.streamValue);
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.streamEntry.streamIdentifier))
      {
         pThis->streamEntry.streamIdentifier = berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.streamEntry.streamValue))
      {
         readValue(pBase, &pThis->streamEntry.streamValue);
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_StreamEntry, pThis->state);
      }
   }
}

static void onItemReady_Matrix(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   berint number;

   if(pBase->isContainer == false)
   {
      if(berTag_equals(&pBase->tag, &glowTags.matrix.number))
      {
         number = berReader_getInteger(pBase);

         pThis->path[pThis->pathLength] = number;
         pThis->pathLength++;
      }
   }
}

static void onItemReady_QualifiedMatrix(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   if(pBase->isContainer)
   {
      pThis->pathLength = 0;
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.qualifiedMatrix.path))
      {
         if(pThis->pathLength != 0)
            throwError(509, "QualifiedMatrix encountered on non-root level!");

         pThis->pathLength = berReader_getRelativeOid(pBase, pThis->path, GLOW_MAX_TREE_DEPTH);

         if(pThis->pathLength == 0)
            throwError(510, "empty path in QualifiedMatrix!");
      }
   }
}

static void onItemReady_MatrixContents(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   const BerTag *pTag = &pBase->tag;
   GlowParametersLocation *pParams;

   if(pBase->isContainer)
   {
      if(pThis->onMatrix != NULL)
         pThis->onMatrix(&pThis->matrix, pThis->path, pThis->pathLength, pThis->state);

      // reset read matrix
      glowMatrix_free(&pThis->matrix);
   }
   else
   {
      if(berTag_equals(pTag, &glowTags.matrixContents.identifier))
      {
         pThis->matrix.pIdentifier = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->matrix.pIdentifier, pBase->length + 1);
         assertIdentifierValid(pThis->matrix.pIdentifier, true);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.description))
      {
         pThis->matrix.pDescription = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->matrix.pDescription, pBase->length + 1);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.type))
      {
         pThis->matrix.type = (GlowMatrixType)berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.addressingMode))
      {
         pThis->matrix.addressingMode = (GlowMatrixAddressingMode)berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.targetCount))
      {
         pThis->matrix.targetCount = berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.sourceCount))
      {
         pThis->matrix.sourceCount = berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.maximumTotalConnects))
      {
         pThis->matrix.maximumTotalConnects = berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.maximumConnectsPerTarget))
      {
         pThis->matrix.maximumConnectsPerTarget = berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.parametersLocation))
      {
         pParams = &pThis->matrix.parametersLocation;

         if(pBase->type == BerType_RelativeOid)
         {
            pParams->basePath.length = berReader_getRelativeOid(pBase, pParams->basePath.ids, GLOW_MAX_TREE_DEPTH);
            pParams->kind = GlowParametersLocationKind_BasePath;
         }
         else if(pBase->type == BerType_Integer)
         {
            pParams->inlineId = berReader_getInteger(pBase);
            pParams->kind = GlowParametersLocationKind_Inline;
         }
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_MatrixContents, pThis->state);
      }
   }
}

static void onItemReady_Signal(NonFramingGlowReader *pThis, const BerReader *pBase, bool isTarget)
{
   if(pBase->isContainer)
   {
      if(isTarget)
      {
         if(pThis->onTarget != NULL)
            pThis->onTarget(&pThis->signal, pThis->path, pThis->pathLength, pThis->state);
      }
      else
      {
         if(pThis->onSource != NULL)
            pThis->onSource(&pThis->signal, pThis->path, pThis->pathLength, pThis->state);
      }

      // reset read signal
      bzero(pThis->signal);
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.signal.number))
      {
         pThis->signal.number = berReader_getInteger(pBase);
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_Target, pThis->state);
      }
   }
}

static void onItemReady_Target(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   onItemReady_Signal(pThis, pBase, true);
}

static void onItemReady_Source(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   onItemReady_Signal(pThis, pBase, false);
}

static void onItemReady_Connection(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   if(pBase->isContainer)
   {
      if(pThis->onConnection != NULL)
         pThis->onConnection(&pThis->connection, pThis->path, pThis->pathLength, pThis->state);

      // reset read connection
      glowConnection_free(&pThis->connection);
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.connection.target))
      {
         pThis->connection.target = berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.connection.sources))
      {
         if(pBase->length > 0)
         {
            pThis->connection.pSources = newarr(berint, pBase->length);
            pThis->connection.sourcesLength = berReader_getRelativeOid(pBase, pThis->connection.pSources, pBase->length);
         }
         else
         {
            pThis->connection.pSources = NULL;
            pThis->connection.sourcesLength = 0;
         }
      }
      else if(berTag_equals(&pBase->tag, &glowTags.connection.operation))
      {
         pThis->connection.operation = (GlowConnectionOperation)berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.connection.disposition))
      {
         pThis->connection.disposition = (GlowConnectionDisposition)berReader_getInteger(pBase);
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_Target, pThis->state);
      }
   }
}

onItemReady_t getOnItemReady_EnterContainer(const BerReader *pBase)
{
   EmberAsyncReader *pAsync = (EmberAsyncReader *)pBase;
   NonFramingGlowReader *pThis = (NonFramingGlowReader *)pBase;
   __EmberAsyncContainerStack *pStack;
   __EmberAsyncContainer *pContainer;

   switch(pBase->type)
   {
      case GlowType_Node:
         bzero(pThis->node);
         pThis->node.isOnline = true;
         return onItemReady_Node;
      case GlowType_QualifiedNode:
         bzero(pThis->node);
         pThis->node.isOnline = true;
         return onItemReady_QualifiedNode;
      case GlowType_Parameter:
         bzero(pThis->parameter);
         return onItemReady_Parameter;
      case GlowType_QualifiedParameter:
         bzero(pThis->node);
         return onItemReady_QualifiedParameter;
      case GlowType_Command:
         bzero(pThis->command);
         return onItemReady_Command;
      case GlowType_StreamEntry:
         bzero(pThis->streamEntry);
         return onItemReady_StreamEntry;
      case GlowType_StreamDescription:
         bzero(pThis->parameter.streamDescriptor);
         return onItemReady_StreamDescription;
      case GlowType_Matrix:
         bzero(pThis->matrix);
         return onItemReady_Matrix;
      case GlowType_QualifiedMatrix:
         bzero(pThis->matrix);
         return onItemReady_QualifiedMatrix;
      case GlowType_Target:
         bzero(pThis->signal);
         return onItemReady_Target;
      case GlowType_Source:
         bzero(pThis->signal);
         return onItemReady_Source;
      case GlowType_Connection:
         bzero(pThis->connection);
         return onItemReady_Connection;
      default:
         pStack = pAsync->pContainerStack;

         if(pStack->length > 0)
         {
            pContainer = &pStack->items[pStack->length - 1];

            if(pBase->type == BerType_Set)
            {
               if(berTag_equals(&pBase->tag, &glowTags.parameter.contents)
               && (pContainer->type == GlowType_Parameter || pContainer->type == GlowType_QualifiedParameter))
                  return onItemReady_ParameterContents;

               if(berTag_equals(&pBase->tag, &glowTags.node.contents)
               && (pContainer->type == GlowType_Node || pContainer->type == GlowType_QualifiedNode))
                  return onItemReady_NodeContents;

               if(berTag_equals(&pBase->tag, &glowTags.matrix.contents)
               && (pContainer->type == GlowType_Matrix || pContainer->type == GlowType_QualifiedMatrix))
                  return onItemReady_MatrixContents;
            }
         }
         break;
   }

   return NULL;
}

onItemReady_t getOnItemReady_LeaveContainer(const BerReader *pBase)
{
   EmberAsyncReader *pAsync = (EmberAsyncReader *)pBase;
   __EmberAsyncContainerStack *pStack = pAsync->pContainerStack;
   __EmberAsyncContainer *pContainer;
   __EmberAsyncContainer *pParentContainer;

   if(pStack->length > 0)
   {
      pContainer = &pStack->items[pStack->length - 1];

      switch(pContainer->type)
      {
         case GlowType_Node: return onItemReady_Node;
         case GlowType_QualifiedNode: return onItemReady_QualifiedNode;
         case GlowType_Parameter: return onItemReady_Parameter;
         case GlowType_QualifiedParameter: return onItemReady_QualifiedParameter;
         case GlowType_Command: return onItemReady_Command;
         case GlowType_StreamEntry: return onItemReady_StreamEntry;
         case GlowType_StreamDescription: return onItemReady_StreamDescription;
         case GlowType_Matrix: return onItemReady_Matrix;
         case GlowType_QualifiedMatrix: return onItemReady_QualifiedMatrix;
         case GlowType_Target: return onItemReady_Target;
         case GlowType_Source: return onItemReady_Source;
         case GlowType_Connection: return onItemReady_Connection;
         default:
            if(pStack->length > 1)
            {
               pParentContainer = &pStack->items[pStack->length - 2];

               if(pContainer->type == BerType_Set)
               {
                  if(berTag_equals(&pContainer->tag, &glowTags.parameter.contents)
                  && (pParentContainer->type == GlowType_Parameter || pParentContainer->type == GlowType_QualifiedParameter))
                     return onItemReady_ParameterContents;

                  if(berTag_equals(&pContainer->tag, &glowTags.node.contents)
                  && (pParentContainer->type == GlowType_Node || pParentContainer->type == GlowType_QualifiedNode))
                     return onItemReady_NodeContents;

                  if(berTag_equals(&pContainer->tag, &glowTags.matrix.contents)
                  && (pParentContainer->type == GlowType_Matrix || pParentContainer->type == GlowType_QualifiedMatrix))
                     return onItemReady_MatrixContents;
               }
            }
            break;
      }
   }

   return NULL;
}

static void onItemReady(const BerReader *pBase)
{
   NonFramingGlowReader *pThis = (NonFramingGlowReader *)pBase;

   if(pThis->onItemReadyState != NULL)
      pThis->onItemReadyState(pThis, (EmberAsyncReader *)pBase, pBase);

   if(pBase->isContainer)
   {
      if(pBase->type == GlowType_Node
      || pBase->type == GlowType_Parameter
      || pBase->type == GlowType_Matrix)
         pThis->pathLength--;

      pThis->onItemReadyState = getOnItemReady_LeaveContainer(pBase);
   }
}

static void onNewContainer(const BerReader *pBase)
{
   NonFramingGlowReader *pThis = (NonFramingGlowReader *)pBase;

   if(berTag_equals(&pBase->tag, &glowTags.root))
      pThis->pathLength = 0;

   pThis->onItemReadyState = getOnItemReady_EnterContainer(pBase);
}


// ====================================================================
//
// NonFramingGlowReader globals
//
// ====================================================================

void nonFramingGlowReader_init(NonFramingGlowReader *pThis,
                               onNode_t onNode,
                               onParameter_t onParameter,
                               onCommand_t onCommand,
                               onStreamEntry_t onStreamEntry,
                               voidptr state)
{
   ASSERT(pThis != NULL);

   bzero(*pThis);

   emberAsyncReader_init(&pThis->base);

   pThis->base.onNewContainer = onNewContainer;
   pThis->base.onItemReady = onItemReady;

   pThis->onNode = onNode;
   pThis->onParameter = onParameter;
   pThis->onCommand = onCommand;
   pThis->onStreamEntry = onStreamEntry;

   pThis->state = state;
}

void nonFramingGlowReader_free(NonFramingGlowReader *pThis)
{
   ASSERT(pThis != NULL);

   emberAsyncReader_free(&pThis->base);

   bzero(*pThis);
}

void nonFramingGlowReader_reset(NonFramingGlowReader *pThis)
{
   ASSERT(pThis != NULL);

   pThis->pathLength = 0;
   emberAsyncReader_reset(&pThis->base);
}


// ====================================================================
//
// GlowReader locals
//
// ====================================================================

static void onPackageReceived(const byte *pPackage, int length, voidptr state)
{
   GlowReader *pThis = (GlowReader *)state;
   int appBytesCount;
   int headerLength;

   ASSERT(pThis != NULL);
   ASSERT(pPackage != NULL);

   if(pThis->onPackageReceived != NULL)
      pThis->onPackageReceived(pPackage, length, pThis->base.state);

   if(length >= 7
   && pPackage[1] == EMBER_MESSAGE_ID
   && pPackage[2] == EMBER_COMMAND_PAYLOAD
   && pPackage[5] == EMBER_DTD_GLOW)
   {
      appBytesCount = pPackage[6];
      headerLength = 7 + appBytesCount;

      if(pPackage[4] & EmberFramingFlag_FirstPackage)
         emberAsyncReader_reset(&pThis->base.base);

      emberAsyncReader_readBytes(&pThis->base.base, pPackage + headerLength, length - headerLength);

      if(pPackage[4] & EmberFramingFlag_LastPackage)
         glowReader_reset(pThis);
   }
}


// ====================================================================
//
// GlowReader globals
//
// ====================================================================

void glowReader_init(GlowReader *pThis,
                     onNode_t onNode,
                     onParameter_t onParameter,
                     onCommand_t onCommand,
                     onStreamEntry_t onStreamEntry,
                     voidptr state,
                     byte *pRxBuffer,
                     unsigned int rxBufferSize)
{
   ASSERT(pThis != NULL);
   ASSERT(pRxBuffer != NULL);
   ASSERT(rxBufferSize > 0);

   pThis->onPackageReceived = NULL;
   nonFramingGlowReader_init(&pThis->base, onNode, onParameter, onCommand, onStreamEntry, state);
   emberFramingReader_init(&pThis->framing, pRxBuffer, rxBufferSize, onPackageReceived, pThis);
}

void glowReader_readBytes(GlowReader *pThis, const byte *pBytes, int count)
{
   ASSERT(pThis != NULL);

   emberFramingReader_readBytes(&pThis->framing, pBytes, count);
}

void glowReader_free(GlowReader *pThis)
{
   ASSERT(pThis != NULL);

   nonFramingGlowReader_free(&pThis->base);

   bzero(*pThis);
}

void glowReader_reset(GlowReader *pThis)
{
   ASSERT(pThis != NULL);

   nonFramingGlowReader_reset(&pThis->base);
   emberFramingReader_reset(&pThis->framing);
}
