/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <string.h>
#include "bytebuffer.h"
#include "emberinternal.h"

void byteBuffer_init(ByteBuffer *pThis, byte *pMemory, unsigned int size)
{
   ASSERT(pThis != NULL);
   ASSERT(pMemory != NULL);
   ASSERT(size > 0);

   pThis->pMemory = pMemory;
   pThis->size = size;
   pThis->position = 0;
   pThis->isDynamic = false;
}

void byteBuffer_initDynamic(ByteBuffer *pThis, unsigned int size)
{
   ASSERT(pThis != NULL);
   ASSERT(size >= 0);

   byteBuffer_free(pThis);

   pThis->pMemory = size > 0
                    ? newarr(byte, size)
                    : NULL;
   pThis->size = size;
   pThis->position = 0;
   pThis->isDynamic = true;
}

void byteBuffer_add(ByteBuffer *pThis, byte b)
{
   unsigned int grow;

   ASSERT(pThis != NULL);
   ASSERT(pThis->pMemory != NULL || pThis->isDynamic);

   if(pThis->position >= pThis->size)
   {
      if(pThis->isDynamic)
      {
         grow = pThis->size;

         if(grow > 64)
            grow = 64;

         if(grow < 16)
            grow = 16;

         byteBuffer_resize(pThis, pThis->size + grow);
      }
      else
      {
         throwError(401, "buffer overrun");
         return;
      }
   }

   pThis->pMemory[pThis->position] = b;
   pThis->position++;
}

void byteBuffer_reset(ByteBuffer *pThis)
{
   ASSERT(pThis != NULL);
   ASSERT(pThis->pMemory != NULL || pThis->isDynamic);

   pThis->position = 0;
}

void byteBuffer_resize(ByteBuffer *pThis, unsigned int size)
{
   byte *pMemory;
   ASSERT(pThis != NULL);

   if(pThis->isDynamic)
   {
      if(pThis->size < size)
      {
         pMemory = newarr(byte, size);

         if(pThis->pMemory != NULL)
         {
            memcpy(pMemory, pThis->pMemory, pThis->size);
            freeMemory(pThis->pMemory);
         }

         pThis->pMemory = pMemory;
         pThis->size = size;
      }
   }
}

bool byteBuffer_isEmpty(const ByteBuffer *pThis)
{
   ASSERT(pThis != NULL);
   ASSERT(pThis->pMemory != NULL);

   return pThis->position == 0;
}

void byteBuffer_free(ByteBuffer *pThis)
{
   ASSERT(pThis != NULL);

   if(pThis->isDynamic)
   {
      if(pThis->pMemory != NULL)
         freeMemory(pThis->pMemory);

      bzero_item(*pThis);
   }
}
