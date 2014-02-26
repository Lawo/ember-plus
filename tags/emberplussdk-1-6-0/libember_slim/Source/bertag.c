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

#include "ber.h"
#include "emberinternal.h"

void berTag_init(BerTag *pThis, BerClass berClass, tagnumber number)
{
   ASSERT(pThis != NULL);

   pThis->preamble = berClass & BerClass_Mask;
   pThis->number = number;
}

bool berTag_isContainer(const BerTag *pThis)
{
   ASSERT(pThis != NULL);

   return (pThis->preamble & BER_CONTAINER_FLAG) != 0;
}

void berTag_setContainer(BerTag *pThis, bool value)
{
   ASSERT(pThis != NULL);

   if(value)
      pThis->preamble |= BER_CONTAINER_FLAG;
   else
      pThis->preamble &= ~BER_CONTAINER_FLAG;
}

BerClass berTag_getClass(const BerTag *pThis)
{
   ASSERT(pThis != NULL);

   return (BerClass)(pThis->preamble & BerClass_Mask);
}

void berTag_setClass(BerTag *pThis, BerClass value)
{
   ASSERT(pThis != NULL);

   pThis->preamble &= ~BerClass_Mask;
   pThis->preamble |= value & BerClass_Mask;
}

bool berTag_isZero(const BerTag *pThis)
{
   ASSERT(pThis != NULL);

   return pThis->preamble == 0 && pThis->number == 0;
}

BerTag berTag_toContainer(const BerTag *pThis)
{
   BerTag tag;

   ASSERT(pThis != NULL);

   tag.preamble = (byte)(pThis->preamble | BER_CONTAINER_FLAG);
   tag.number = pThis->number;
   return tag;
}

bertype berTag_numberAsType(const BerTag *pThis)
{
   ASSERT(pThis != NULL);

   return berTag_getClass(pThis) == BerClass_Application
          ? pThis->number | BerType_ApplicationFlag
          : pThis->number;
}

#ifdef SECURE_CRT
void berTag_toString(const BerTag *pThis, pstr pBuffer, int bufferSize)
{
   BerClass tagClass;
   pcstr pContainerSuffix;

   ASSERT(pThis != NULL);

   tagClass = berTag_getClass(pThis);
   pContainerSuffix = berTag_isContainer(pThis) ? "!C" : "";

   sprintf_s(pBuffer, bufferSize, "%s-%u%s", ber_getShortClassName(tagClass), (unsigned int)pThis->number, pContainerSuffix);
}
#else
void berTag_toString(const BerTag *pThis, pstr pBuffer)
{
   BerClass tagClass;
   pcstr pContainerSuffix;

   ASSERT(pThis != NULL);

   tagClass = berTag_getClass(pThis);
   pContainerSuffix = berTag_isContainer(pThis) ? "!C" : "";

   sprintf(pBuffer, "%s-%u%s", ber_getShortClassName(tagClass), (unsigned int)pThis->number, pContainerSuffix);
}
#endif

bool berTag_equals(const BerTag *pThis, const BerTag *pThat)
{
   ASSERT(pThis != NULL);
   ASSERT(pThat != NULL);

   return pThis->preamble == pThat->preamble
       && pThis->number == pThat->number;
}
