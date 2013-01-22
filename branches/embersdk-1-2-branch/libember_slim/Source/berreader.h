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

#ifndef __LIBEMBER_SLIM_BERREADER_H
#define __LIBEMBER_SLIM_BERREADER_H

#include "bertypes.h"
#include "bytebuffer.h"
#include "bertag.h"

/**
  * Defines a basic reader to decode in-memory ember TLTLVs.
  * When aggregated, the aggregating type must provide
  * functions to advance the reader.
  * @note a TLTLV is a tuple of (tag, outer length, type, length)
  */
typedef struct SBerReader
{
   /**
     * The tag of the current TLTLV.
     */
   BerTag tag;

   /**
     * The length of the current TLTLV.
     */
   int length;

   /**
     * The type of the current TLTLV.
     */
   bertype type;

   /**
     * If true, the current TLTLV is a BER container.
     */
   bool isContainer;

   /**
     * The outer length of the current TLTLV.
     */
   int outerLength;

   /**
     * Buffer containing the encoded value of the current TLTLV.
     */
   ByteBuffer buffer;
} BerReader;

/**
  * Initializes a BerReader instance.
  * Must be called before any other operations on the
  * BerReader instance are invoked.
  * @param pThis pointer to the object to process.
  */
void berReader_init(BerReader *pThis);

/**
  * Frees all memory allocated by a BerReader instance.
  * BerReader instance are invoked.
  * @param pThis pointer to the object to process.
  */
void berReader_free(BerReader *pThis);

/**
  * Resets a BerReader instance.
  * Clears all fields except the buffer.
  * @param pThis pointer to the object to process.
  */
void berReader_reset(BerReader *pThis);

/**
  * Gets the value of the current TLTLV as a boolean.
  * Expected type is BerType_Boolean.
  * @param pThis pointer to the object to process.
  * @return the decoded value.
  */
bool berReader_getBoolean(const BerReader *pThis);

/**
  * Gets the value of the current TLTLV as an int32.
  * Expected type is BerType_Integer.
  * @param pThis pointer to the object to process.
  * @return the decoded value.
  */
berint berReader_getInteger(const BerReader *pThis);

/**
  * Gets the value of the current TLTLV as a int64.
  * Expected type is BerType_Integer.
  * @param pThis pointer to the object to process.
  * @return the decoded value.
  */
berlong berReader_getLong(const BerReader *pThis);

/**
  * Gets the value of the current TLTLV as a double.
  * Expected type is BerType_Real.
  * @param pThis pointer to the object to process.
  * @return the decoded value.
  */
double berReader_getReal(const BerReader *pThis);

/**
  * Gets the value of the current TLTLV as a zero-terminated utf-8 string.
  * Expected type is BerType_UTF8String.
  * @param pThis pointer to the object to process.
  * @param pDest pointer to the buffer to receive the string.
  * @param size number of bytes available at @p pDest. A maximum
  *     of @p size characters will be stored at @p pDest.
  * @note Currently libember_slim only supports the ASCII character
  *     range (0 through 127).
  */
void berReader_getString(const BerReader *pThis, pstr pDest, int size);

/**
  * Gets the value of the current TLTLV as a byte array.
  * Expected type is BerType_OctetString.
  * @param pThis pointer to the object to process.
  * @param pDest pointer to the buffer to receive the octet string.
  * @param size number of bytes available at @p pDest. A maximum
  *     of @p size characters will be stored at @p pDest.
  * @return number of decoded octets.
  */
int berReader_getOctetString(const BerReader *pThis, byte *pDest, int size);

/**
  * Gets the value of the current TLTLV as a an array of integers,
  * each integer containing one subidentifier.
  * Expected type is BerType_RelativeOid.
  * @param pThis pointer to the object to process.
  * @param pDest pointer to an integer array to store the
  *     decoded subidentifiers to.
  * @param destSize maximum number of subidentifiers to decode.
  * @return number of decoded subidentifiers.
  * @note This function slightly bends the ASN.1 standard and
  *     is subject to be removed.
  */
int berReader_getRelativeOid(const BerReader *pThis, berint *pDest, int destSize);

#endif
