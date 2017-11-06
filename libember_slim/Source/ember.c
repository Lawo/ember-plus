/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <string.h>
#include "ember.h"
#include "emberplus.h"
#include "emberinternal.h"


// ====================================================================
//
// Locals
//
// ====================================================================

static const byte _indefiniteLengthTerminator[] = { 0, 0, 0, 0 };

static void writeTypeTag(BerOutput *pOut, bertype type, bool isContainer)
{
   BerTag tag;
   BerClass tagClass = IsApplicationDefinedBerType(type)
                       ? BerClass_Application
                       : BerClass_Universal;

   berTag_init(&tag, tagClass, type & ~BerType_ApplicationFlag);

   if(isContainer)
      tag.preamble |= BER_CONTAINER_FLAG;

   ber_encodeTag(pOut, &tag);
}

static void writeOuterHeader(BerOutput *pOut, const BerTag *pTag, int length)
{
   BerTag tag = berTag_toContainer(pTag);
   ber_encodeTag(pOut, &tag);
   ber_encodeLength(pOut, length);
}


// ====================================================================
//
// Internals
//
// ====================================================================

extern throwError_t _pThrowError;
extern failAssertion_t _pFailAssertion;
extern allocMemory_t _pAllocMemory;
extern freeMemory_t _pFreeMemory;


// ====================================================================
//
// Globals
//
// ====================================================================

void ember_init(throwError_t throwError, failAssertion_t failAssertion, allocMemory_t allocMemory, freeMemory_t freeMemory)
{
   _pThrowError = throwError;
   _pFailAssertion = failAssertion;
   _pAllocMemory = allocMemory;
   _pFreeMemory = freeMemory;
}

void ember_writeContainerBegin(BerOutput *pOut, const BerTag *pTag, bertype type)
{
   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);

   writeOuterHeader(pOut, pTag, BER_INDEFINITE_LENGTH);
   writeTypeTag(pOut, type, true);
   ber_encodeLength(pOut, BER_INDEFINITE_LENGTH);
}

void ember_writeSequenceBegin(BerOutput *pOut, const BerTag *pTag)
{
   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);

   ember_writeContainerBegin(pOut, pTag, BerType_Sequence);
}

void ember_writeSetBegin(BerOutput *pOut, const BerTag *pTag)
{
   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);

   ember_writeContainerBegin(pOut, pTag, BerType_Set);
}

void ember_writeContainerEnd(BerOutput *pOut)
{
   ASSERT(pOut != NULL);

   pOut->writeBytes(pOut, _indefiniteLengthTerminator, sizeof(_indefiniteLengthTerminator));
}

void ember_writeBoolean(BerOutput *pOut, const BerTag *pTag, bool value)
{
   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);

   writeOuterHeader(pOut, pTag, 3);
   writeTypeTag(pOut, BerType_Boolean, false);
   ber_encodeLength(pOut, 1);
   ber_encodeBoolean(pOut, value);
}

void ember_writeInteger(BerOutput *pOut, const BerTag *pTag, berint value)
{
   int valueLength = ber_getIntegerLength(value);

   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);

   writeOuterHeader(pOut, pTag, valueLength + 2);
   writeTypeTag(pOut, BerType_Integer, false);
   ber_encodeLength(pOut, valueLength);
   ber_encodeInteger(pOut, value, valueLength);
}

void ember_writeLong(BerOutput *pOut, const BerTag *pTag, berlong value)
{
   int valueLength = ber_getLongLength(value);

   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);

   writeOuterHeader(pOut, pTag, valueLength + 2);
   writeTypeTag(pOut, BerType_Integer, false);
   ber_encodeLength(pOut, valueLength);
   ber_encodeLong(pOut, value, valueLength);
}

void ember_writeReal(BerOutput *pOut, const BerTag *pTag, double value)
{
   BerMemoryOutput out;
   byte buffer[20];
   int valueLength;

   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);

   berMemoryOutput_init(&out, buffer, sizeof(buffer));

   valueLength = ber_encodeReal(&out.base, value);
   writeOuterHeader(pOut, pTag, valueLength + 2);
   writeTypeTag(pOut, BerType_Real, false);
   ber_encodeLength(pOut, valueLength);
   pOut->writeBytes(pOut, out.pMemory, valueLength);
}

void ember_writeString(BerOutput *pOut, const BerTag *pTag, pcstr pValue)
{
   int valueLength;
   BerTag innerTag;

   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);
   ASSERT(pValue != NULL);

   valueLength = (int)strlen(pValue);
   berTag_init(&innerTag, BerClass_Universal, BerType_UTF8String);

   writeOuterHeader(pOut, pTag, valueLength + ber_getHeaderLength(&innerTag, valueLength));
   ber_encodeTag(pOut, &innerTag);
   ber_encodeLength(pOut, valueLength);
   pOut->writeBytes(pOut, (const byte *)pValue, valueLength);
}

void ember_writeOctetString(BerOutput *pOut, const BerTag *pTag, const byte *pValue, int valueLength)
{
   BerTag innerTag;

   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);
   ASSERT(pValue != NULL);

   berTag_init(&innerTag, BerClass_Universal, BerType_OctetString);

   writeOuterHeader(pOut, pTag, valueLength + ber_getHeaderLength(&innerTag, valueLength));
   ber_encodeTag(pOut, &innerTag);
   ber_encodeLength(pOut, valueLength);
   pOut->writeBytes(pOut, pValue, valueLength);
}

void ember_writeRelativeOid(BerOutput *pOut, const BerTag *pTag, const berint *pValue, int count)
{
   int valueLength;
   BerTag innerTag;

   ASSERT(pOut != NULL);
   ASSERT(pTag != NULL);
   ASSERT(pValue != NULL || count == 0);

   valueLength = ber_getRelativeOidLength(pValue, count);
   berTag_init(&innerTag, BerClass_Universal, BerType_RelativeOid);

   writeOuterHeader(pOut, pTag, valueLength + ber_getHeaderLength(&innerTag, valueLength));
   ber_encodeTag(pOut, &innerTag);
   ber_encodeLength(pOut, valueLength);
   ber_encodeRelativeOid(pOut, pValue, count);
}

void ember_writeNull(BerOutput *pOut, const BerTag *pTag)
{
    BerTag innerTag;

    ASSERT(pOut != NULL);
    ASSERT(pTag != NULL);

    berTag_init(&innerTag, BerClass_Universal, BerType_Null);
    writeOuterHeader(pOut, pTag, 0 + ber_getHeaderLength(&innerTag, 0));
    ber_encodeTag(pOut, &innerTag);
    ber_encodeLength(pOut, 0);
    ber_encodeNull(pOut);
}
