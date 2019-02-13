/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <stdlib.h>
#include <string.h>
#include "ber.h"
#include "emberasyncreader.h"
#include "emberinternal.h"


// ======================================================
//
// EmberAsyncContainer locals
//
// ======================================================

static void container_init(__EmberAsyncContainer *pThis, const BerTag *pTag, bertype type, int length)
{
   pThis->tag = *pTag;
   pThis->type = type;
   pThis->length = length;
   pThis->bytesRead = 0;
   pThis->hasContent = 0;
}

static bool container_isEof(const __EmberAsyncContainer *pThis)
{
   return pThis->bytesRead >= pThis->length
       && pThis->length != BER_INDEFINITE_LENGTH;
}


// ======================================================
//
// EmberAsyncContainerStack locals
//
// ======================================================

static void containerStack_init(__EmberAsyncContainerStack *pThis)
{
   pThis->length = 0;
}

static __EmberAsyncContainer *containerStack_push(__EmberAsyncContainerStack *pThis, const BerTag *pTag, bertype type, int length)
{
   __EmberAsyncContainer *pItem;

   if(pThis->length < EMBER_MAX_TREE_DEPTH)
   {
      pItem = &pThis->items[pThis->length];
      container_init(pItem, pTag, type, length);
      pThis->length++;

      return pItem;
   }
   else
   {
      throwError(111, "ember tree stack overflow");
   }

   return NULL;
}

static __EmberAsyncContainer *containerStack_pop(__EmberAsyncContainerStack *pThis)
{
   if(pThis->length > 0)
   {
      pThis->length--;

      return &pThis->items[pThis->length];
   }
   else
   {
      throwError(112, "ember tree stack underflow");
   }

   return NULL;
}

static __EmberAsyncContainer *containerStack_peek(__EmberAsyncContainerStack *pThis)
{
   if(pThis->length > 0)
      return &pThis->items[pThis->length - 1];

   return NULL;
}


// ======================================================
//
// EmberAsyncReader locals
//
// ======================================================

static void resetState(EmberAsyncReader *pThis, __EmberAsyncDecodeState state)
{
   pThis->decodeState = state;
   pThis->bytesRead = 0;
   pThis->bytesExpected = 0;

   byteBuffer_reset(&pThis->base.buffer);
}

static void disposeCurrentTlv(EmberAsyncReader *pThis)
{
   berReader_reset(&pThis->base);
}

static void onValueReady(EmberAsyncReader *pThis)
{
   if(pThis->onItemReady != NULL)
      pThis->onItemReady(&pThis->base);

   resetState(pThis, DecodeState_Tag);
   disposeCurrentTlv(pThis);
}

static void pushContainer(EmberAsyncReader *pThis)
{
   BerReader *pBase = &pThis->base;

   pThis->pCurrentContainer = containerStack_push(pThis->pContainerStack, &pBase->tag, pBase->type, pBase->length);

   ASSERT(pThis->pCurrentContainer != NULL);
}

static bool readByte_Tag(EmberAsyncReader *pThis, byte b)
{
   BerReader *pBase;
   BerMemoryInput input;
   BerTag typeTag;

   if(b == 0 && pThis->bytesRead == 0)
   {
      resetState(pThis, DecodeState_Terminator);
      return false;
   }

   if(pThis->bytesRead > 6)
      throwError(103, "Number of tag octets out of bounds");

   if((pThis->bytesRead == 0 && (b & 0x1F) != 0x1F)
   || (pThis->bytesRead > 0 && (b & 0x80) == 0))
   {
      pBase = &pThis->base;
      berMemoryInput_init(&input, pBase->buffer.pMemory, (unsigned int)pBase->buffer.size);

      if(berTag_isZero(&pBase->tag))
      {
         pBase->tag = ber_decodeTag(&input.base);

         if(berTag_getClass(&pBase->tag) == BerClass_Universal)
            throwError(107, "Universal outer tag encountered");

         if(berTag_isContainer(&pBase->tag) == false)
            throwError(108, "Primitive outer tag encountered");

         berTag_setContainer(&pBase->tag, false);
      }
      else
      {
         typeTag = ber_decodeTag(&input.base);

         pBase->isContainer = berTag_isContainer(&typeTag);
         pBase->type = berTag_numberAsType(&typeTag);

         if(IsApplicationDefinedBerType(pBase->type) == false)
         {
            if(berTag_getClass(&typeTag) != BerClass_Universal)
               throwError(110, "Non-universal inner tag encountered");

            if(pBase->type == 0 || pBase->type >= BerType_LastUniversal)
               throwError(109, "Invalid BER type encountered");
         }
      }

      resetState(pThis, DecodeState_Length);
      return false;
   }

   pThis->bytesRead++;
   return false;
}

bool readByte_Length(EmberAsyncReader *pThis, byte b)
{
   BerReader *pBase;
   BerMemoryInput input;
   bool isEofOk;

   if(pThis->bytesExpected == 0)
   {
      if((b & 0x80) != 0)
         pThis->bytesExpected = (b & 0x7F) + 1;
      else
         pThis->bytesExpected = 1;

      if(pThis->bytesExpected > 5)
         throwError(104, "Number of length octets out of bounds");
   }

   pThis->bytesRead++;

   if(pThis->bytesRead == pThis->bytesExpected)
   {
      pBase = &pThis->base;
      berMemoryInput_init(&input, pBase->buffer.pMemory, (unsigned int)pBase->buffer.size);

      if(pBase->type == 0)
      {
         pBase->outerLength = ber_decodeLength(&input.base);

         if(pBase->outerLength == 0)
            throwError(102, "Zero outer length encountered");

         resetState(pThis, DecodeState_Tag);
      }
      else
      {
         pBase->length = ber_decodeLength(&input.base);
         isEofOk = pBase->length == 0;

         if(pBase->isContainer)
         {
            resetState(pThis, DecodeState_Tag);

            if(pThis->onNewContainer != NULL)
               pThis->onNewContainer(pBase);

            byte hasContent = 0;
            __EmberAsyncContainer* c = 0;

            if((pThis->pCurrentContainer != 0) && (pBase->tag.preamble == 0x80))
            {
               switch (pThis->pCurrentContainer->type)
               {
                  case 0x800a:
                  case 0x8003:
                     switch (pBase->tag.number)
                     {
                        case 1: // contents
                        case 2: // children
                           hasContent = pBase->tag.number;
                           c = pThis->pCurrentContainer;
                           break;

                        default:
                           break;
                     }
                     break;

                   default:
                     break;
                }
            }

            pushContainer(pThis);

            disposeCurrentTlv(pThis);
            if(c != 0)
            {
               c->hasContent = hasContent;
            }

            return isEofOk;
         }

         if(pBase->length == 0)
         {
            onValueReady(pThis);
         }
         else
         {
            resetState(pThis, DecodeState_Value);
         }

         return isEofOk;
      }
   }

   return false;
}

static bool readByte_Value(EmberAsyncReader *pThis)
{
   BerReader *pBase = &pThis->base;

   if(pThis->bytesRead == 0)
   {
      pThis->bytesExpected = pBase->length;
      byteBuffer_resize(&pThis->base.buffer, pBase->length);
   }

   pThis->bytesRead++;

   if(pThis->bytesRead == pThis->bytesExpected)
   {
      ASSERT(pThis->bytesRead == pBase->length);
      ASSERT(pBase->buffer.position == (unsigned int)pThis->bytesRead);

      onValueReady(pThis);
      return true;
   }

   return false;
}

static bool readByte_Terminator(EmberAsyncReader *pThis, byte b)
{
   if(pThis->pCurrentContainer == NULL
   || pThis->pCurrentContainer->length != BER_INDEFINITE_LENGTH)
      throwError(105, "Unexpected terminator");

   if(b == 0)
   {
      pThis->bytesRead++;

      if(pThis->bytesRead == 3)
      {
         // end of indefinite length container
         if(pThis->pCurrentContainer != NULL)
            pThis->pCurrentContainer->length = pThis->pCurrentContainer->bytesRead;

         return true;
      }
   }
   else
   {
      throwError(101, "Non-zero byte in terminator");
   }

   return false;
}

static bool endContainer(EmberAsyncReader *pThis)
{
   BerReader *pBase;
   __EmberAsyncContainer *pContainer;

   if(pThis->pContainerStack->length != 0)
   {
      pBase = &pThis->base;
      pContainer = containerStack_pop(pThis->pContainerStack);

      ASSERT(pContainer != NULL);
      ASSERT(pContainer == pThis->pCurrentContainer);

      pBase->tag = pContainer->tag;
      pBase->type = pContainer->type;
      pBase->length = pContainer->length;
      pBase->isContainer = true;

      if(pThis->onItemReady != NULL)
         pThis->onItemReady(pBase);

      if(pThis->pContainerStack->length != 0)
      {
         pThis->pCurrentContainer = containerStack_peek(pThis->pContainerStack);
         pThis->pCurrentContainer->bytesRead += pBase->length;
      }
      else
      {
         pThis->pCurrentContainer = NULL;
      }

      disposeCurrentTlv(pThis);
      resetState(pThis, DecodeState_Tag);
      return true;
   }

   resetState(pThis, DecodeState_Tag);
   return false;
}


// ======================================================
//
// EmberAsyncReader globals
//
// ======================================================

void emberAsyncReader_init(EmberAsyncReader *pThis)
{
   ASSERT(pThis != NULL);

   bzero_item(*pThis);

   berReader_init(&pThis->base);

   resetState(pThis, DecodeState_Tag);

   pThis->pContainerStack = newobj(__EmberAsyncContainerStack);
   containerStack_init(pThis->pContainerStack);
}

void emberAsyncReader_reset(EmberAsyncReader *pThis)
{
   ASSERT(pThis != NULL);
   ASSERT(pThis->pContainerStack != NULL);

   resetState(pThis, DecodeState_Tag);
   pThis->pCurrentContainer = NULL;
   containerStack_init(pThis->pContainerStack);
}

void emberAsyncReader_free(EmberAsyncReader *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->pContainerStack != NULL)
      freeMemory(pThis->pContainerStack);

   berReader_free(&pThis->base);

   bzero_item(*pThis);
}

void emberAsyncReader_readByte(EmberAsyncReader *pThis, byte b)
{
   BerReader *pBase;
   bool isEofOk = false;

   ASSERT(pThis != NULL);

   pBase = &pThis->base;
   byteBuffer_add(&pBase->buffer, b);

   if(pThis->pCurrentContainer != NULL)
      pThis->pCurrentContainer->bytesRead++;

   switch(pThis->decodeState)
   {
      case DecodeState_Tag:
         isEofOk = readByte_Tag(pThis, b);
         break;

      case DecodeState_Length:
         isEofOk = readByte_Length(pThis, b);
         break;

      case DecodeState_Value:
         isEofOk = readByte_Value(pThis);
         break;

      case DecodeState_Terminator:
         isEofOk = readByte_Terminator(pThis, b);
         break;

      default:
         FAIL(); // we should never get here
         break;
   }

   while(pThis->pCurrentContainer != NULL
      && container_isEof(pThis->pCurrentContainer))
   {
      if(isEofOk == false)
         throwError(106, "Unexpected end of container");

      endContainer(pThis);
   }
}

void emberAsyncReader_readBytes(EmberAsyncReader *pThis, const byte *pBytes, int count)
{
   ASSERT(pThis != NULL);

   for( ; count > 0; count--, pBytes++)
      emberAsyncReader_readByte(pThis, *pBytes);
}
