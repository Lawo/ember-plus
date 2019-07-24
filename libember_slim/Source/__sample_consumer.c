/*
   libember_slim sample

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <limits.h>

//$$ MSVCRT specific
#include <WinSock2.h>

#include "emberplus.h"
#include "emberinternal.h"

//$$ MSVCRT specific
// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")


#ifndef SECURE_CRT
#define stringCopy(dest, size, source) \
   do { strncpy(dest, source, size); dest[(size) - 1] = 0; } while(0)

#define printf_s printf
#define sscanf_s sscanf
#else
#define stringCopy(dest, size, source) \
   do { strncpy_s(dest, size, source, (size) - 1); dest[(size) - 1] = 0; } while(0)
#endif

#define stringDup(pStr) \
   (pStr != NULL ? _strdup(pStr) : NULL)

// ====================================================================
//
// utils
//
// ====================================================================

static void onThrowError(int error, pcstr pMessage)
{
   printf_s("ber error %d: '%s'\n", error, pMessage);
}

static void onFailAssertion(pcstr pFileName, int lineNumber)
{
   printf_s("Debug assertion failed @ '%s' line %d", pFileName, lineNumber);
}

static void initSockets()
{
   //$$ MSVCRT specific
   WSADATA wsaData;

   WSAStartup(MAKEWORD(2, 2), &wsaData);
}

static void shutdownSockets()
{
   //$$ MSVCRT specific
   WSACleanup();
}


// ====================================================================
//
// Linked List
//
// ====================================================================

typedef struct SPtrListNode
{
   voidptr value;
   struct SPtrListNode *pNext;
} PtrListNode;

typedef struct SPtrList
{
   PtrListNode *pHead;
   PtrListNode *pLast;
   int count;
} PtrList;

void ptrList_init(PtrList *pThis)
{
   bzero_item(*pThis);
}

void ptrList_addLast(PtrList *pThis, voidptr value)
{
   PtrListNode *pNode = newobj(PtrListNode);
   pNode->value = value;
   pNode->pNext = NULL;

   if(pThis->pLast == NULL)
   {
      pThis->pHead = pNode;
      pThis->pLast = pNode;
   }
   else
   {
      pThis->pLast->pNext = pNode;
      pThis->pLast = pNode;
   }

   pThis->count++;
}

void ptrList_free(PtrList *pThis)
{
   PtrListNode *pNode = pThis->pHead;
   PtrListNode *pPrev;

   while(pNode != NULL)
   {
      pPrev = pNode;
      pNode = pNode->pNext;

      freeMemory(pPrev);
   }

   bzero_item(*pThis);
}


// ====================================================================
//
// Model
//
// ====================================================================

typedef struct STarget
{
   berint number;
   berint *pConnectedSources;
   int connectedSourcesCount;
} Target;

typedef struct SSource
{
   berint number;
} Source;

typedef struct SElement
{
   berint number;
   GlowElementType type;
   GlowFieldFlags paramFields;

   union
   {
      GlowNode node;
      GlowParameter param;

      struct
      {
         GlowMatrix matrix;
         PtrList targets;
         PtrList sources;
      } matrix;

      GlowFunction function;
   } glow;

   PtrList children;
   struct SElement *pParent;
} Element;

static void element_init(Element *pThis, Element *pParent, GlowElementType type, berint number)
{
   bzero_item(*pThis);

   pThis->pParent = pParent;
   pThis->type = type;
   pThis->number = number;

   ptrList_init(&pThis->children);

   if(pParent != NULL)
      ptrList_addLast(&pParent->children, pThis);

   if(pThis->type == GlowElementType_Node)
      pThis->glow.node.isOnline = true;
}

static void element_free(Element *pThis)
{
   Element *pChild;
   Target *pTarget;
   PtrListNode *pNode;

   for(pNode = pThis->children.pHead; pNode != NULL; pNode = pNode->pNext)
   {
      pChild = (Element *)pNode->value;
      element_free(pChild);
      freeMemory(pChild);
   }

   ptrList_free(&pThis->children);

   if(pThis->type == GlowElementType_Parameter)
   {
      glowValue_free(&pThis->glow.param.value);

      if(pThis->glow.param.pEnumeration != NULL)
         freeMemory((void *)pThis->glow.param.pEnumeration);
      if(pThis->glow.param.pFormula != NULL)
         freeMemory((void *)pThis->glow.param.pFormula);
      if(pThis->glow.param.pFormat != NULL)
         freeMemory((void *)pThis->glow.param.pFormat);
   }
   else if(pThis->type == GlowElementType_Matrix)
   {
      for(pNode = pThis->glow.matrix.targets.pHead; pNode != NULL; pNode = pNode->pNext)
      {
         pTarget = (Target *)pNode->value;

         if(pTarget->pConnectedSources != NULL)
            freeMemory(pTarget->pConnectedSources);

         freeMemory(pTarget);
      }

      for(pNode = pThis->glow.matrix.sources.pHead; pNode != NULL; pNode = pNode->pNext)
      {
         if(pNode->value != NULL)
            freeMemory(pNode->value);
      }

      ptrList_free(&pThis->glow.matrix.targets);
      ptrList_free(&pThis->glow.matrix.sources);
   }
   else if(pThis->type == GlowElementType_Function)
   {
      glowFunction_free(&pThis->glow.function);
   }
   else if(pThis->type == GlowElementType_Node)
   {
      glowNode_free(&pThis->glow.node);
   }

   bzero_item(*pThis);
}

static pcstr element_getIdentifier(const Element *pThis)
{
   switch(pThis->type)
   {
      case GlowElementType_Node: return pThis->glow.node.pIdentifier;
      case GlowElementType_Parameter: return pThis->glow.param.pIdentifier;
      case GlowElementType_Matrix: return pThis->glow.matrix.matrix.pIdentifier;
      case GlowElementType_Function: return pThis->glow.function.pIdentifier;
   }

   return NULL;
}

static berint *element_getPath(const Element *pThis, berint *pBuffer, int *pCount)
{
   const Element *pElement;
   berint *pPosition = &pBuffer[*pCount];
   int count = 0;

   for(pElement = pThis; pElement->pParent != NULL; pElement = pElement->pParent)
   {
      pPosition--;

      if(pPosition < pBuffer)
         return NULL;

      *pPosition = pElement->number;
      count++;

   }

   *pCount = count;
   return pPosition;
}

static pcstr element_getIdentifierPath(const Element *pThis, pstr pBuffer, int bufferSize)
{
   const Element *pElement;
   pstr pPosition = &pBuffer[bufferSize - 1];
   int length;
   pcstr pIdentifier;

   *pPosition = 0;

   for(pElement = pThis; pElement->pParent != NULL; pElement = pElement->pParent)
   {
      if(*pPosition != 0)
         *--pPosition = '/';

      pIdentifier = element_getIdentifier(pElement);
      length = (int)strlen(pIdentifier);
      pPosition -= length;

      if(pPosition < pBuffer)
         return NULL;

      memcpy(pPosition, pIdentifier, length);
   }

   return pPosition;
}

static Element *element_findChild(const Element *pThis, berint number)
{
   Element *pChild;
   PtrListNode *pNode;

   for(pNode = pThis->children.pHead; pNode != NULL; pNode = pNode->pNext)
   {
      pChild = (Element *)pNode->value;

      if(pChild->number == number)
         return pChild;
   }

   return NULL;
}

static Element *element_findChildByIdentifier(const Element *pThis, pcstr pIdentifier)
{
   Element *pChild;
   pcstr pIdent;
   PtrListNode *pNode;

   for(pNode = pThis->children.pHead; pNode != NULL; pNode = pNode->pNext)
   {
      pChild = (Element *)pNode->value;
      pIdent = element_getIdentifier(pChild);

      if(_stricmp(pIdent, pIdentifier) == 0)
         return pChild;
   }

   return NULL;
}

static Element *element_findDescendant(const Element *pThis, const berint *pPath, int pathLength, Element **ppParent)
{
   int index;
   Element *pElement = (Element *)pThis;
   Element *pParent = NULL;

   for(index = 0; index < pathLength; index++)
   {
      if(pElement != NULL)
         pParent = pElement;

      pElement = element_findChild(pElement, pPath[index]);

      if(pElement == NULL)
      {
         if(index < pathLength - 1)
            pParent = NULL;

         break;
      }
   }

   if(ppParent != NULL)
      *ppParent = pParent;

   return pElement;
}

static GlowParameterType element_getParameterType(const Element *pThis)
{
   if(pThis->type == GlowElementType_Parameter)
   {
      if(pThis->paramFields & GlowFieldFlag_Enumeration)
         return GlowParameterType_Enum;

      if(pThis->paramFields & GlowFieldFlag_Value)
         return pThis->glow.param.value.flag;

      if(pThis->paramFields & GlowFieldFlag_Type)
         return pThis->glow.param.type;
   }

   return (GlowParameterType)0;
}

static Target *element_findOrCreateTarget(Element *pThis, berint number)
{
   Target *pTarget;
   PtrListNode *pNode;

   if(pThis->type == GlowElementType_Matrix)
   {
      for(pNode = pThis->glow.matrix.targets.pHead; pNode != NULL; pNode = pNode->pNext)
      {
         pTarget = (Target *)pNode->value;

         if(pTarget->number == number)
            return pTarget;
      }

      pTarget = newobj(Target);
      bzero_item(*pTarget);
      pTarget->number = number;

      ptrList_addLast(&pThis->glow.matrix.targets, pTarget);
      return pTarget;
   }

   return NULL;
}

static Source *element_findSource(const Element *pThis, berint number)
{
   Source *pSource;
   PtrListNode *pNode;

   if(pThis->type == GlowElementType_Matrix)
   {
      for(pNode = pThis->glow.matrix.sources.pHead; pNode != NULL; pNode = pNode->pNext)
      {
         pSource = (Source *)pNode->value;

         if(pSource->number == number)
            return pSource;
      }
   }

   return NULL;
}

static void printValue(const GlowValue *pValue)
{
   switch(pValue->flag)
   {
      case GlowParameterType_Integer:
         printf_s("integer '%lld'", pValue->choice.integer);
         break;

      case GlowParameterType_Real:
         printf_s("real '%lf'", pValue->choice.real);
         break;

      case GlowParameterType_String:
         printf_s("string '%s'", pValue->choice.pString);
         break;

      case GlowParameterType_Boolean:
         printf_s("boolean '%d'", pValue->choice.boolean);
         break;

      case GlowParameterType_Octets:
         printf_s("octets length %d", pValue->choice.octets.length);
         break;

      default:
         break;
   }
}

static void printMinMax(const GlowMinMax *pMinMax)
{
   switch(pMinMax->flag)
   {
      case GlowParameterType_Integer:
         printf_s("%lld", pMinMax->choice.integer);
         break;

      case GlowParameterType_Real:
         printf_s("%lf", pMinMax->choice.real);
         break;

      default:
         break;
   }
}

static void element_print(const Element *pThis, bool isVerbose)
{
   GlowFieldFlags fields;
   const GlowParameter *pParameter;
   const GlowMatrix *pMatrix;
   const GlowFunction *pFunction;
   int index;

   if(pThis->type == GlowElementType_Parameter)
   {
      pParameter = &pThis->glow.param;
      printf_s("P %04ld %s\n", pThis->number, pParameter->pIdentifier);

      if(isVerbose)
      {
         fields = pThis->paramFields;

         if(fields & GlowFieldFlag_Description)
            printf_s("  description:      %s\n", pParameter->pDescription);

         if(fields & GlowFieldFlag_Value)
         {
            printf_s("  value:            ");
            printValue(&pParameter->value);
            printf_s("\n");
         }

         if(fields & GlowFieldFlag_Minimum)
         {
            printf_s("  minimum:          ");
            printMinMax(&pParameter->minimum);
            printf_s("\n");
         }

         if(fields & GlowFieldFlag_Maximum)
         {
            printf_s("  maximum:          ");
            printMinMax(&pParameter->maximum);
            printf_s("\n");
         }

         if(fields & GlowFieldFlag_Access)
            printf_s("  access:           %d\n", pParameter->access);
         if(fields & GlowFieldFlag_Factor)
            printf_s("  factor:           %d\n", pParameter->factor);
         if(fields & GlowFieldFlag_IsOnline)
            printf_s("  isOnline:         %d\n", pParameter->isOnline);
         if(fields & GlowFieldFlag_Step)
            printf_s("  step:             %d\n", pParameter->step);
         if(fields & GlowFieldFlag_Type)
            printf_s("  type:             %d\n", pParameter->type);
         if(fields & GlowFieldFlag_StreamIdentifier)
            printf_s("  streamIdentifier: %ld\n", pParameter->streamIdentifier);

         if(fields & GlowFieldFlag_StreamDescriptor)
         {
            printf_s("  streamDescriptor:\n");
            printf_s("    format:         %d\n", pParameter->streamDescriptor.format);
            printf_s("    offset:         %d\n", pParameter->streamDescriptor.offset);
         }

         if(pParameter->pEnumeration != NULL)
            printf_s("  enumeration:\n%s\n", pParameter->pEnumeration);
         if(pParameter->pFormat != NULL)
            printf_s("  format:           %s\n", pParameter->pFormat);
         if(pParameter->pFormula != NULL)
            printf_s("  formula:\n%s\n", pParameter->pFormula);
         if(pParameter->pSchemaIdentifiers != NULL)
            printf_s("  schemaIdentifiers: %s\n", pParameter->pSchemaIdentifiers);
      }
   }
   else if(pThis->type == GlowElementType_Node)
   {
      printf_s("N %04ld %s\n", pThis->number, pThis->glow.node.pIdentifier);

      if(isVerbose)
      {
         printf_s("  description:      %s\n", pThis->glow.node.pDescription);
         printf_s("  isRoot:           %s\n", pThis->glow.node.isRoot ? "true" : "false");
         printf_s("  isOnline:         %s\n", pThis->glow.node.isOnline ? "true" : "false");

         if(pThis->glow.node.pSchemaIdentifiers != NULL)
            printf_s("  schemaIdentifiers: %s\n", pThis->glow.node.pSchemaIdentifiers);
      }
   }
   else if(pThis->type == GlowElementType_Matrix)
   {
      pMatrix = &pThis->glow.matrix.matrix;
      printf_s("M %04ld %s\n", pThis->number, pMatrix->pIdentifier);

      if(isVerbose)
      {
         if(pMatrix->pDescription != NULL)
            printf_s("  description:                 %s\n", pMatrix->pDescription);
         if(pMatrix->type != GlowMatrixType_OneToN)
            printf_s("  type:                        %d\n", pMatrix->type);
         if(pMatrix->addressingMode != GlowMatrixAddressingMode_Linear)
            printf_s("  addressingMode:              %d\n", pMatrix->addressingMode);
         printf_s("  targetCount:                 %d\n", pMatrix->targetCount);
         printf_s("  sourceCount:                 %d\n", pMatrix->sourceCount);
         if(pMatrix->maximumTotalConnects != 0)
            printf_s("  maximumTotalConnects:        %d\n", pMatrix->maximumTotalConnects);
         if(pMatrix->maximumConnectsPerTarget != 0)
            printf_s("  maximumConnectsPerTarget:    %d\n", pMatrix->maximumConnectsPerTarget);
         if(glowParametersLocation_isValid(&pMatrix->parametersLocation))
         {
            printf_s("  parametersLocation:          ");

            if(pMatrix->parametersLocation.kind == GlowParametersLocationKind_BasePath)
            {
               for(index = 0; index < pMatrix->parametersLocation.choice.basePath.length; index++)
               {
                  printf_s("%d", pMatrix->parametersLocation.choice.basePath.ids[index]);

                  if(index < pMatrix->parametersLocation.choice.basePath.length - 1)
                     printf_s(".");
               }

               printf_s("\n");
            }
            else if(pMatrix->parametersLocation.kind == GlowParametersLocationKind_Inline)
            {
               printf_s("%d\n", pMatrix->parametersLocation.choice.inlineId);
            }
         }
         if(pMatrix->pSchemaIdentifiers != NULL)
            printf_s("  schemaIdentifiers:            %s\n", pMatrix->pSchemaIdentifiers);
      }
   }
   else if(pThis->type == GlowElementType_Function)
   {
      pFunction = &pThis->glow.function;
      printf_s("F %04ld %s\n", pThis->number, pFunction->pIdentifier);

      if(isVerbose)
      {
         if(pFunction->pDescription != NULL)
            printf_s("  description:                 %s\n", pFunction->pDescription);
         if(pFunction->pArguments != NULL)
         {
            printf_s("  arguments:\n");
            for(index = 0; index < pFunction->argumentsLength; index++)
               printf_s("    %s:%d\n", pFunction->pArguments[index].pName, pFunction->pArguments[index].type);
         }
         if(pFunction->pResult != NULL)
         {
            printf_s("  result:\n");
            for(index = 0; index < pFunction->resultLength; index++)
               printf_s("    %s:%d\n", pFunction->pResult[index].pName, pFunction->pResult[index].type);
         }
      }
   }
}


// ====================================================================
//
// glow handlers
//
// ====================================================================

typedef struct SSession
{
   SOCKET sock;
   pcstr remoteAddress;
   int remotePort;
   Element root;
   Element *pCursor;
   berint cursorPathBuffer[GLOW_MAX_TREE_DEPTH];
   berint *pCursorPath;
   int cursorPathLength;

   // some reading state
   pstr pEnumeration;
   pstr pFormula;
   pstr pFormat;
} Session;

static void onNode(const GlowNode *pNode, GlowFieldFlags fields, const berint *pPath, int pathLength, voidptr state)
{
   Session *pSession = (Session *)state;
   Element *pElement;
   Element *pParent;

   // if received element is a child of current cursor, print it
   if(memcmp(pPath, pSession->pCursorPath, pSession->cursorPathLength * sizeof(berint)) == 0
   && pathLength == pSession->cursorPathLength + 1)
      printf_s("* N %04ld %s\n", pPath[pathLength - 1], pNode->pIdentifier);

   pElement = element_findDescendant(&pSession->root, pPath, pathLength, &pParent);

   if(pParent != NULL)
   {
      if(pElement == NULL)
      {
         pElement = newobj(Element);
         element_init(pElement, pParent, GlowElementType_Node, pPath[pathLength - 1]);

         if(fields & GlowFieldFlag_Identifier)
            pElement->glow.node.pIdentifier = stringDup(pNode->pIdentifier);
      }

      if(fields & GlowFieldFlag_Description)
         pElement->glow.node.pDescription = stringDup(pNode->pDescription);

      if(fields & GlowFieldFlag_IsOnline)
         pElement->glow.node.isOnline = pNode->isOnline;

      if(fields & GlowFieldFlag_IsRoot)
         pElement->glow.node.isRoot = pNode->isRoot;

      if(fields & GlowFieldFlag_SchemaIdentifier)
         pElement->glow.node.pSchemaIdentifiers = stringDup(pNode->pSchemaIdentifiers);
   }
}

static void onParameter(const GlowParameter *pParameter, GlowFieldFlags fields, const berint *pPath, int pathLength, voidptr state)
{
   Session *pSession = (Session *)state;
   Element *pElement;
   Element *pParent;
   GlowParameter *pLocalParam;

   // if received element is a child of current cursor, print it
   if(memcmp(pPath, pSession->pCursorPath, pSession->cursorPathLength * sizeof(berint)) == 0
   && pathLength == pSession->cursorPathLength + 1)
      printf_s("* P %04ld %s\n", pPath[pathLength - 1], pParameter->pIdentifier);

   pElement = element_findDescendant(&pSession->root, pPath, pathLength, &pParent);

   if(pParent != NULL)
   {
      if(pElement == NULL)
      {
         pElement = newobj(Element);
         element_init(pElement, pParent, GlowElementType_Parameter, pPath[pathLength - 1]);
      }

      pLocalParam = &pElement->glow.param;

      if((fields & GlowFieldFlag_Identifier) == GlowFieldFlag_Identifier)
         pLocalParam->pIdentifier = stringDup(pParameter->pIdentifier);
      if(fields & GlowFieldFlag_Description)
         pLocalParam->pDescription = stringDup(pParameter->pDescription);
      if(fields & GlowFieldFlag_Value)
      {
         glowValue_free(&pLocalParam->value);
         glowValue_copyTo(&pParameter->value, &pLocalParam->value);
      }
      if(fields & GlowFieldFlag_Minimum)
         memcpy(&pLocalParam->minimum, &pParameter->minimum, sizeof(GlowMinMax));
      if(fields & GlowFieldFlag_Maximum)
         memcpy(&pLocalParam->maximum, &pParameter->maximum, sizeof(GlowMinMax));
      if(fields & GlowFieldFlag_Access)
         pLocalParam->access = pParameter->access;
      if(fields & GlowFieldFlag_Factor)
         pLocalParam->factor = pParameter->factor;
      if(fields & GlowFieldFlag_IsOnline)
         pLocalParam->isOnline = pParameter->isOnline;
      if(fields & GlowFieldFlag_Step)
         pLocalParam->step = pParameter->step;
      if(fields & GlowFieldFlag_Type)
         pLocalParam->type = pParameter->type;
      if(fields & GlowFieldFlag_StreamIdentifier)
         pLocalParam->streamIdentifier = pParameter->streamIdentifier;
      if(fields & GlowFieldFlag_StreamDescriptor)
         memcpy(&pLocalParam->streamDescriptor, &pParameter->streamDescriptor, sizeof(GlowStreamDescription));
      if(fields & GlowFieldFlag_SchemaIdentifier)
         pLocalParam->pSchemaIdentifiers = stringDup(pParameter->pSchemaIdentifiers);

      if(pSession->pEnumeration != NULL)
      {
         pLocalParam->pEnumeration = pSession->pEnumeration;
         pSession->pEnumeration = NULL;
      }
      if(pSession->pFormat != NULL)
      {
         pLocalParam->pFormat = pSession->pFormat;
         pSession->pFormat = NULL;
      }
      if(pSession->pFormula != NULL)
      {
         pLocalParam->pFormula = pSession->pFormula;
         pSession->pFormula = NULL;
      }

      pElement->paramFields = (GlowFieldFlags)(pElement->paramFields | fields);

      // if cursor parameter has updated value, print value
      if(pElement == pSession->pCursor
      && (fields & GlowFieldFlag_Value))
      {
         printValue(&pParameter->value);
         printf_s("\n");
      }
   }
}

static void onMatrix(const GlowMatrix *pMatrix, const berint *pPath, int pathLength, voidptr state)
{
   Session *pSession = (Session *)state;
   Element *pElement;
   Element *pParent;

   // if received element is a child of current cursor, print it
   if(memcmp(pPath, pSession->pCursorPath, pSession->cursorPathLength * sizeof(berint)) == 0
   && pathLength == pSession->cursorPathLength + 1)
      printf_s("* M %04ld %s\n", pPath[pathLength - 1], pMatrix->pIdentifier);

   pElement = element_findDescendant(&pSession->root, pPath, pathLength, &pParent);

   if(pParent != NULL)
   {
      if(pElement == NULL)
      {
         pElement = newobj(Element);
         element_init(pElement, pParent, GlowElementType_Matrix, pPath[pathLength - 1]);
      }

      memcpy(&pElement->glow.matrix.matrix, pMatrix, sizeof(*pMatrix));
      pElement->glow.matrix.matrix.pIdentifier = stringDup(pMatrix->pIdentifier);
      pElement->glow.matrix.matrix.pDescription = stringDup(pMatrix->pDescription);
      pElement->glow.matrix.matrix.pSchemaIdentifiers = stringDup(pMatrix->pSchemaIdentifiers);
   }
}

static void onTarget(const GlowSignal *pSignal, const berint *pPath, int pathLength, voidptr state)
{
   Session *pSession = (Session *)state;
   Element *pElement;

   // if signal resides in cursor element, print it
   if(memcmp(pPath, pSession->pCursorPath, pSession->cursorPathLength * sizeof(berint)) == 0
   && pathLength == pSession->cursorPathLength)
      printf_s("* T %04ld\n", pSignal->number);

   pElement = element_findDescendant(&pSession->root, pPath, pathLength, NULL);

   if(pElement != NULL
   && pElement->type == GlowElementType_Matrix)
      element_findOrCreateTarget(pElement, pSignal->number);
}

static void onSource(const GlowSignal *pSignal, const berint *pPath, int pathLength, voidptr state)
{
   Session *pSession = (Session *)state;
   Element *pElement;
   Source *pSource;

   // if signal resides in cursor element, print it
   if(memcmp(pPath, pSession->pCursorPath, pSession->cursorPathLength * sizeof(berint)) == 0
   && pathLength == pSession->cursorPathLength)
      printf_s("* S %04ld\n", pSignal->number);

   pElement = element_findDescendant(&pSession->root, pPath, pathLength, NULL);

   if(pElement != NULL
   && pElement->type == GlowElementType_Matrix)
   {
      pSource = newobj(Source);
      bzero_item(*pSource);
      pSource->number = pSignal->number;

      ptrList_addLast(&pElement->glow.matrix.sources, pSource);
   }
}

static void onConnection(const GlowConnection *pConnection, const berint *pPath, int pathLength, voidptr state)
{
   Session *pSession = (Session *)state;
   Element *pElement;
   int index;
   Target *pTarget;

   // if signal resides in cursor element, print it
   if(memcmp(pPath, pSession->pCursorPath, pSession->cursorPathLength * sizeof(berint)) == 0
   && pathLength == pSession->cursorPathLength)
   {
      printf_s("* C %04ld <- [", pConnection->target);

      for(index = 0; index < pConnection->sourcesLength; index++)
      {
         printf_s("%04ld", pConnection->pSources[index]);

         if(index < pConnection->sourcesLength - 1)
            printf_s(", ");
      }

      printf_s("]\n");
   }

   pElement = element_findDescendant(&pSession->root, pPath, pathLength, NULL);

   if(pElement != NULL
   && pElement->type == GlowElementType_Matrix)
   {
      pTarget = element_findOrCreateTarget(pElement, pConnection->target);

      if(pTarget != NULL)
      {
         if(pTarget->pConnectedSources != NULL)
            freeMemory(pTarget->pConnectedSources);

         pTarget->pConnectedSources = newarr(berint, pConnection->sourcesLength);
         memcpy(pTarget->pConnectedSources, pConnection->pSources, pConnection->sourcesLength * sizeof(berint));
         pTarget->connectedSourcesCount = pConnection->sourcesLength;
      }
   }
}

static void cloneTupleItemDescription(GlowTupleItemDescription *pDest, const GlowTupleItemDescription *pSource)
{
   memcpy(pDest, pSource, sizeof(*pSource));
   pDest->pName = stringDup(pSource->pName);
}

static void onFunction(const GlowFunction *pFunction, const berint *pPath, int pathLength, voidptr state)
{
   Session *pSession = (Session *)state;
   Element *pElement;
   Element *pParent;
   int index;

   // if received element is a child of current cursor, print it
   if(memcmp(pPath, pSession->pCursorPath, pSession->cursorPathLength * sizeof(berint)) == 0
   && pathLength == pSession->cursorPathLength + 1)
      printf_s("* F %04ld %s\n", pPath[pathLength - 1], pFunction->pIdentifier);

   pElement = element_findDescendant(&pSession->root, pPath, pathLength, &pParent);

   if(pParent != NULL)
   {
      if(pElement == NULL)
      {
         pElement = newobj(Element);
         element_init(pElement, pParent, GlowElementType_Function, pPath[pathLength - 1]);
      }

      memcpy(&pElement->glow.function, pFunction, sizeof(*pFunction));
      pElement->glow.function.pIdentifier = stringDup(pFunction->pIdentifier);
      pElement->glow.function.pDescription = stringDup(pFunction->pDescription);

      // clone arguments
      if(pFunction->pArguments != NULL)
      {
         pElement->glow.function.pArguments = newarr(GlowTupleItemDescription, pFunction->argumentsLength);

         for(index = 0; index < pFunction->argumentsLength; index++)
            cloneTupleItemDescription(&pElement->glow.function.pArguments[index], &pFunction->pArguments[index]);
      }

      // clone result
      if(pFunction->pResult != NULL)
      {
         pElement->glow.function.pResult = newarr(GlowTupleItemDescription, pFunction->resultLength);

         for(index = 0; index < pFunction->resultLength; index++)
            cloneTupleItemDescription(&pElement->glow.function.pResult[index], &pFunction->pResult[index]);
      }
   }
}

static void onInvocationResult(const GlowInvocationResult *pInvocationResult, voidptr state)
{
   int index;
   pcstr status = pInvocationResult->hasError
                  ? "error"
                  : "ok";

   printf_s("* IR %04ld %s\n", pInvocationResult->invocationId, status);

   if(pInvocationResult->hasError == false)
   {
      for(index = 0; index < pInvocationResult->resultLength; index++)
      {
         printf_s("    ");
         printValue(&pInvocationResult->pResult[index]);
         printf_s("\n");
      }
   }
}

static void onOtherPackageReceived(const byte *pPackage, int length, voidptr state)
{
   Session *pSession = (Session *)state;
   SOCKET sock = pSession->sock;
   byte buffer[16];
   unsigned int txLength;

   if(length >= 4
   && pPackage[1] == EMBER_MESSAGE_ID
   && pPackage[2] == EMBER_COMMAND_KEEPALIVE_REQUEST)
   {
      txLength = emberFraming_writeKeepAliveResponse(buffer, sizeof(buffer), pPackage[0]);
      send(sock, (char *)buffer, txLength, 0);
   }
}

void onUnsupportedTltlv(const BerReader *pReader, const berint *pPath, int pathLength, GlowReaderPosition position, voidptr state)
{
   Session *pSession = (Session *)state;

   if(position == GlowReaderPosition_ParameterContents)
   {
      if(berTag_equals(&pReader->tag, &glowTags.parameterContents.enumeration))
      {
         pSession->pEnumeration = newarr(char, pReader->length + 1);
         berReader_getString(pReader, pSession->pEnumeration, pReader->length);
      }
      else if(berTag_equals(&pReader->tag, &glowTags.parameterContents.formula))
      {
         pSession->pFormula = newarr(char, pReader->length + 1);
         berReader_getString(pReader, pSession->pFormula, pReader->length);
      }
      else if(berTag_equals(&pReader->tag, &glowTags.parameterContents.format))
      {
         pSession->pFormat = newarr(char, pReader->length + 1);
         berReader_getString(pReader, pSession->pFormat, pReader->length);
      }
   }
}


// ====================================================================
//
// main loop
//
// ====================================================================

static void writePrompt(Session *pSession)
{
   const int identPathBufferSize = 256;
   pstr pIdentPathBuffer;
   pcstr pIdentPath;

   pIdentPathBuffer = newarr(char, identPathBufferSize);
   pIdentPath = element_getIdentifierPath(pSession->pCursor, pIdentPathBuffer, identPathBufferSize);

   printf_s("%s:%d/", pSession->remoteAddress, pSession->remotePort);

   if(pIdentPath != NULL)
      printf_s("%s", pIdentPath);

   printf_s(">");

   freeMemory(pIdentPathBuffer);
}

static Element *findElement(pcstr pIdentifier, const Session *pSession)
{
   if(strcmp(pIdentifier, ".") == 0)
   {
      return pSession->pCursor;
   }
   else if(strcmp(pIdentifier, "..") == 0)
   {
      return pSession->pCursor->pParent;
   }

   return element_findChildByIdentifier(pSession->pCursor, pIdentifier);
}

static bool setParameterValue(const Session *pSession, pcstr pValueString)
{
   GlowOutput output;
   const int bufferSize = 512;
   byte *pBuffer;
   const Element *pElement = pSession->pCursor;
   GlowParameterType type = element_getParameterType(pElement);
   GlowParameter parameter;

   bzero_item(parameter);

   switch(type)
   {
      case GlowParameterType_Integer:
      case GlowParameterType_Enum:
         sscanf_s(pValueString, "%lld", &parameter.value.choice.integer);
         parameter.value.flag = GlowParameterType_Integer;
         break;

      case GlowParameterType_Boolean:
         parameter.value.choice.boolean = atoi(pValueString) != 0;
         parameter.value.flag = GlowParameterType_Boolean;
         break;

      case GlowParameterType_Real:
         sscanf_s(pValueString, "%lf", &parameter.value.choice.real);
         parameter.value.flag = GlowParameterType_Real;
         break;

      case GlowParameterType_String:
         parameter.value.choice.pString = (pstr)pValueString;
         parameter.value.flag = GlowParameterType_String;
         break;

      default:
         return false;
   }

   pBuffer = newarr(byte, bufferSize);
   glowOutput_init(&output, pBuffer, bufferSize, 0);
   glowOutput_beginPackage(&output, true);
   glow_writeQualifiedParameter(&output, &parameter, GlowFieldFlag_Value, pSession->pCursorPath, pSession->cursorPathLength);
   send(pSession->sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
   freeMemory(pBuffer);
   return true;
}

static pcstr parseInteger(pcstr pString, berint *pInteger)
{
   berint integer = 0;

   while(isspace(*pString))
      pString++;

   if(isdigit(*pString))
   {
      for( ; isdigit(*pString); pString++)
         integer = integer * 10 + (*pString - '0');

      *pInteger = integer;
      return pString;
   }

   return NULL;
}

static bool issueConnect(const Session *pSession, pcstr pArguments, GlowConnectionOperation operation)
{
   GlowOutput output;
   const int bufferSize = 512;
   byte *pBuffer;
   const Element *pElement = pSession->pCursor;
   GlowConnection connection;
   bool result = false;

   bzero_item(connection);

   if(pElement->type == GlowElementType_Matrix)
   {
      pArguments = parseInteger(pArguments, &connection.target);

      if(pArguments != NULL)
      {
         connection.operation = operation;
         connection.pSources = newarr(berint, 64);

         while((pArguments = parseInteger(pArguments, &connection.pSources[connection.sourcesLength])) != NULL)
            connection.sourcesLength++;

         if(connection.sourcesLength > 0)
         {
            pBuffer = newarr(byte, bufferSize);
            glowOutput_init(&output, pBuffer, bufferSize, 0);
            glowOutput_beginPackage(&output, true);
            glow_writeConnectionsPrefix(&output, pSession->pCursorPath, pSession->cursorPathLength);
            glow_writeConnection(&output, &connection);
            glow_writeConnectionsSuffix(&output);
            send(pSession->sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
            freeMemory(pBuffer);
            result = true;
         }
      }
   }

   glowConnection_free(&connection);
   return result;
}

static bool invoke(const Session *pSession, pcstr pArguments)
{
   printf_s("please implement this function\n");
   return false;
   // *** sample implementation for a function that takes two integer arguments
   //GlowOutput output;
   //const int bufferSize = 512;
   //byte *pBuffer;
   //const Element *pElement = pSession->pCursor;
   //GlowCommand command;
   //bool result = false;
   //GlowValue value[2];

   //value[0].flag = GlowParameterType_Integer;
   //value[0].integer = 101;
   //value[1].flag = GlowParameterType_Integer;
   //value[1].integer = 202;

   //bzero_item(command);

   //if(pElement->type == GlowElementType_Function)
   //{
   //   command.number = GlowCommandType_Invoke;
   //   command.invocation.invocationId = 1;
   //   command.invocation.pArguments = value;
   //   command.invocation.argumentsLength = 2;

   //   pBuffer = newarr(byte, bufferSize);
   //   glowOutput_init(&output, pBuffer, bufferSize, 0);
   //   glowOutput_beginPackage(&output, true);
   //   glow_writeQualifiedCommand(&output, &command, pSession->pCursorPath, pSession->cursorPathLength, pElement->type);
   //   send(pSession->sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
   //   freeMemory(pBuffer);
   //   result = true;
   //}

   //return result;
}

static bool handleInput(Session *pSession, pcstr pInput)
{
   GlowOutput output;
   const int bufferSize = 512;
   byte *pBuffer;
   GlowCommand command;
   pcstr pArgument;
   Element *pElement;
   PtrListNode *pNode;

   if(strcmp(pInput, "dir") == 0)
   {
      pBuffer = newarr(byte, bufferSize);

      bzero_item(command);
      command.number = GlowCommandType_GetDirectory;

      glowOutput_init(&output, pBuffer, bufferSize, 0);
      glowOutput_beginPackage(&output, true);
      glow_writeQualifiedCommand(
         &output,
         &command,
         pSession->pCursorPath,
         pSession->cursorPathLength,
         pSession->pCursor->type);
      send(pSession->sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);

      freeMemory(pBuffer);

      printf_s("|>\n");
   }
   else if(strcmp(pInput, "ls") == 0)
   {
      for(pNode = pSession->pCursor->children.pHead; pNode != NULL; pNode = pNode->pNext)
         element_print((Element *)pNode->value, false);

      writePrompt(pSession);
   }
   else if(strncmp(pInput, "cd ", 3) == 0)
   {
      pArgument = &pInput[3];
      pElement = findElement(pArgument, pSession);

      if(pElement != NULL)
      {
         pSession->cursorPathLength = GLOW_MAX_TREE_DEPTH;
         pSession->pCursorPath = element_getPath(pElement, pSession->cursorPathBuffer, &pSession->cursorPathLength);
         pSession->pCursor = pElement;
      }

      writePrompt(pSession);
   }
   else if(strcmp(pInput, "cd..") == 0)
   {
      pElement = pSession->pCursor->pParent;

      if(pElement != NULL)
      {
         pSession->cursorPathLength = GLOW_MAX_TREE_DEPTH;
         pSession->pCursorPath = element_getPath(pElement, pSession->cursorPathBuffer, &pSession->cursorPathLength);
         pSession->pCursor = pElement;
      }

      writePrompt(pSession);
   }
   else if(strncmp(pInput, "print ", 6) == 0)
   {
      pArgument = &pInput[6];
      pElement = findElement(pArgument, pSession);

      if(pElement != NULL)
         element_print(pElement, true);

      writePrompt(pSession);
   }
   else if(strcmp(pInput, "print") == 0)
   {
      element_print(pSession->pCursor, true);

      writePrompt(pSession);
   }
   else if(strncmp(pInput, "set ", 4) == 0)
   {
      pArgument = &pInput[4];

      if(setParameterValue(pSession, pArgument))
         printf_s("|>\n");
      else
         writePrompt(pSession);
   }
   else if(strncmp(pInput, "connect ", 8) == 0)
   {
      pArgument = &pInput[8];

      if(issueConnect(pSession, pArgument, GlowConnectionOperation_Connect))
         printf_s("|>\n");
      else
         writePrompt(pSession);
   }
   else if(strncmp(pInput, "disconnect ", 11) == 0)
   {
      pArgument = &pInput[11];

      if(issueConnect(pSession, pArgument, GlowConnectionOperation_Disconnect))
         printf_s("|>\n");
      else
         writePrompt(pSession);
   }
   else if(strncmp(pInput, "invoke ", 7) == 0)
   {
      pArgument = &pInput[7];

      if(invoke(pSession, pArgument))
         printf_s("|>\n");
      else
         writePrompt(pSession);
   }
   else if(strcmp(pInput, "?") == 0)
   {
      printf_s("dir             - send GetDirectory command\n"
               "cd IDENT        - change to child with identifier IDENT\n"
               "ls              - list children\n"
               "print [IDENT]   - print child with identifier IDENT\n"
               "set VALUE       - set parameter value to VALUE\n"
               "connect T S     - connect S to T\n"
               "disconnect T S  - disconnect S from T\n"
               "invoke VALUE... - invoke function with arguments VALUE...\n"
               "quit            - exit application\n"
               "for IDENT you can also write '.' or '..'.\n");

      writePrompt(pSession);
   }
   else if(*pInput == 0)
   {
      writePrompt(pSession);
   }
   else if(strcmp(pInput, "quit") == 0)
   {
      return true;
   }
   else
   {
      printf_s("Unknown command\n");
      writePrompt(pSession);
   }

   return false;
}

static void run(Session *pSession)
{
   static char s_input[256];
   byte buffer[64];
   int read;
   char ch;
   int inputLength = 0;
   bool isQuit = false;
   const struct timeval timeout = {0, 16 * 1000}; // 16 milliseconds timeout for select()
   const int rxBufferSize = 1290; // max size of unescaped package
   fd_set fdset;
   int fdsReady;
   GlowReader *pReader = newobj(GlowReader);
   byte *pRxBuffer = newarr(byte, rxBufferSize);
   SOCKET sock = pSession->sock;

   glowReader_init(pReader, onNode, onParameter, NULL, NULL, (voidptr)pSession, pRxBuffer, rxBufferSize);
   pReader->base.onMatrix = onMatrix;
   pReader->base.onTarget = onTarget;
   pReader->base.onSource = onSource;
   pReader->base.onConnection = onConnection;
   pReader->base.onFunction = onFunction;
   pReader->base.onInvocationResult = onInvocationResult;
   pReader->onOtherPackageReceived = onOtherPackageReceived;
   pReader->base.onUnsupportedTltlv = onUnsupportedTltlv;

   printf_s("Enter '?' for help\n");
   writePrompt(pSession);

   while(isQuit == false)
   {
      if(_kbhit())
      {
         ch = (char)_getch();

         if(ch == '\n' || ch == '\r')
         {
            printf_s("\n");

            s_input[inputLength] = 0;
            isQuit = handleInput(pSession, s_input);

            inputLength = 0;
         }
         else if(ch == '\b')
         {
            if(inputLength > 0)
            {
               _putch(ch);
               _putch(' ');
               _putch(ch);

               inputLength--;
            }
         }
         else
         {
            if(inputLength < sizeof(s_input) - 1)
            {
               _putch(ch);

               s_input[inputLength] = ch;
               inputLength++;
            }
         }
      }
      else
      {
         FD_ZERO(&fdset);
         FD_SET(sock, &fdset);

         fdsReady = select(1, &fdset, NULL, NULL, &timeout);

         if(fdsReady == 1) // socket is ready to read
         {
            if(FD_ISSET(sock, &fdset))
            {
               read = recv(sock, (char *)buffer, sizeof(buffer), 0);

               if(read > 0)
                  glowReader_readBytes(pReader, buffer, read);
               else
                  isQuit = true;
            }
         }
         else if(fdsReady < 0) // connection lost
         {
            isQuit = true;
         }
      }
   }

   glowReader_free(pReader);
   freeMemory(pRxBuffer);
   freeMemory(pReader);
}


// ====================================================================
//
// consumer sample entry point
//
// ====================================================================

static volatile int allocCount = 0;

static void *allocMemoryImpl(size_t size)
{
   void *pMemory = malloc(size);

   //if(sizeof(void *) == 8)
   //   printf("allocate %lu bytes: %llX\n", size, (unsigned long long)pMemory);
   //else
   //   printf("allocate %lu bytes: %lX\n", size, (unsigned long)pMemory);

   allocCount++;
   return pMemory;
}

static void freeMemoryImpl(void *pMemory)
{
   //if(sizeof(void *) == 8)
   //   printf("free: %llX\n", (unsigned long long)pMemory);
   //else
   //   printf("free: %lX\n", (unsigned long)pMemory);

   allocCount--;
   free(pMemory);
}

void runConsumer(int argc, char **argv)
{
   int port;
   struct sockaddr_in remoteAddr;
   unsigned long uladdr;
   SOCKET sock;
   int result;
   Session session;

   ember_init(onThrowError, onFailAssertion, allocMemoryImpl, freeMemoryImpl);

   initSockets();

   if(argc >= 3)
   {
      uladdr = inet_addr(argv[1]);
      port = atoi(argv[2]);

      if(port > 0
      && uladdr != INADDR_NONE
      && uladdr != INADDR_ANY)
      {
         remoteAddr.sin_family = AF_INET;
         remoteAddr.sin_port = htons((unsigned short)(port & 0xFFFF));
         remoteAddr.sin_addr.S_un.S_addr = uladdr;

         printf_s("connecting to %s:%d...\n", inet_ntoa(remoteAddr.sin_addr), port);

         sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
         result = connect(sock, (struct sockaddr *)&remoteAddr, sizeof(remoteAddr));

         if(result != SOCKET_ERROR)
         {
            bzero_item(session);
            session.sock = sock;
            session.pCursor = &session.root;
            session.remoteAddress = argv[1];
            session.remotePort = port;
            element_init(&session.root, NULL, GlowElementType_Node, 0);

            run(&session);

            element_free(&session.root);
            closesocket(sock);
         }
         else
         {
            printf_s("connect error.\n");
         }
      }
      else
      {
         printf_s("address or port error.\n");
      }
   }

   if(allocCount > 0)
   {
      printf("UNFREED MEMORY DETECTED %d!\n", allocCount);
   }

   shutdownSockets();
}
