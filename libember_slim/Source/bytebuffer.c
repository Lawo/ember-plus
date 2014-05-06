/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol
   Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

      bzero(*pThis);
   }
}
