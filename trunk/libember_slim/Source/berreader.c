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
#include <stdlib.h>
#include "ber.h"
#include "emberinternal.h"


// ====================================================================
//
// Globals
//
// ====================================================================

void berReader_init(BerReader *pThis)
{
   ASSERT(pThis != NULL);

   bzero(*pThis);

   byteBuffer_initDynamic(&pThis->buffer, 0);
}

void berReader_free(BerReader *pThis)
{
   ASSERT(pThis != NULL);

   byteBuffer_free(&pThis->buffer);

   bzero(*pThis);
}

void berReader_reset(BerReader *pThis)
{
   ASSERT(pThis != NULL);

   bzero(pThis->tag);
   pThis->length = 0;
   pThis->type = BerType_Invalid;
   pThis->isContainer = false;
   pThis->outerLength = 0;
}

bool berReader_getBoolean(const BerReader *pThis)
{
   BerMemoryInput input;

   ASSERT(pThis != NULL);

   if(pThis->isContainer || pThis->length == 0 || byteBuffer_isEmpty(&pThis->buffer))
      throwError(201, "Invalid Boolean encoding");

   ASSERT(pThis->type == BerType_Boolean || IsApplicationDefinedBerType(pThis->type));

   berMemoryInput_init(&input, pThis->buffer.pMemory, pThis->length);

   return ber_decodeBoolean(&input.base);
}

berint berReader_getInteger(const BerReader *pThis)
{
   BerMemoryInput input;

   ASSERT(pThis != NULL);

   if(pThis->isContainer || pThis->length == 0 || byteBuffer_isEmpty(&pThis->buffer))
      throwError(202, "Invalid Integer encoding");

   ASSERT(pThis->type == BerType_Integer || IsApplicationDefinedBerType(pThis->type));

   berMemoryInput_init(&input, pThis->buffer.pMemory, pThis->length);

   return ber_decodeInteger(&input.base, pThis->length);
}

berlong berReader_getLong(const BerReader *pThis)
{
   BerMemoryInput input;

   if(pThis->isContainer || pThis->length == 0 || byteBuffer_isEmpty(&pThis->buffer))
      throwError(203, "Invalid Integer encoding");

   ASSERT(pThis->type == BerType_Integer || IsApplicationDefinedBerType(pThis->type));

   berMemoryInput_init(&input, pThis->buffer.pMemory, pThis->length);

   return ber_decodeLong(&input.base, pThis->length);
}

double berReader_getReal(const BerReader *pThis)
{
   BerMemoryInput input;

   if(pThis->isContainer)
      throwError(204, "Invalid Real encoding");

   ASSERT(pThis->type == BerType_Real || IsApplicationDefinedBerType(pThis->type));

   berMemoryInput_init(&input, pThis->buffer.pMemory, pThis->length);

   return ber_decodeReal(&input.base, pThis->length);
}

void berReader_getString(const BerReader *pThis, pstr pDest, int size)
{
   BerMemoryInput input;

   if(pThis->isContainer)
      throwError(205, "Invalid String encoding");

   ASSERT(pThis->type == BerType_UTF8String || IsApplicationDefinedBerType(pThis->type));

   berMemoryInput_init(&input, pThis->buffer.pMemory, pThis->length);

   ber_decodeString(&input.base, pDest, size < pThis->length ? size : pThis->length);
}

int berReader_getOctetString(const BerReader *pThis, byte *pDest, int size)
{
   BerMemoryInput input;
   int count;

   if(pThis->isContainer)
      throwError(206, "Invalid OctetString encoding");

   ASSERT(pThis->type == BerType_OctetString || IsApplicationDefinedBerType(pThis->type));

   berMemoryInput_init(&input, pThis->buffer.pMemory, pThis->length);

   count = size < pThis->length ? size : pThis->length;

   ber_decodeOctetString(&input.base, pDest, count);

   return count;
}

int berReader_getRelativeOid(const BerReader *pThis, berint *pDest, int destSize)
{
   BerMemoryInput input;

   if(pThis->isContainer)
      throwError(207, "Invalid ObjectIdentifier encoding");

   ASSERT(pThis->type == BerType_RelativeOid || IsApplicationDefinedBerType(pThis->type));

   berMemoryInput_init(&input, pThis->buffer.pMemory, pThis->length);

   return ber_decodeRelativeOid(&input.base, pDest, destSize, pThis->length);
}
