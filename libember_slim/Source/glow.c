/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <string.h>
#include "glow.h"
#include "emberinternal.h"

const __GlowTags glowTags =
{
   // template
   {
      { BerClass_ContextSpecific, 0 }, // number
      { BerClass_ContextSpecific, 1 }, // element
      { BerClass_ContextSpecific, 2 }, // description
   },

   // qualifiedTemplate
   {
      { BerClass_ContextSpecific, 0 }, // path
      { BerClass_ContextSpecific, 1 }, // element
      { BerClass_ContextSpecific, 2 }, // description
   },

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
      {BerClass_ContextSpecific, 17},  // contents.schemaIdentifiers
      {BerClass_ContextSpecific, 18},  // contents.templateReference
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
      {BerClass_ContextSpecific, 4},   // contents.schemaIdentifiers
      {BerClass_ContextSpecific, 5},   // contents.templateReference
   },

   // command
   {
      {BerClass_ContextSpecific, 0},   // number
      {BerClass_ContextSpecific, 1},   // dirFieldMask
      {BerClass_ContextSpecific, 2},   // invocation
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
      {BerClass_ContextSpecific, 9},   // gainParameterNumber
      {BerClass_ContextSpecific, 10},  // labels
      {BerClass_ContextSpecific, 11},  // schemaIdentifiers
      {BerClass_ContextSpecific, 12},  // templateReference
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


   // function
   {
      {BerClass_ContextSpecific, 0},   // number
      {BerClass_ContextSpecific, 1},   // contents
      {BerClass_ContextSpecific, 2},   // children
   },

   // qualifiedFunction
   {
      {BerClass_ContextSpecific, 0},   // path
      {BerClass_ContextSpecific, 1},   // contents
      {BerClass_ContextSpecific, 2},   // children
   },

   // functionContents
   {
      {BerClass_ContextSpecific, 0},   // identifier
      {BerClass_ContextSpecific, 1},   // description
      {BerClass_ContextSpecific, 2},   // arguments
      {BerClass_ContextSpecific, 3},   // result
      {BerClass_ContextSpecific, 4},   // templateReference
   },

   // tupleItemDescription
   {
      {BerClass_ContextSpecific, 0},   // type
      {BerClass_ContextSpecific, 1},   // name
   },

   // invocation
   {
      {BerClass_ContextSpecific, 0},   // invocationId
      {BerClass_ContextSpecific, 1},   // arguments
   },

   // invocationResult
   {
      {BerClass_ContextSpecific, 0},   // invocationId
      {BerClass_ContextSpecific, 1},   // success
      {BerClass_ContextSpecific, 2},   // result
   },

   // root
   {BerClass_Application, 0},
};


// ====================================================================
//
// Glow internals
//
// ====================================================================

bool glow_assertIdentifierValid(pcstr pIdentifier, bool isRx)
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
// DOM support globals
//
// ====================================================================

bool glowParametersLocation_isValid(const GlowParametersLocation *pThis)
{
   return pThis->kind == GlowParametersLocationKind_Inline
       || pThis->choice.basePath.length > 0;
}

void glowTemplate_free(GlowTemplate *pThis)
{
    ASSERT(pThis != NULL);

    if (pThis->pDescription != NULL)
        freeMemory(pThis->pDescription);

    pThis->state = false;
}

void glowNode_free(GlowNode *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->pIdentifier != NULL)
      freeMemory(pThis->pIdentifier);

   if(pThis->pDescription != NULL)
      freeMemory(pThis->pDescription);

   if(pThis->pSchemaIdentifiers != NULL)
      freeMemory(pThis->pSchemaIdentifiers);

   if(pThis->pTemplateReference != NULL)
       freeMemory(pThis->pTemplateReference);

   bzero_item(*pThis);
}

void glowValue_copyFrom(GlowValue *pThis, const GlowValue *pSource)
{
   size_t length;

   ASSERT(pThis != NULL);
   ASSERT(pSource != NULL);

   pThis->flag = pSource->flag;

   switch(pSource->flag)
   {
      case GlowParameterType_String:
         if(pSource->choice.pString != NULL)
         {
            length = strlen(pSource->choice.pString) + 1;
            pThis->choice.pString = newarr(char, length);
            memcpy(pThis->choice.pString, pSource->choice.pString, length);
         }
         else
         {
            pThis->choice.pString = NULL;
         }
         break;

      case GlowParameterType_Octets:
         if(pSource->choice.octets.pOctets != NULL)
         {
            pThis->choice.octets.pOctets = newarr(byte, pSource->choice.octets.length);
            pThis->choice.octets.length = pSource->choice.octets.length;
            memcpy(pThis->choice.octets.pOctets, pSource->choice.octets.pOctets, pSource->choice.octets.length);
         }
         else
         {
            pThis->choice.octets.length = 0;
            pThis->choice.octets.pOctets = NULL;
         }
         break;

      default:
         memcpy(pThis, pSource, sizeof(GlowValue));
         break;
   }
}

void glowValue_free(GlowValue *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->flag == GlowParameterType_String)
   {
      if(pThis->choice.pString != NULL)
         freeMemory(pThis->choice.pString);
   }
   else if(pThis->flag == GlowParameterType_Octets)
   {
      if(pThis->choice.octets.pOctets != NULL)
         freeMemory(pThis->choice.octets.pOctets);
   }

   bzero_item(*pThis);
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

   if(pThis->defaultValue.flag != 0)
       glowValue_free(&pThis->defaultValue);

   if(pThis->pSchemaIdentifiers != NULL)
      freeMemory(pThis->pSchemaIdentifiers);

   if(pThis->pTemplateReference != NULL)
      freeMemory(pThis->pTemplateReference);

   bzero_item(*pThis);
}

void glowMatrix_free(GlowMatrix *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->pIdentifier != NULL)
      freeMemory(pThis->pIdentifier);

   if(pThis->pDescription != NULL)
      freeMemory(pThis->pDescription);

   if(pThis->pSchemaIdentifiers != NULL)
      freeMemory(pThis->pSchemaIdentifiers);

   if(pThis->pTemplateReference != NULL)
      freeMemory(pThis->pTemplateReference);

   bzero_item(*pThis);
}

void glowConnection_free(GlowConnection *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->pSources != NULL)
      freeMemory(pThis->pSources);

   bzero_item(*pThis);
}

void glowFunction_free(GlowFunction *pThis)
{
   int index;

   ASSERT(pThis != NULL);

   if(pThis->pIdentifier != NULL)
      freeMemory(pThis->pIdentifier);

   if(pThis->pDescription != NULL)
      freeMemory(pThis->pDescription);

   if(pThis->pTemplateReference != NULL)
      freeMemory(pThis->pTemplateReference);

   if(pThis->pArguments != NULL)
   {
      for(index = 0; index < pThis->argumentsLength; index++)
         glowTupleItemDescription_free(&pThis->pArguments[index]);

      freeMemory(pThis->pArguments);
   }

   if(pThis->pResult != NULL)
   {
      for(index = 0; index < pThis->resultLength; index++)
         glowTupleItemDescription_free(&pThis->pResult[index]);

      freeMemory(pThis->pResult);
   }

   bzero_item(*pThis);
}

void glowInvocation_free(GlowInvocation *pThis)
{
   int index;

   ASSERT(pThis != NULL);

   if(pThis->pArguments != NULL)
   {
      for(index = 0; index < pThis->argumentsLength; index++)
         glowValue_free(&pThis->pArguments[index]);

      freeMemory(pThis->pArguments);
   }

   bzero_item(*pThis);
}

void glowCommand_free(GlowCommand *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->number == GlowCommandType_Invoke)
      glowInvocation_free(&pThis->options.invocation);

   bzero_item(*pThis);
}

void glowInvocationResult_free(GlowInvocationResult *pThis)
{
   int index;

   ASSERT(pThis != NULL);

   if(pThis->pResult != NULL)
   {
      for(index = 0; index < pThis->resultLength; index++)
         glowValue_free(&pThis->pResult[index]);

      freeMemory(pThis->pResult);
   }

   bzero_item(*pThis);
}

void glowTupleItemDescription_free(GlowTupleItemDescription *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->pName != NULL)
      freeMemory(pThis->pName);

   bzero_item(*pThis);
}
