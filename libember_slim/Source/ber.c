/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <stdio.h>
#include <string.h>
#include <float.h>
#include "ber.h"
#include "emberinternal.h"


// ====================================================================
//
// Utility functions
//
// ====================================================================

static const pcstr _typeNames[] =
{
   NULL,

   "Boolean",
   "Integer",
   "Bitstring",
   "OctetString",
   "Null",
   "ObjectIdentifier",
   "ObjectDescriptor",
   "External",
   "Real",
   "Enumerated",
   "EmbeddedPdv",
   "UTF8",
   "RelativeOid",

   NULL,
   NULL,

   "Sequence",
   "Set",
   "NumericString",
   "PrintableString",
   "TeletexString",
   "VideotexString",
   "IA5String",
   "UTCTime",
   "GeneralizedTime",
   "GraphicString",
   "VisibleString",
   "GeneralString",
   "UniversalString",
   "UnspecifiedString",
   "BMPString",
};

static const pcstr _shortClassNames[] =
{
   "U",
   "A",
   "C",
   "P",
};

void ber_getTypeName(pstr pBuffer, int bufferLength, bertype type)
{
   ASSERT(pBuffer != NULL);

   if(IsApplicationDefinedBerType(type))
   {
#ifdef SECURE_CRT
      sprintf_s(pBuffer, bufferLength, "APPLICATION-%d", (int)(type & ~BerType_ApplicationFlag));
#else
      sprintf(pBuffer, "APPLICATION-%d", (int)(type & ~BerType_ApplicationFlag));
#endif
   }
   else if(type < BerType_LastUniversal)
   {
#ifdef SECURE_CRT
      strncpy_s(pBuffer, bufferLength, _typeNames[type], bufferLength - 1);
      pBuffer[bufferLength - 1] = 0;
#else
      strncpy(pBuffer, _typeNames[type], bufferLength);
      pBuffer[bufferLength - 1] = 0;
#endif
   }
   else
   {
      *pBuffer = 0;
   }
}

pcstr ber_getShortClassName(BerClass berClass)
{
   return _shortClassNames[(berClass & BerClass_Mask) >> 6];
}

int ber_getTagLength(const BerTag *pTag)
{
   tagnumber number = pTag->number;

   if(number < 0x1F)
      return 1;

   return 1 + ber_getMultiByteIntegerLength(number);
}

int ber_getIntegerLength(berint value)
{
   dword dwValue = (dword)value;
   dword mask = 0xFF800000L;
   int length = 4;
   dword topBitMask;

   if(value < 0)
   {
      for( ; (dwValue & mask) == mask && length > 1; mask >>= 8)
         length--;
   }
   else
   {
      topBitMask = 0x80000000L;

      for( ; (dwValue & mask) == 0 && length > 1; mask >>= 8)
      {
         topBitMask >>= 8;
         length--;
      }

      if(dwValue & topBitMask)
         length++; // include leading zero octet to signal positive value
   }

   return length;
}

static int getLongLength(berlong value, bool isSigned)
{
   qword qwValue = (qword)value;
   qword mask = isSigned
                ? 0xFF80000000000000ULL
                : 0xFF00000000000000ULL;
   int length = 8;
   qword topBitMask;

   if(value < 0)
   {
      for( ; (qwValue & mask) == mask && length > 1; mask >>= 8)
         length--;
   }
   else
   {
      topBitMask = 0x8000000000000000ULL;

      for( ; (qwValue & mask) == 0 && length > 1; mask >>= 8)
      {
         topBitMask >>= 8;
         length--;
      }

      if((qwValue & topBitMask) != 0 && isSigned)
         length++;
   }

   return length;
}

int ber_getLongLength(berlong value)
{
   return getLongLength(value, true);
}

int ber_getMultiByteIntegerLength(dword value)
{
   int size = 1;

   if(value & 0xF0000000UL) // most significant 4 bits
      size++;

   if(value & 0xFFE00000UL) // most significant 11 bits
      size++;

   if(value & 0xFFFFC000UL) // most significant 18 bits
      size++;

   if(value & 0xFFFFFF80UL) // most significant 25 bits
      size++;

   return size;
}

int ber_getStringLength(pcstr pValue)
{
   if(pValue == NULL)
      return 0;

   return (int)(strlen(pValue));
}

int ber_getHeaderLength(const BerTag *pTag, int length)
{
   int result = ber_getTagLength(pTag);

   if(length <= 0x7F || length == BER_INDEFINITE_LENGTH)
      return result + 1;

   return result + 1 + ber_getIntegerLength(length);
}

int ber_getRelativeOidLength(const berint *pValue, int count)
{
   int size = 0;

   for( ; count > 0; count--, pValue++)
      size += ber_getMultiByteIntegerLength((dword)*pValue);

   return size;
}


// ====================================================================
//
// Encode functions
// all return the number of bytes in the
// encoded result.
//
// ====================================================================

int ber_encodeTag(BerOutput *pOut, const BerTag *pTag)
{
   tagnumber number = pTag->number;
   byte preamble = pTag->preamble & 0xE0;
   int size = 1;

   if(number < 0x1F)
   {
      pOut->writeByte(pOut, (byte)(preamble | (number & 0x1F)));
   }
   else
   {
      pOut->writeByte(pOut, (byte)(preamble | 0x1F));

      size += ber_encodeMultiByteInteger(pOut, number);
   }

   return size;
}

int ber_encodeMultiByteInteger(BerOutput *pOut, dword value)
{
   int nSize = 1;

   if(value & 0xF0000000UL) // most significant 4 bits
   {
      pOut->writeByte(pOut, (byte)(0x80 | ((value >> 28) & 0x0F)));
      nSize++;
   }

   if(value & 0xFFE00000UL) // most significant 11 bits
   {
      pOut->writeByte(pOut, (byte)(0x80 | ((value >> 21) & 0x7F)));
      nSize++;
   }

   if(value & 0xFFFFC000UL) // most significant 18 bits
   {
      pOut->writeByte(pOut, (byte)(0x80 | ((value >> 14) & 0x7F)));
      nSize++;
   }

   if(value & 0xFFFFFF80UL) // most significant 25 bits
   {
      pOut->writeByte(pOut, (byte)(0x80 | ((value >> 7) & 0x7F)));
      nSize++;
   }

   pOut->writeByte(pOut, (byte)(0x00 | ((value >> 0) & 0x7F)));

   return nSize;
}

int ber_encodeLength(BerOutput *pOut, int value)
{
   int size = 1;
   int integerLength;

   if(value == BER_INDEFINITE_LENGTH)
   {
      pOut->writeByte(pOut, 0x80);
   }
   else
   {
      if(value <= 0x7F)
      {
         pOut->writeByte(pOut, (byte)(value & 0x7F));
      }
      else
      {
         integerLength = ber_getIntegerLength(value);
         pOut->writeByte(pOut, (byte)(0x80 | integerLength));

         size += ber_encodeInteger(pOut, value, integerLength);
      }
   }

   return size;
}

// value encodings
int ber_encodeBoolean(BerOutput *pOut, bool value)
{
   pOut->writeByte(pOut, value ? 0xFF : 0);
   return 1;
}

int ber_encodeInteger(BerOutput *pOut, berint value, int length)
{
   dword dwValue = (dword)value;
   dword bits = length * 8;

   while(bits > 0)
   {
      bits -= 8;

      pOut->writeByte(pOut, (byte)((dwValue >> bits) & 0xFF));
   }

   return length;
}

int ber_encodeLong(BerOutput *pOut, berlong value, int length)
{
   qword qwValue;
   int bits;

   ASSERT(length > 0 && length <= 8);

   qwValue = (qword)value;
   bits = length * 8;

   while(bits > 0)
   {
      bits -= 8;

      pOut->writeByte(pOut, (byte)((qwValue >> bits) & 0xFF));
   }

   return length;
}

// max encoded length: 17
int ber_encodeReal(BerOutput *pOut, double value)
{
   int size = 0;
   berlong exponent;
   berlong mantissa;
   int exponentLength;
   byte preamble;

#if DOUBLE_LENGTH == 8
   // this is a replacement for
   // qword qwValue = *(qword *)&value;
   // to avoid strict-aliasing warning
   union
   {
      double d;
      qword qw;
   } conversion;
   qword qwValue;

   conversion.d = value;
   qwValue = conversion.qw;

   if(qwValue != 0)
   {
      if(qwValue == 0x7FF0000000000000LL) // positive infinity
      {
         pOut->writeByte(pOut, 0x40); // 01000000 Value is PLUS-INFINITY
         size = 1;
      }
      else if(qwValue == 0x8FF0000000000000LL) // negative infinity
      {
         pOut->writeByte(pOut, 0x41); // 01000001 Value is MINUS-INFINITY
         size = 1;
      }
      else
      {
         exponent = ((0x7FF0000000000000LL & qwValue) >> 52) - 1023;
         mantissa = 0x000FFFFFFFFFFFFFLL & qwValue;
         exponentLength = ber_getLongLength(exponent);

         mantissa |= 0x10000000000000LL; // set virtual delimeter

         // normalize mantissa (required by CER and DER)
         while((mantissa & 0xFF) == 0)
            mantissa >>= 8;

         while((mantissa & 0x01) == 0)
            mantissa >>= 1;

         preamble = 0x80;
         preamble |= (byte)(exponentLength - 1);

         if((qwValue & 0x8000000000000000LL) != 0)
            preamble |= 0x40; // Sign

         pOut->writeByte(pOut, preamble);

         size++;
         size += ber_encodeLong(pOut, exponent, exponentLength);
         size += ber_encodeLong(pOut, mantissa, getLongLength(mantissa, false));
      }
   }
#elif DOUBLE_LENGTH == 4
   // this is a replacement for
   // dword dwValue = *(dword *)&value;
   // to avoid strict-aliasing warning
   union
   {
      double d;
      dword dw;
   } conversion;
   dword dwValue;

   conversion.d = value;
   dwValue = conversion.dw;

   if(dwValue != 0)
   {
      if(dwValue == 0x7FF00000L) // positive infinity
      {
         pOut->writeByte(pOut, 0x40); // 01000000 Value is PLUS-INFINITY
         size = 1;
      }
      else if(dwValue == 0x8FF00000L) // negative infinity
      {
         pOut->writeByte(pOut, 0x41); // 01000001 Value is MINUS-INFINITY
         size = 1;
      }
      else
      {
         exponent = ((0x7F800000L & dwValue) >> 23) - 127;
         mantissa = 0x007FFFFFL & dwValue;
         exponentLength = ber_getLongLength(exponent);

         mantissa |= 0x00800000L; // set virtual delimeter

         // normalize mantissa (required by CER and DER)
         while((mantissa & 0xFF) == 0)
            mantissa >>= 8;

         while((mantissa & 0x01) == 0)
            mantissa >>= 1;

         preamble = 0x80;
         preamble |= (byte)(exponentLength - 1);

         if((dwValue & 0x80000000L) != 0)
            preamble |= 0x40; // Sign

         pOut->writeByte(pOut, preamble);

         size++;
         size += ber_encodeLong(pOut, exponent, exponentLength);
         size += ber_encodeLong(pOut, mantissa, getLongLength(mantissa, false));
      }
   }
#else
#error DOUBLE_LENGTH must be either 8 or 4!
#endif

   return size;
}

int ber_encodeString(BerOutput *pOut, pcstr pValue)
{
   int length = ber_getStringLength(pValue);

   if(length > 0 && pValue != NULL)
      pOut->writeBytes(pOut, (const byte *)pValue, length);

   return length;
}

int ber_encodeOctetString(BerOutput *pOut, const byte *pValue, int length)
{
   pOut->writeBytes(pOut, pValue, length);

   return length;
}

int ber_encodeRelativeOid(BerOutput *pOut, const berint *pValue, int count)
{
   int size = 0;

   for( ; count > 0; count--, pValue++)
      size += ber_encodeMultiByteInteger(pOut, (dword)*pValue);

   return size;
}

int ber_encodeNull(BerOutput *pOut)
{
    (void)pOut;
    return 0;
}


// ====================================================================
//
// Decode functions
//
// ====================================================================

BerTag ber_decodeTag(BerInput *pIn)
{
   byte tagCode = pIn->readByte(pIn);

   BerTag tag;
   tag.preamble = tagCode & ~0x1F;
   tag.number = tagCode & 0x1F;

   if(tag.number == 0x1F)
      tag.number = ber_decodeMultiByteInteger(pIn, NULL);

   return tag;
}

int ber_decodeLength(BerInput *pIn)
{
   int value;
   int byteCount;

   value = pIn->readByte(pIn);

   if(value & 0x80)
   {
      byteCount = value & 0x7F;

      if(byteCount == 0)
      {
         value = BER_INDEFINITE_LENGTH; // indefinite length form
      }
      else
      {
         value = 0;

         for( ; byteCount > 0; byteCount--)
            value = (value << 8) | pIn->readByte(pIn);
      }
   }

   return value;
}

dword ber_decodeMultiByteInteger(BerInput *pIn, int *pConsumedBytesCount)
{
   byte b;
   dword value = 0;
   int count = 0;

   do
   {
      b = pIn->readByte(pIn);
      value = (value << 7) | (b & ~0x80);
      count++;

   } while(b & 0x80);

   if(pConsumedBytesCount != NULL)
      *pConsumedBytesCount = count;

   return value;
}

qword ber_decodeMultiByteLong(BerInput *pIn)
{
   byte b;
   qword value = 0;

   do
   {
      b = pIn->readByte(pIn);
      value = (value << 7) | (b & ~0x80);

   } while(b & 0x80);

   return value;
}

bool ber_decodeBoolean(BerInput *pIn)
{
   return pIn->readByte(pIn) != 0;
}

berint ber_decodeInteger(BerInput *pIn, int length)
{
   berint value = 0;
   int b;
   int count;

   ASSERT(length >= 0 && length <= 4);

   for(count = 0; count < length; count++)
   {
      b = pIn->readByte(pIn);

      if(count == 0 && (b & 0x80))
         b -= 0x100; // negative value, invert upper 3 bytes

      value = (value << 8) | b;
   }

   return value;
}

static berlong decodeLong(BerInput *pIn, int length, bool isSigned)
{
   berlong value = 0;
   berlong b;
   int count;

   ASSERT(length >= 0 && length <= 8);

   for(count = 0; count < length; count++)
   {
      b = pIn->readByte(pIn);

      if(count == 0
      && (b & 0x80) != 0
      && isSigned)
         b -= 0x100; // negative value, invert upper 7 bytes

      value = (value << 8) | b;
   }

   return value;
}

berlong ber_decodeLong(BerInput *pIn, int length)
{
   return decodeLong(pIn, length, true);
}

double ber_decodeReal(BerInput *pIn, int length)
{
   double value = 0;
   byte preamble;
   int exponentLength;
   int sign;
   int ff;
   berlong exponent;
   berlong mantissa;

#if DOUBLE_LENGTH == 8
   qword qwValue;
   union
   {
      double d;
      qword qw;
   } conversion;

   if(length != 0)
   {
      preamble = pIn->readByte(pIn);

      qwValue = 0;

      if(length == 1 )
      {
          if ( preamble == 0x40) // positive infinity
              qwValue = 0x7FF0000000000000ULL;
          else if ( preamble == 0x41) // positive infinity
              qwValue = 0x8FF0000000000000ULL;
          else
              qwValue = 0x7fC0000000000000ULL;
      }
      else
      {
         exponentLength = 1 + (preamble & 3);
         sign = preamble & 0x40;
         ff = (preamble >> 2) & 3;

         // Unpack mantissa & decrement exponent for base 2
         exponent = decodeLong(pIn, exponentLength, true);
         mantissa = decodeLong(pIn, length - exponentLength - 1, false) << ff;

         // de-normalize mantissa (required by CER and DER)
         while((mantissa & 0x7FFFF00000000000LL) == 0x0)
            mantissa <<= 8;

         while((mantissa & 0x7FF0000000000000LL) == 0x0)
            mantissa <<= 1;

         mantissa &= 0x0FFFFFFFFFFFFFLL;

         qwValue = (exponent + 1023) << 52;
         qwValue |= mantissa;

         if(sign != 0)
            qwValue |= 0x8000000000000000LL;
      }

      // this is a replacement for
      // value = *(double *)&qwValue;
      // to avoid strict-aliasing warning
      conversion.qw = qwValue;
      value = conversion.d;
   }
#elif DOUBLE_LENGTH == 4
   dword dwValue;
   union
   {
      double d;
      dword dw;
   } conversion;

   if(length != 0)
   {
      preamble = pIn->readByte(pIn);

      dwValue = 0;

      if(length == 1 )
      {
          if ( preamble == 0x40) // positive infinity
              qwValue = 0x7FF00000L;
          else if ( preamble == 0x41) // positive infinity
              qwValue = 0x8FF00000L;
          else
              qwValue = 0x7fC00000L;
      }
      else
      {
         exponentLength = 1 + (preamble & 3);
         sign = preamble & 0x40;
         ff = (preamble >> 2) & 3;

         // Unpack mantissa & decrement exponent for base 2
         exponent = decodeLong(pIn, exponentLength, true);
         mantissa = decodeLong(pIn, length - exponentLength - 1, false) << ff;

         // de-normalize mantissa (required by CER and DER)
         while((mantissa & 0x0FFF0000L) == 0)
            mantissa <<= 8;

         while((mantissa & 0x00800000L) == 0)
            mantissa <<= 1;

         mantissa &= 0x7FFFFFL;

         dwValue = (dword)(((exponent + 127) << 23) & 0xFFFFFFFFL);
         dwValue |= mantissa;

         if(sign != 0)
            dwValue |= 0x80000000L;
      }

      // this is a replacement for
      // value = *(double *)&dwValue;
      // to avoid strict-aliasing warning
      conversion.dw = dwValue;
      value = conversion.d;
   }
#else
#error DOUBLE_LENGTH must be either 8 or 4!
#endif

   return value;
}

void ber_decodeString(BerInput *pIn, pstr pDest, int length)
{
   ASSERT(pDest != NULL);

   for( ; length > 0; length--, pDest++)
      *pDest = pIn->readByte(pIn);

   *pDest = 0;
}

void ber_decodeOctetString(BerInput *pIn, byte *pDest, int length)
{
   ASSERT(pDest != NULL);

   for( ; length > 0; length--, pDest++)
      *pDest = pIn->readByte(pIn);
}

int ber_decodeRelativeOid(BerInput *pIn, berint *pDest, int destSize, int length)
{
   int byteCount;
   int subidCount;

   for(subidCount = 0; length > 0 && subidCount < destSize; subidCount++)
   {
      *pDest = ber_decodeMultiByteInteger(pIn, &byteCount);

      pDest++;
      length -= byteCount;
   }

   return subidCount;
}

void ber_decodeNull(BerInput *pIn)
{
    (void)pIn;
}

