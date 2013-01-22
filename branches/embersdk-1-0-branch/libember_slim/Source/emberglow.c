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

   // root
   {BerClass_Application, 0},
};


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
         ember_writeString(pOut, pTag, pValue->string);
         break;

      case GlowParameterType_Boolean:
         ember_writeBoolean(pOut, pTag, pValue->boolean);
         break;

      case GlowParameterType_Octets:
         ember_writeOctetString(pOut, pTag, pValue->octets.octets, pValue->octets.length);
         break;

      default:
         throwError(506, "GlowValue object has unsupported type!");
         break;
   }
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
   ASSERT(pNode->identifier[0] != 0);

   ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedNode);
   ember_writeRelativeOid(pOut, &glowTags.qualifiedNode.path, pPath, pathLength);

   ember_writeSetBegin(pOut, &glowTags.qualifiedNode.contents);
   if(fields & GlowFieldFlag_Identifier)
      ember_writeString(pOut, &glowTags.nodeContents.identifier, pNode->identifier);
   if(fields & GlowFieldFlag_Description)
      ember_writeString(pOut, &glowTags.nodeContents.description, pNode->description);

   if(pNode->isRoot)
      ember_writeBoolean(pOut, &glowTags.nodeContents.isRoot, pNode->isRoot);

   ember_writeContainerEnd(pOut); // end node.contents

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
   if(fields & GlowFieldFlag_Identifier)
      ember_writeString(pOut, &glowTags.parameterContents.identifier, pParameter->identifier);

   if(fields & GlowFieldFlag_Description)
      ember_writeString(pOut, &glowTags.parameterContents.description, pParameter->description);

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
                                    bool isNestedInParameter)
{
   if(pathLength > 0)
   {
      if(isNestedInParameter)
      {
         ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedParameter);
         ember_writeRelativeOid(pOut, &glowTags.qualifiedParameter.path, pPath, pathLength);
         ember_writeContainerBegin(pOut, &glowTags.qualifiedParameter.children, GlowType_ElementCollection);
      }
      else
      {
         ember_writeContainerBegin(pOut, &glowTags.elementCollection.element, GlowType_QualifiedNode);
         ember_writeRelativeOid(pOut, &glowTags.qualifiedNode.path, pPath, pathLength);
         ember_writeContainerBegin(pOut, &glowTags.qualifiedParameter.children, GlowType_ElementCollection);
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

   glow_writeQualifiedParameterImpl(&pOut->base.base.base, pParameter, fields, pPath, pathLength);
}

void glow_writeQualifiedCommand(GlowOutput *pOut,
                                const GlowCommand *pCommand,
                                const berint *pPath,
                                int pathLength,
                                bool isNestedInParameter)
{
   ASSERT(pOut != NULL);
   ASSERT(pCommand != NULL);
   ASSERT(pPath != NULL || pathLength == 0);

   glow_writeQualifiedCommandImpl(&pOut->base.base.base, pCommand, pPath, pathLength, isNestedInParameter);
}

void glow_writeStreamEntry(GlowOutput *pOut, const GlowStreamEntry *pEntry)
{
   ASSERT(pOut != NULL);
   ASSERT(pEntry != NULL);

   glow_writeStreamEntryImpl(&pOut->base.base.base, pEntry);
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
         berReader_getString(pBase, pValue->string, GLOW_MAX_VALUE_LENGTH);
         break;

      case BerType_Boolean:
         pValue->flag = GlowParameterType_Boolean;
         pValue->boolean = berReader_getBoolean(pBase);
         break;

      case BerType_OctetString:
         pValue->flag = GlowParameterType_Octets;
         pValue->octets.length = berReader_getOctetString(pBase, pValue->octets.octets, GLOW_MAX_VALUE_LENGTH);
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

static void onItemReady_Node(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase);
static void onItemReady_QualifiedNode(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase);
static void onItemReady_Parameter(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase);
static void onItemReady_QualifiedParameter(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase);
static void onItemReady_ParameterContents(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase);
static void onItemReady_Command(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase);
static void onItemReady_StreamDescription(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase);

static void onItemReady_Node(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   berint number;

   if(pBase->isContainer)
   {
      if(pThis->onNode != NULL)
         pThis->onNode(&pThis->node, pThis->path, pThis->pathLength, pThis->state);

      bzero(pThis->node);
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

static void onItemReady_QualifiedNode(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   if(pBase->isContainer)
   {
      if(pThis->onNode != NULL)
         pThis->onNode(&pThis->node, pThis->path, pThis->pathLength, pThis->state);

      bzero(pThis->node);
      pThis->pathLength = 0;
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.qualifiedNode.path))
      {
         if(pThis->pathLength != 0)
            throwError(502, "QualifiedNode encountered on non-root level!");

         pThis->pathLength = berReader_getRelativeOid(pBase, pThis->path, MAX_GLOW_TREE_DEPTH);

         if(pThis->pathLength == 0)
            throwError(501, "empty path in QualifiedNode!");
      }
   }
}

static void onItemReady_NodeContents(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   const BerTag *pTag = &pBase->tag;

   if(pBase->isContainer == false)
   {
      if(berTag_equals(pTag, &glowTags.nodeContents.identifier))
      {
         berReader_getString(pBase, pThis->node.identifier, GLOW_MAX_IDENTIFIER_LENGTH);
      }
      else if(berTag_equals(pTag, &glowTags.nodeContents.description))
      {
         berReader_getString(pBase, pThis->node.description, GLOW_MAX_DESCRIPTION_LENGTH);
      }
      else if(berTag_equals(pTag, &glowTags.nodeContents.isRoot))
      {
         pThis->node.isRoot = berReader_getBoolean(pBase);
      }
      else
      {
         if(pThis->onUnsupportedTltlv != NULL)
            pThis->onUnsupportedTltlv(pBase, pThis->path, pThis->pathLength, GlowReaderPosition_NodeContents, pThis->state);
      }
   }
}

static void onItemReady_Parameter(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   berint number;

   if(pBase->isContainer)
   {
      if(pThis->onParameter != NULL)
         pThis->onParameter(&pThis->parameter, pThis->paramFields, pThis->path, pThis->pathLength, pThis->state);

      // reset read parameter
      bzero(pThis->parameter);
      pThis->paramFields = GlowFieldFlag_None;
   }
   else
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
      if(pThis->onParameter != NULL)
         pThis->onParameter(&pThis->parameter, pThis->paramFields, pThis->path, pThis->pathLength, pThis->state);

      // reset read parameter
      bzero(pThis->parameter);
      pThis->paramFields = GlowFieldFlag_None;
      pThis->pathLength = 0;
   }
   else
   {
      if(berTag_equals(&pBase->tag, &glowTags.qualifiedParameter.path))
      {
         if(pThis->pathLength != 0)
            throwError(504, "QualifiedParameter encountered on non-root level!");

         pThis->pathLength = berReader_getRelativeOid(pBase, pThis->path, MAX_GLOW_TREE_DEPTH);

         if(pThis->pathLength == 0)
            throwError(503, "empty path in QualifiedParameter!");
      }
   }
}

static void onItemReady_ParameterContents(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   int fields = pThis->paramFields;
   const BerTag *pTag = &pBase->tag;

   if(pBase->isContainer == false)
   {
      if(berTag_equals(pTag, &glowTags.parameterContents.identifier))
      {
         berReader_getString(pBase, pThis->parameter.identifier, GLOW_MAX_IDENTIFIER_LENGTH);
         fields |= GlowFieldFlag_Identifier;
      }
      else if(berTag_equals(pTag, &glowTags.parameterContents.description))
      {
         berReader_getString(pBase, pThis->parameter.description, GLOW_MAX_DESCRIPTION_LENGTH);
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

      pThis->paramFields = (GlowFieldFlags)fields;
   }
}

static void onItemReady_StreamDescription(NonFramingGlowReader *pThis, const EmberAsyncReader *pAsync, const BerReader *pBase)
{
   const BerTag *pTag = &pBase->tag;

   if(pBase->isContainer)
   {
      pThis->paramFields = (GlowFieldFlags)(pThis->paramFields | GlowFieldFlag_StreamDescriptor);
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
         pThis->onStreamEntry(&pThis->streamEntry, pThis->path, pThis->pathLength, pThis->state);
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

onItemReady_t getOnItemReady_EnterContainer(const BerReader *pBase)
{
   EmberAsyncReader *pAsync = (EmberAsyncReader *)pBase;
   __EmberAsyncContainerStack *pStack;
   __EmberAsyncContainer *pContainer;

   if(pBase->type == GlowType_Node)
   {
      return onItemReady_Node;
   }
   else if(pBase->type == GlowType_Parameter)
   {
      return onItemReady_Parameter;
   }
   else if(pBase->type == GlowType_Command)
   {
      return onItemReady_Command;
   }
   else if(pBase->type == GlowType_StreamEntry)
   {
      return onItemReady_StreamEntry;
   }
   else if(pBase->type == GlowType_QualifiedNode)
   {
      return onItemReady_QualifiedNode;
   }
   else if(pBase->type == GlowType_QualifiedParameter)
   {
      return onItemReady_QualifiedParameter;
   }
   else if(pBase->type == GlowType_StreamDescription)
   {
      return onItemReady_StreamDescription;
   }
   else
   {
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
         }
      }
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

      if(pContainer->type == GlowType_Node)
      {
         return onItemReady_Node;
      }
      else if(pContainer->type == GlowType_Parameter)
      {
         return onItemReady_Parameter;
      }
      else if(pContainer->type == GlowType_Command)
      {
         return onItemReady_Command;
      }
      else if(pContainer->type == GlowType_StreamEntry)
      {
         return onItemReady_StreamEntry;
      }
      else if(pContainer->type == GlowType_QualifiedNode)
      {
         return onItemReady_QualifiedNode;
      }
      else if(pContainer->type == GlowType_QualifiedParameter)
      {
         return onItemReady_QualifiedParameter;
      }
      else if(pContainer->type == GlowType_StreamDescription)
      {
         return onItemReady_StreamDescription;
      }
      else
      {
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
            }
         }
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
      || pBase->type == GlowType_Parameter)
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
                               byte *pBuffer,
                               int bufferSize,
                               onNode_t onNode,
                               onParameter_t onParameter,
                               onCommand_t onCommand,
                               onStreamEntry_t onStreamEntry,
                               voidptr state)
{
   ASSERT(pThis != NULL);
   ASSERT(pBuffer != NULL);
   ASSERT(bufferSize > 0);

   bzero(*pThis);

   emberAsyncReader_init(&pThis->base, pBuffer, bufferSize);

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
                     byte *pValueBuffer,
                     int valueBufferSize,
                     onNode_t onNode,
                     onParameter_t onParameter,
                     onCommand_t onCommand,
                     onStreamEntry_t onStreamEntry,
                     voidptr state,
                     byte *pRxBuffer,
                     unsigned int rxBufferSize)
{
   ASSERT(pThis != NULL);
   ASSERT(pValueBuffer != NULL);
   ASSERT(valueBufferSize > 0);
   ASSERT(pRxBuffer != NULL);
   ASSERT(rxBufferSize > 0);

   pThis->onPackageReceived = NULL;
   nonFramingGlowReader_init(&pThis->base, pValueBuffer, valueBufferSize, onNode, onParameter, onCommand, onStreamEntry, state);
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
