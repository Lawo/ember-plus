/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <string.h>
#include "glowrx.h"
#include "emberinternal.h"


// ====================================================================
//
// Import internals
//
// ====================================================================

bool glow_assertIdentifierValid(pcstr pIdentifier, bool isRx);


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
         pValue->choice.integer = berReader_getLong(pBase);
         break;

      case BerType_Real:
         pValue->flag = GlowParameterType_Real;
         pValue->choice.real = berReader_getReal(pBase);
         break;

      case BerType_UTF8String:
         pValue->flag = GlowParameterType_String;
         pValue->choice.pString = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pValue->choice.pString, pBase->length);
         break;

      case BerType_Boolean:
         pValue->flag = GlowParameterType_Boolean;
         pValue->choice.boolean = berReader_getBoolean(pBase);
         break;

      case BerType_OctetString:
         pValue->flag = GlowParameterType_Octets;
         if(pBase->length > 0)
         {
            pValue->choice.octets.pOctets = newarr(byte, pBase->length);
            pValue->choice.octets.length = berReader_getOctetString(pBase, pValue->choice.octets.pOctets, pBase->length);
         }
         else
         {
            pValue->choice.octets.pOctets = NULL;
            pValue->choice.octets.length = 0;
         }
         break;

      case BerType_Null:
          pValue->flag = GlowParameterType_None;
          pValue->choice.integer = 0;
          berReader_getNull(pBase);
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
         pMinMax->choice.integer = berReader_getLong(pBase);
         break;

      case BerType_Real:
         pMinMax->flag = GlowParameterType_Real;
         pMinMax->choice.real = berReader_getReal(pBase);
         break;

      case BerType_Null:
          pMinMax->flag = GlowParameterType_None;
          pMinMax->choice.integer = 0;
          break;

      default:
         throwError(508, "BerReader reports unsupported GlowMinMax type!");
         break;
   }
}

typedef void (*onItemReady_t)(NonFramingGlowReader *pThis);


static void onItemReady_Template(NonFramingGlowReader *pThis)
{
    const BerReader *pBase = &pThis->base.base;
    berint number;

    if (pBase->isContainer)
    {
        if (pThis->onTemplate != NULL)
            pThis->onTemplate(&pThis->glow.template_, pThis->fields, pThis->glow.template_.state, pThis->path, pThis->pathLength, pThis->state);

        // reset read template
        glowTemplate_free(&pThis->glow.template_);
        pThis->fields = GlowFieldFlag_None;

    }
    else
    {
        pThis->glow.template_.state = true;

        if (berTag_equals(&pBase->tag, &glowTags.template_.number))
        {
            number = berReader_getInteger(pBase);

            pThis->path[pThis->pathLength] = number;
            pThis->pathLength++;
        }
        else if (berTag_equals(&pBase->tag, &glowTags.template_.description))
        {
            pThis->glow.parameter.pDescription = newarr(char, pBase->length + 1);
            pThis->fields |= GlowFieldFlag_Description;
            berReader_getString(pBase, pThis->glow.template_.pDescription, pBase->length + 1);
        }
    }
}

static void onItemReady_QualifiedTemplate(NonFramingGlowReader *pThis)
{
    const BerReader *pBase = &pThis->base.base;

    if (pBase->isContainer)
    {
        if (pThis->onTemplate != NULL)
            pThis->onTemplate(&pThis->glow.template_, pThis->fields, pThis->glow.template_.state, pThis->path, pThis->pathLength, pThis->state);

        // reset read template
        glowTemplate_free(&pThis->glow.template_);
        pThis->fields = GlowFieldFlag_None;
    }
    else
    {
        pThis->glow.template_.state = true;

        if (berTag_equals(&pBase->tag, &glowTags.qualifiedTemplate.path))
        {
            if (pThis->pathLength != 0)
                throwError(504, "QualifiedTemplate encountered on non-root level!");

            pThis->pathLength = berReader_getRelativeOid(pBase, pThis->path, GLOW_MAX_TREE_DEPTH);

            if (pThis->pathLength == 0)
                throwError(503, "empty path in QualifiedTemplate!");
        }
        else if (berTag_equals(&pBase->tag, &glowTags.qualifiedTemplate.description))
        {
            pThis->glow.template_.pDescription = newarr(char, pBase->length + 1);
            pThis->fields |= GlowFieldFlag_Description;
            berReader_getString(pBase, pThis->glow.template_.pDescription, pBase->length + 1);
        }
    }
}

static void onItemReady_Node(NonFramingGlowReader *pThis)
{
   berint number;
   const BerReader *pBase = &pThis->base.base;

   if(pBase->isContainer)
   {
      const EmberAsyncReader* pReader = &pThis->base;
      if(!pReader->pCurrentContainer->hasContent)
      {
         if(pThis->onNode != NULL)
            pThis->onNode(&pThis->glow.node, GlowFieldFlag_None, pThis->path, pThis->pathLength, pThis->state);
      }
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.node.number))
      {
         number = berReader_getInteger(pBase);

         pThis->path[pThis->pathLength] = number;
         pThis->pathLength++;
      }
   }
}

static void onItemReady_QualifiedNode(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;

   if(pBase->isContainer)
   {
      const EmberAsyncReader* pReader = &pThis->base;
      if(!pReader->pCurrentContainer->hasContent)
      {
         if(pThis->onNode != NULL)
            pThis->onNode(&pThis->glow.node, GlowFieldFlag_None, pThis->path, pThis->pathLength, pThis->state);
      }
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

static void onItemReady_NodeContents(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
   int fields = pThis->fields;
   const BerTag *pTag = &pBase->tag;

   if(pBase->isContainer)
   {
      if(pThis->onNode != NULL)
         pThis->onNode(&pThis->glow.node, pThis->fields, pThis->path, pThis->pathLength, pThis->state);

      glowNode_free(&pThis->glow.node);
      pThis->fields = GlowFieldFlag_None;
   }
   else
   {
      if(berTag_equals(pTag, &glowTags.nodeContents.identifier))
      {
         pThis->glow.node.pIdentifier = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->glow.node.pIdentifier, pBase->length + 1);
         glow_assertIdentifierValid(pThis->glow.node.pIdentifier, true);
         fields |= GlowFieldFlag_Identifier;
      }
      else if(berTag_equals(pTag, &glowTags.nodeContents.description))
      {
         pThis->glow.node.pDescription = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->glow.node.pDescription, pBase->length + 1);
         fields |= GlowFieldFlag_Description;
      }
      else if(berTag_equals(pTag, &glowTags.nodeContents.isRoot))
      {
         pThis->glow.node.isRoot = berReader_getBoolean(pBase);
         fields |= GlowFieldFlag_IsRoot;
      }
      else if(berTag_equals(pTag, &glowTags.nodeContents.isOnline))
      {
         pThis->glow.node.isOnline = berReader_getBoolean(pBase);
         fields |= GlowFieldFlag_IsOnline;
      }
      else if(berTag_equals(pTag, &glowTags.nodeContents.schemaIdentifiers))
      {
         pThis->glow.node.pSchemaIdentifiers = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->glow.node.pSchemaIdentifiers, pBase->length + 1);
         fields |= GlowFieldFlag_SchemaIdentifier;
      }
      else if (berTag_equals(pTag, &glowTags.nodeContents.templateReference))
      {
          pThis->glow.node.pTemplateReference = newarr(berint, pBase->length);
          pThis->glow.node.templateReferenceLength = berReader_getRelativeOid(pBase, pThis->glow.node.pTemplateReference, pBase->length);
          fields |= GlowFieldFlag_TemplateReference;
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_NodeContents, pThis->state);
      }

      pThis->fields = (GlowFieldFlags)fields;
   }
}

static void onItemReady_Parameter(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
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

static void onItemReady_QualifiedParameter(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;

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

static void onItemReady_ParameterContents(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
   int fields = pThis->fields;
   const BerTag *pTag = &pBase->tag;

   if(pBase->isContainer)
   {
      if(pThis->onParameter != NULL)
         pThis->onParameter(&pThis->glow.parameter, pThis->fields, pThis->path, pThis->pathLength, pThis->state);

      // reset read parameter
      glowParameter_free(&pThis->glow.parameter);
      pThis->fields = GlowFieldFlag_None;
   }
   else
   {
      if(berTag_equals(pTag, &glowTags.parameterContents.identifier))
      {
         pThis->glow.parameter.pIdentifier = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->glow.parameter.pIdentifier, pBase->length + 1);
         glow_assertIdentifierValid(pThis->glow.parameter.pIdentifier, true);
         fields |= GlowFieldFlag_Identifier;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.description))
      {
         pThis->glow.parameter.pDescription = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->glow.parameter.pDescription, pBase->length + 1);
         fields |= GlowFieldFlag_Description;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.value))
      {
         readValue(pBase, &pThis->glow.parameter.value);
         fields |= GlowFieldFlag_Value;
      }
      else if (berTag_equals(pTag, &glowTags.parameterContents.defaultValue))
      {
          readValue(pBase, &pThis->glow.parameter.defaultValue);
          fields |= GlowFieldFlag_DefaultValue;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.minimum))
      {
         readMinMax(pBase, &pThis->glow.parameter.minimum);
         fields |= GlowFieldFlag_Minimum;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.maximum))
      {
         readMinMax(pBase, &pThis->glow.parameter.maximum);
         fields |= GlowFieldFlag_Maximum;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.access))
      {
         pThis->glow.parameter.access = (GlowAccess)berReader_getInteger(pBase);
         fields |= GlowFieldFlag_Access;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.factor))
      {
         pThis->glow.parameter.factor = berReader_getInteger(pBase);
         fields |= GlowFieldFlag_Factor;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.isOnline))
      {
         pThis->glow.parameter.isOnline = berReader_getBoolean(pBase);
         fields |= GlowFieldFlag_IsOnline;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.step))
      {
         pThis->glow.parameter.step = berReader_getInteger(pBase);
         fields |= GlowFieldFlag_Step;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.type))
      {
         pThis->glow.parameter.type = (GlowParameterType)berReader_getInteger(pBase);
         fields |= GlowFieldFlag_Type;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.streamIdentifier))
      {
         pThis->glow.parameter.streamIdentifier = berReader_getInteger(pBase);
         fields |= GlowFieldFlag_StreamIdentifier;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.schemaIdentifiers))
      {
         pThis->glow.parameter.pSchemaIdentifiers = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->glow.parameter.pSchemaIdentifiers, pBase->length + 1);
         fields |= GlowFieldFlag_SchemaIdentifier;
      }
      else if (berTag_equals(pTag, &glowTags.parameterContents.templateReference))
      {
          pThis->glow.parameter.pTemplateReference = newarr(berint, pBase->length);
          pThis->glow.parameter.templateReferenceLength = berReader_getRelativeOid(pBase, pThis->glow.parameter.pTemplateReference, pBase->length);
          fields |= GlowFieldFlag_TemplateReference;
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_ParameterContents, pThis->state);
      }

      pThis->fields = (GlowFieldFlags)fields;
   }
}

static void onItemReady_StreamDescription(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
   const BerTag *pTag = &pBase->tag;

   if(pBase->isContainer)
   {
      pThis->fields = (GlowFieldFlags)(pThis->fields | GlowFieldFlag_StreamDescriptor);
   }
   else
   {
      if(berTag_equals(pTag, &glowTags.streamDescription.format))
      {
         pThis->glow.parameter.streamDescriptor.format = (GlowStreamFormat)berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.streamDescription.offset))
      {
         pThis->glow.parameter.streamDescriptor.offset = berReader_getInteger(pBase);
      }
   }
}

static void onItemReady_Command(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;

   if(pBase->isContainer)
   {
      if(pThis->glow.command.number == GlowCommandType_GetDirectory
      && pThis->glow.command.options.dirFieldMask == 0)
         pThis->glow.command.options.dirFieldMask = GlowFieldFlag_All;

      if(pThis->onCommand != NULL)
         pThis->onCommand(&pThis->glow.command, pThis->path, pThis->pathLength, pThis->state);

      glowCommand_free(&pThis->glow.command);
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.command.number))
      {
         pThis->glow.command.number = berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.command.dirFieldMask))
      {
         pThis->glow.command.options.dirFieldMask = (GlowFieldFlags)berReader_getInteger(pBase);
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_Command, pThis->state);
      }
   }
}

static void onItemReady_StreamEntry(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;

   if(pBase->isContainer)
   {
      if(pThis->onStreamEntry != NULL)
         pThis->onStreamEntry(&pThis->glow.streamEntry, pThis->state);

      glowValue_free(&pThis->glow.streamEntry.streamValue);
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.streamEntry.streamIdentifier))
      {
         pThis->glow.streamEntry.streamIdentifier = berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.streamEntry.streamValue))
      {
         readValue(pBase, &pThis->glow.streamEntry.streamValue);
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_StreamEntry, pThis->state);
      }
   }
}

static void onItemReady_Matrix(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
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

static void onItemReady_QualifiedMatrix(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;

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

static void onItemReady_MatrixContents(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
   const BerTag *pTag = &pBase->tag;
   GlowParametersLocation *pParams;

   if(pBase->isContainer)
   {
      if(pThis->onMatrix != NULL)
         pThis->onMatrix(&pThis->glow.matrix, pThis->path, pThis->pathLength, pThis->state);

      // reset read matrix
      glowMatrix_free(&pThis->glow.matrix);
   }
   else
   {
      if(berTag_equals(pTag, &glowTags.matrixContents.identifier))
      {
         pThis->glow.matrix.pIdentifier = newarr(char, pBase->length + 1);
         pThis->fields |= GlowFieldFlag_Identifier;
         berReader_getString(pBase, pThis->glow.matrix.pIdentifier, pBase->length + 1);
         glow_assertIdentifierValid(pThis->glow.matrix.pIdentifier, true);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.description))
      {
         pThis->glow.matrix.pDescription = newarr(char, pBase->length + 1);
         pThis->fields |= GlowFieldFlag_Description;
         berReader_getString(pBase, pThis->glow.matrix.pDescription, pBase->length + 1);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.type))
      {
          pThis->glow.matrix.type = (GlowMatrixType)berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.addressingMode))
      {
         pThis->glow.matrix.addressingMode = (GlowMatrixAddressingMode)berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.targetCount))
      {
         pThis->glow.matrix.targetCount = berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.sourceCount))
      {
         pThis->glow.matrix.sourceCount = berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.maximumTotalConnects))
      {
         pThis->glow.matrix.maximumTotalConnects = berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.maximumConnectsPerTarget))
      {
         pThis->glow.matrix.maximumConnectsPerTarget = berReader_getInteger(pBase);
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.parametersLocation))
      {
         pParams = &pThis->glow.matrix.parametersLocation;

         if(pBase->type == BerType_RelativeOid)
         {
            pParams->choice.basePath.length = berReader_getRelativeOid(pBase, pParams->choice.basePath.ids, GLOW_MAX_TREE_DEPTH);
            pParams->kind = GlowParametersLocationKind_BasePath;
         }
         else if(pBase->type == BerType_Integer)
         {
            pParams->choice.inlineId = berReader_getInteger(pBase);
            pParams->kind = GlowParametersLocationKind_Inline;
         }
      }
      else if(berTag_equals(pTag, &glowTags.matrixContents.schemaIdentifiers))
      {
         pThis->glow.matrix.pSchemaIdentifiers = newarr(char, pBase->length + 1);
         pThis->fields |= GlowFieldFlag_SchemaIdentifier;
         berReader_getString(pBase, pThis->glow.matrix.pSchemaIdentifiers, pBase->length + 1);
      }
      else if (berTag_equals(pTag, &glowTags.matrixContents.templateReference))
      {
          pThis->glow.matrix.pTemplateReference = newarr(berint, pBase->length);
          pThis->fields |= GlowFieldFlag_TemplateReference;
          pThis->glow.matrix.templateReferenceLength = berReader_getRelativeOid(pBase, pThis->glow.matrix.pTemplateReference, pBase->length);
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
            pThis->onTarget(&pThis->glow.signal, pThis->path, pThis->pathLength, pThis->state);
      }
      else
      {
         if(pThis->onSource != NULL)
            pThis->onSource(&pThis->glow.signal, pThis->path, pThis->pathLength, pThis->state);
      }

      // reset read signal
      bzero_item(pThis->glow.signal);
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.signal.number))
      {
         pThis->glow.signal.number = berReader_getInteger(pBase);
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_Target, pThis->state);
      }
   }
}

static void onItemReady_Target(NonFramingGlowReader *pThis)
{
   onItemReady_Signal(pThis, &pThis->base.base, true);
}

static void onItemReady_Source(NonFramingGlowReader *pThis)
{
   onItemReady_Signal(pThis, &pThis->base.base, false);
}

static void onItemReady_Connection(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;

   if(pBase->isContainer)
   {
      if(pThis->onConnection != NULL)
         pThis->onConnection(&pThis->glow.connection, pThis->path, pThis->pathLength, pThis->state);

      // reset read connection
      glowConnection_free(&pThis->glow.connection);
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.connection.target))
      {
         pThis->glow.connection.target = berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.connection.sources))
      {
         if(pBase->length > 0)
         {
            pThis->glow.connection.pSources = newarr(berint, pBase->length);
            pThis->glow.connection.sourcesLength = berReader_getRelativeOid(pBase, pThis->glow.connection.pSources, pBase->length);
         }
         else
         {
            pThis->glow.connection.pSources = NULL;
            pThis->glow.connection.sourcesLength = 0;
         }
      }
      else if(berTag_equals(&pBase->tag, &glowTags.connection.operation))
      {
         pThis->glow.connection.operation = (GlowConnectionOperation)berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.connection.disposition))
      {
         pThis->glow.connection.disposition = (GlowConnectionDisposition)berReader_getInteger(pBase);
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_Target, pThis->state);
      }
   }
}

static void onItemReady_Function(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
   berint number;

   if(pBase->isContainer == false)
   {
      if(berTag_equals(&pBase->tag, &glowTags.function.number))
      {
         number = berReader_getInteger(pBase);

         pThis->path[pThis->pathLength] = number;
         pThis->pathLength++;
      }
   }
}

static void onItemReady_QualifiedFunction(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;

   if(pBase->isContainer)
   {
      pThis->pathLength = 0;
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.qualifiedFunction.path))
      {
         if(pThis->pathLength != 0)
            throwError(509, "QualifiedFunction encountered on non-root level!");

         pThis->pathLength = berReader_getRelativeOid(pBase, pThis->path, GLOW_MAX_TREE_DEPTH);

         if(pThis->pathLength == 0)
            throwError(510, "empty path in QualifiedFunction!");
      }
   }
}

static void onItemReady_FunctionContents(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
   const BerTag *pTag = &pBase->tag;

   if(pBase->isContainer)
   {
      if(pThis->onFunction != NULL)
         pThis->onFunction(&pThis->glow.function, pThis->path, pThis->pathLength, pThis->state);

      // reset read function
      glowFunction_free(&pThis->glow.function);
   }
   else
   {
      if(berTag_equals(pTag, &glowTags.functionContents.identifier))
      {
         pThis->glow.function.pIdentifier = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->glow.function.pIdentifier, pBase->length + 1);
         glow_assertIdentifierValid(pThis->glow.function.pIdentifier, true);
      }
      else if(berTag_equals(pTag, &glowTags.functionContents.description))
      {
         pThis->glow.function.pDescription = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pThis->glow.function.pDescription, pBase->length + 1);
      }
      else if (berTag_equals(pTag, &glowTags.functionContents.templateReference))
      {
          pThis->glow.function.pTemplateReference = newarr(berint, pBase->length);
          pThis->fields |= GlowFieldFlag_TemplateReference;
          pThis->glow.function.templateReferenceLength = berReader_getRelativeOid(pBase, pThis->glow.function.pTemplateReference, pBase->length);
      }
   }
}

static void onItemReady_FunctionArgument(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
   GlowFunction *pFunction = &pThis->glow.function;
   GlowTupleItemDescription *pTupleItem = &pFunction->pArguments[pFunction->argumentsLength];

   if(pBase->isContainer)
   {
      pFunction->argumentsLength++;
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.tupleItemDescription.type))
      {
         pTupleItem->type = (GlowParameterType)berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.tupleItemDescription.name))
      {
         pTupleItem->pName = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pTupleItem->pName, pBase->length + 1);
      }
   }
}

static void onItemReady_FunctionResult(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
   GlowFunction *pFunction = &pThis->glow.function;
   GlowTupleItemDescription *pTupleItem = &pFunction->pResult[pFunction->resultLength];

   if(pBase->isContainer)
   {
      pFunction->resultLength++;
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.tupleItemDescription.type))
      {
         pTupleItem->type = (GlowParameterType)berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.tupleItemDescription.name))
      {
         pTupleItem->pName = newarr(char, pBase->length + 1);
         berReader_getString(pBase, pTupleItem->pName, pBase->length + 1);
      }
   }
}

static void onItemReady_Invocation(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;

   if(pBase->isContainer == false)
   {
      if(berTag_equals(&pBase->tag, &glowTags.invocation.invocationId))
         pThis->glow.command.options.invocation.invocationId = berReader_getInteger(pBase);
   }
}

static void onItemReady_InvocationResult(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;

   if(pBase->isContainer)
   {
      if(pThis->onInvocationResult)
         pThis->onInvocationResult(&pThis->glow.invocationResult, pThis->state);

      // reset read invocation result
      glowInvocationResult_free(&pThis->glow.invocationResult);
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.invocationResult.invocationId))
      {
         pThis->glow.invocationResult.invocationId = berReader_getInteger(pBase);
      }
      else if(berTag_equals(&pBase->tag, &glowTags.invocationResult.success))
      {
         pThis->glow.invocationResult.hasError = berReader_getBoolean(pBase) == false;
      }
   }
}

static void allocValues(GlowValue **ppValues, int length)
{
   GlowValue *pNew;

   ASSERT(ppValues != NULL);

   if(*ppValues == NULL)
   {
      ASSERT(length == 0);
      *ppValues = newarr(GlowValue, 4);
      memset(*ppValues, 0, sizeof(GlowValue) * 4);
   }
   else
   {
      if(length >= 4)
      {
         pNew = newarr(GlowValue, length + 1);
         memset(pNew + length, 0, sizeof(GlowValue));
         memcpy(pNew, *ppValues, sizeof(GlowValue) * length);
         freeMemory(*ppValues);
         *ppValues = pNew;
      }
   }
}

static void onItemReady_InvocationArguments(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
   GlowInvocation *pInvocation = &pThis->glow.command.options.invocation;

   if(pBase->isContainer == false)
   {
      allocValues(&pInvocation->pArguments, pInvocation->argumentsLength);
      readValue(pBase, &pInvocation->pArguments[pInvocation->argumentsLength]);
      pInvocation->argumentsLength++;
   }
}

static void onItemReady_InvocationResultResult(NonFramingGlowReader *pThis)
{
   const BerReader *pBase = &pThis->base.base;
   GlowInvocationResult *pInvocationResult = &pThis->glow.invocationResult;

   if(pBase->isContainer == false)
   {
      allocValues(&pInvocationResult->pResult, pInvocationResult->resultLength);
      readValue(pBase, &pInvocationResult->pResult[pInvocationResult->resultLength]);
      pInvocationResult->resultLength++;
   }
}

static void allocTupleDescriptions(GlowTupleItemDescription **ppTupleItems, int length)
{
   GlowTupleItemDescription *pNew;

   ASSERT(ppTupleItems != NULL);

   if(*ppTupleItems == NULL)
   {
      ASSERT(length == 0);
      *ppTupleItems = newarr(GlowTupleItemDescription, 4);
      memset(*ppTupleItems, 0, sizeof(GlowTupleItemDescription) * 4);
   }
   else
   {
      if(length >= 4)
      {
         pNew = newarr(GlowTupleItemDescription, length + 1);
         memset(pNew + length, 0, sizeof(GlowTupleItemDescription));
         memcpy(pNew, *ppTupleItems, length * sizeof(GlowTupleItemDescription));
         freeMemory(*ppTupleItems);
         *ppTupleItems = pNew;
      }
   }
}

static onItemReady_t getOnItemReady_EnterContainer(const BerReader *pBase)
{
   EmberAsyncReader *pAsync = (EmberAsyncReader *)pBase;
   NonFramingGlowReader *pThis = (NonFramingGlowReader *)pBase;
   __EmberAsyncContainerStack *pStack;
   __EmberAsyncContainer *pParent;

   switch(pBase->type)
   {
      case GlowType_Template:
         bzero_item(pThis->glow.template_);
         return onItemReady_Template;
      case GlowType_QualifiedTemplate:
         bzero_item(pThis->glow.template_);
         return onItemReady_QualifiedTemplate;
      case GlowType_Node:
         bzero_item(pThis->glow.node);
         pThis->glow.node.isOnline = true;
         return onItemReady_Node;
      case GlowType_QualifiedNode:
         bzero_item(pThis->glow.node);
         pThis->glow.node.isOnline = true;
         return onItemReady_QualifiedNode;
      case GlowType_Parameter:
         bzero_item(pThis->glow.parameter);
         return onItemReady_Parameter;
      case GlowType_QualifiedParameter:
         bzero_item(pThis->glow.parameter);
         return onItemReady_QualifiedParameter;
      case GlowType_Command:
         bzero_item(pThis->glow.command);
         return onItemReady_Command;
      case GlowType_StreamEntry:
         bzero_item(pThis->glow.streamEntry);
         return onItemReady_StreamEntry;
      case GlowType_StreamDescription:
         bzero_item(pThis->glow.parameter.streamDescriptor);
         return onItemReady_StreamDescription;
      case GlowType_Matrix:
         bzero_item(pThis->glow.matrix);
         return onItemReady_Matrix;
      case GlowType_QualifiedMatrix:
         bzero_item(pThis->glow.matrix);
         return onItemReady_QualifiedMatrix;
      case GlowType_Target:
         bzero_item(pThis->glow.signal);
         return onItemReady_Target;
      case GlowType_Source:
         bzero_item(pThis->glow.signal);
         return onItemReady_Source;
      case GlowType_Connection:
         bzero_item(pThis->glow.connection);
         return onItemReady_Connection;
      case GlowType_Function:
         bzero_item(pThis->glow.function);
         return onItemReady_Function;
      case GlowType_QualifiedFunction:
         bzero_item(pThis->glow.function);
         return onItemReady_QualifiedFunction;
      case GlowType_Invocation:
         bzero_item(pThis->glow.command.options.invocation);
         return onItemReady_Invocation;
      case GlowType_InvocationResult:
         bzero_item(pThis->glow.invocationResult);
         return onItemReady_InvocationResult;
      default:
         pStack = pAsync->pContainerStack;

         if(pStack->length > 0)
         {
            pParent = &pStack->items[pStack->length - 1];

            if(pBase->type == BerType_Set)
            {
               if(berTag_equals(&pBase->tag, &glowTags.parameter.contents)
               && (pParent->type == GlowType_Parameter || pParent->type == GlowType_QualifiedParameter))
                  return onItemReady_ParameterContents;

               if(berTag_equals(&pBase->tag, &glowTags.node.contents)
               && (pParent->type == GlowType_Node || pParent->type == GlowType_QualifiedNode))
                  return onItemReady_NodeContents;

               if(berTag_equals(&pBase->tag, &glowTags.matrix.contents)
               && (pParent->type == GlowType_Matrix || pParent->type == GlowType_QualifiedMatrix))
                  return onItemReady_MatrixContents;

               if(berTag_equals(&pBase->tag, &glowTags.function.contents)
               && (pParent->type == GlowType_Function || pParent->type == GlowType_QualifiedFunction))
                  return onItemReady_FunctionContents;
            }

            if(pBase->type == BerType_Sequence)
            {
               if(berTag_equals(&pBase->tag, &glowTags.invocation.arguments)
               && pParent->type == GlowType_Invocation)
                  return onItemReady_InvocationArguments;

               if(berTag_equals(&pBase->tag, &glowTags.invocationResult.result)
               && pParent->type == GlowType_InvocationResult)
                  return onItemReady_InvocationResultResult;
            }

            if(pBase->type == GlowType_TupleItemDescription)
            {
               if(berTag_equals(&pParent->tag, &glowTags.functionContents.arguments)
               && pParent->type == BerType_Sequence)
               {
                  allocTupleDescriptions(&pThis->glow.function.pArguments, pThis->glow.function.argumentsLength);
                  return onItemReady_FunctionArgument;
               }

               if(berTag_equals(&pParent->tag, &glowTags.functionContents.result)
               && pParent->type == BerType_Sequence)
               {
                  allocTupleDescriptions(&pThis->glow.function.pResult, pThis->glow.function.resultLength);
                  return onItemReady_FunctionResult;
               }
            }
         }
         break;
   }

   return NULL;
}

static onItemReady_t getOnItemReady_LeaveContainer(const BerReader *pBase)
{
   EmberAsyncReader *pAsync = (EmberAsyncReader *)pBase;
   __EmberAsyncContainerStack *pStack = pAsync->pContainerStack;
   __EmberAsyncContainer *pCurrent;
   __EmberAsyncContainer *pParent;

   if(pStack->length > 0)
   {
      pCurrent = &pStack->items[pStack->length - 1];

      switch(pCurrent->type)
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
         case GlowType_Function: return onItemReady_Function;
         case GlowType_QualifiedFunction: return onItemReady_QualifiedFunction;
         case GlowType_Invocation: return onItemReady_Invocation;
         case GlowType_InvocationResult: return onItemReady_InvocationResult;
         case GlowType_Template: return onItemReady_Template;
         case GlowType_QualifiedTemplate: return onItemReady_QualifiedTemplate;
         default:
            if(pStack->length > 1)
            {
               pParent = &pStack->items[pStack->length - 2];

               if(pCurrent->type == BerType_Set)
               {
                  if(berTag_equals(&pCurrent->tag, &glowTags.parameter.contents)
                  && (pParent->type == GlowType_Parameter || pParent->type == GlowType_QualifiedParameter))
                     return onItemReady_ParameterContents;

                  if(berTag_equals(&pCurrent->tag, &glowTags.node.contents)
                  && (pParent->type == GlowType_Node || pParent->type == GlowType_QualifiedNode))
                     return onItemReady_NodeContents;

                  if(berTag_equals(&pCurrent->tag, &glowTags.matrix.contents)
                  && (pParent->type == GlowType_Matrix || pParent->type == GlowType_QualifiedMatrix))
                     return onItemReady_MatrixContents;

                  if(berTag_equals(&pCurrent->tag, &glowTags.function.contents)
                  && (pParent->type == GlowType_Function || pParent->type == GlowType_QualifiedFunction))
                     return onItemReady_FunctionContents;
               }

               if(pCurrent->type == BerType_Sequence)
               {
                  if(berTag_equals(&pCurrent->tag, &glowTags.invocation.arguments)
                  && pParent->type == GlowType_Invocation)
                     return onItemReady_InvocationArguments;

                  if(berTag_equals(&pCurrent->tag, &glowTags.invocationResult.result)
                  && pParent->type == GlowType_InvocationResult)
                     return onItemReady_InvocationResultResult;
               }

               if(pCurrent->type == GlowType_TupleItemDescription)
               {
                  if(berTag_equals(&pParent->tag, &glowTags.functionContents.arguments)
                  && pParent->type == BerType_Sequence)
                     return onItemReady_FunctionArgument;

                  if(berTag_equals(&pParent->tag, &glowTags.functionContents.result)
                  && pParent->type == BerType_Sequence)
                     return onItemReady_FunctionResult;
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
      pThis->onItemReadyState(pThis);

   if(pBase->isContainer)
   {
      if(pBase->type == GlowType_Node
      || pBase->type == GlowType_Parameter
      || pBase->type == GlowType_Matrix
      || pBase->type == GlowType_Function
      || pBase->type == GlowType_Template)
         pThis->pathLength--;

      pThis->onItemReadyState = getOnItemReady_LeaveContainer(pBase);
   }
}

static void onNewContainer(const BerReader *pBase)
{
   NonFramingGlowReader *pThis = (NonFramingGlowReader *)pBase;
   bool predecessorIsTemplate =
       pThis->onItemReadyState == onItemReady_QualifiedTemplate ||
       pThis->onItemReadyState == onItemReady_Template;

   if(berTag_equals(&pBase->tag, &glowTags.root))
      pThis->pathLength = 0;

   if (predecessorIsTemplate)
       pThis->onItemReadyState(pThis);

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
    nonFramingGlowReader_initEx(pThis, onNode, onParameter, onCommand, onStreamEntry, NULL, NULL, state);
}

void nonFramingGlowReader_initEx(NonFramingGlowReader *pThis,
    onNode_t onNode,
    onParameter_t onParameter,
    onCommand_t onCommand,
    onStreamEntry_t onStreamEntry,
    onFunction_t onFunction,
    onTemplate_t onTemplate,
    voidptr state)
{
    ASSERT(pThis != NULL);

    bzero_item(*pThis);

    emberAsyncReader_init(&pThis->base);

    pThis->base.onNewContainer = onNewContainer;
    pThis->base.onItemReady = onItemReady;

    pThis->onNode = onNode;
    pThis->onParameter = onParameter;
    pThis->onCommand = onCommand;
    pThis->onStreamEntry = onStreamEntry;
    pThis->onTemplate = onTemplate;
    pThis->onFunction = onFunction;

    pThis->state = state;
}

void nonFramingGlowReader_free(NonFramingGlowReader *pThis)
{
   ASSERT(pThis != NULL);

   emberAsyncReader_free(&pThis->base);

   bzero_item(*pThis);
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

   if(length >= 7
   && pPackage[1] == EMBER_MESSAGE_ID
   && pPackage[2] == EMBER_COMMAND_PAYLOAD
   && pPackage[5] == EMBER_DTD_GLOW)
   {
      appBytesCount = pPackage[6];
      headerLength = 7 + appBytesCount;

      if(pPackage[4] & EmberFramingFlag_FirstPackage)
      {
         emberAsyncReader_reset(&pThis->base.base);

         if(pThis->onFirstPackageReceived != NULL)
            pThis->onFirstPackageReceived(pPackage, length, pThis->base.state);
      }

      if(pThis->onPackageReceived != NULL)
         pThis->onPackageReceived(pPackage, length, pThis->base.state);

      emberAsyncReader_readBytes(&pThis->base.base, pPackage + headerLength, length - headerLength);

      if(pPackage[4] & EmberFramingFlag_LastPackage)
      {
         glowReader_reset(pThis);

         if(pThis->onLastPackageReceived != NULL)
            pThis->onLastPackageReceived(pPackage, length, pThis->base.state);
      }
   }
   else
   {
       if(pThis->onOtherPackageReceived != NULL)
          pThis->onOtherPackageReceived(pPackage, length, pThis->base.state);
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
    glowReader_initEx(pThis, onNode, onParameter, onCommand, onStreamEntry, NULL, NULL, state, pRxBuffer, rxBufferSize);
}

void glowReader_initEx(GlowReader *pThis,
    onNode_t onNode,
    onParameter_t onParameter,
    onCommand_t onCommand,
    onStreamEntry_t onStreamEntry,
    onFunction_t onFunction,
    onTemplate_t onTemplate,
    voidptr state,
    byte *pRxBuffer,
    unsigned int rxBufferSize)
{
    ASSERT(pThis != NULL);
    ASSERT(pRxBuffer != NULL);
    ASSERT(rxBufferSize > 0);

    pThis->onOtherPackageReceived = NULL;
    pThis->onFirstPackageReceived = NULL;
    pThis->onLastPackageReceived = NULL;
    pThis->onPackageReceived = NULL;
    nonFramingGlowReader_initEx(&pThis->base, onNode, onParameter, onCommand, onStreamEntry, onFunction, onTemplate, state);
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

   bzero_item(*pThis);
}

void glowReader_reset(GlowReader *pThis)
{
   ASSERT(pThis != NULL);

   nonFramingGlowReader_reset(&pThis->base);
   emberFramingReader_reset(&pThis->framing);
}
