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
}

void byteBuffer_add(ByteBuffer *pThis, byte b)
{
   ASSERT(pThis != NULL);
   ASSERT(pThis->pMemory != NULL);

   if(pThis->position < pThis->size)
   {
      pThis->pMemory[pThis->position] = b;
      pThis->position++;
   }
   else
   {
      throwError(401, "buffer overrun");
   }
}

void byteBuffer_reset(ByteBuffer *pThis)
{
   ASSERT(pThis != NULL);
   ASSERT(pThis->pMemory != NULL);

   pThis->position = 0;
}

bool byteBuffer_isEmpty(const ByteBuffer *pThis)
{
   ASSERT(pThis != NULL);
   ASSERT(pThis->pMemory != NULL);

   return pThis->position == 0;
}
