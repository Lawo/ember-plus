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
#include "glow.h"
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

void glowValue_copyFrom(GlowValue *pThis, const GlowValue *pSource)
{
   size_t length;

   ASSERT(pThis != NULL);
   ASSERT(pSource != NULL);

   pThis->flag = pSource->flag;

   switch(pSource->flag)
   {
      case GlowParameterType_String:
         if(pSource->pString != NULL)
         {
            length = strlen(pSource->pString) + 1;
            pThis->pString = newarr(char, length);
            memcpy(pThis->pString, pSource->pString, length);
         }
         else
         {
            pThis->pString = NULL;
         }
         break;

      case GlowParameterType_Octets:
         if(pSource->octets.pOctets != NULL)
         {
            pThis->octets.pOctets = newarr(byte, pSource->octets.length);
            pThis->octets.length = pSource->octets.length;
            memcpy(pThis->octets.pOctets, pSource->octets.pOctets, pSource->octets.length);
         }
         else
         {
            pThis->octets.length = 0;
            pThis->octets.pOctets = NULL;
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

void glowFunction_free(GlowFunction *pThis)
{
   int index;

   ASSERT(pThis != NULL);

   if(pThis->pIdentifier != NULL)
      freeMemory(pThis->pIdentifier);

   if(pThis->pDescription != NULL)
      freeMemory(pThis->pDescription);

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

   bzero(*pThis);
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

   bzero(*pThis);
}

void glowCommand_free(GlowCommand *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->number == GlowCommandType_Invoke)
      glowInvocation_free(&pThis->invocation);

   bzero(*pThis);
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

   bzero(*pThis);
}

void glowTupleItemDescription_free(GlowTupleItemDescription *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->pName != NULL)
      freeMemory(pThis->pName);

   bzero(*pThis);
}
