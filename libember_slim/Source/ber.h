/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_SLIM_BER_H
#define __LIBEMBER_SLIM_BER_H

#include "api.h"
#include "bertypes.h"
#include "bertag.h"
#include "berio.h"
#include "berreader.h"


// ======================================================
//
// Utilities
//
// ======================================================

/**
  * Get the encoded length of @p pTag in bytes.
  * @param pTag pointer to the tag to get the length of.
  * @return the encoded length of the tag in bytes.
  */
LIBEMBER_API int ber_getTagLength(const BerTag *pTag);

/**
  * Get the encoded length of @p value in bytes.
  * @param value value to get the length of.
  * @return the encoded length of the value in bytes.
  */
LIBEMBER_API int ber_getIntegerLength(berint value);

/**
  * Get the encoded length of @p value in bytes.
  * @param value value to get the length of.
  * @return the encoded length of the value in bytes.
  */
LIBEMBER_API int ber_getLongLength(berlong value);

/**
  * Get the encoded length of a tag,length tuple in bytes.
  * @param pTag pointer to the tag.
  * @param length the length of the data prefixed with
  *      the tag,length tuple.
  * @return the encoded length of the tag,length tuple in bytes.
  */
LIBEMBER_API int ber_getHeaderLength(const BerTag *pTag, int length);

/**
  * Get the encoded length of a multi-byte unsigned int32
  * as used in the encoding of ber tags.
  * @param value the unsigned int32 to get the length of.
  * @return the encoded length of the multi-byte integer in bytes.
  */
LIBEMBER_API int ber_getMultiByteIntegerLength(dword value);

/**
  * Get the encoded length of a zero-terminated character string.
  * @param pValue pointer to the first character of the string.
  * @return the encoded length of the string.
  */
LIBEMBER_API int ber_getStringLength(pcstr pValue);

/**
  * Get the encoded length of an integer array encoded as RELATIVE-OID.
  * @param pValue pointer to the first subidentifier of the oid.
  * @return the encoded length of the oid.
  */
LIBEMBER_API int ber_getRelativeOidLength(const berint *pValue, int count);

/**
  * Copies a human-readable name of the passed @p type into the buffer
  * pointed to by @p pBuffer.
  * @param pBuffer pointer to the buffer that receives the typename.
  * @param bufferLength size of the buffer pointed to by @p pBuffer.
  * @param type the ber type to get the name of.
  */
LIBEMBER_API void ber_getTypeName(pstr pBuffer, int bufferLength, bertype type);

/**
  * Gets a pointer to a human readable, single-letter ber class
  * name for the passed @p berClass.
  * @param berClass the ber tag class to get a name for.
  * @return universal:"U", application:"A", etc.
  * @note the returned string must not be freed by the caller.
  */
LIBEMBER_API pcstr ber_getShortClassName(BerClass berClass);


// ====================================================================
//
// Encode functions
// all return the number of bytes in the
// encoded result.
//
// ====================================================================

/**
  * Encodes a ber tag to the passed output.
  * @param pOut pointer to the output to encode to.
  * @param pTag pointer to the tag to encode.
  * @return the number of bytes written to @p pOut.
  */
LIBEMBER_API int ber_encodeTag(BerOutput *pOut, const BerTag *pTag);

/**
  * Encodes a ber length (as in a TLV) to the passed output.
  * @param pOut pointer to the output to encode to.
  * @param value the length to encode.
  * @return the number of bytes written to @p pOut.
  */
LIBEMBER_API int ber_encodeLength(BerOutput *pOut, int value);

/**
  * Encodes a multi-byte unsigned int32 to the passed output.
  * @param pOut pointer to the output to encode to.
  * @param value the unsigned int32 to encode.
  * @return the number of bytes written to @p pOut.
  */
LIBEMBER_API int ber_encodeMultiByteInteger(BerOutput *pOut, dword value);

/**
  * Encodes a boolean value to the passed output.
  * @param pOut pointer to the output to encode to.
  * @param value the boolean value to encode.
  * @return the number of bytes written to @p pOut.
  */
LIBEMBER_API int ber_encodeBoolean(BerOutput *pOut, bool value);

/**
  * Encodes an int32 value to the passed output.
  * @param pOut pointer to the output to encode to.
  * @param value the int32 value to encode.
  * @param length the number of bytes to write.
  *      Can be determined by calling ber_getIntegerLength.
  *      prior to calling this function.
  * @return the number of bytes written to @p pOut.
  */
LIBEMBER_API int ber_encodeInteger(BerOutput *pOut, berint value, int length);

/**
  * Encodes a int64 value to the passed output.
  * @param pOut pointer to the output to encode to.
  * @param value the int64 value to encode.
  * @param length the number of bytes to write.
  *      Can be determined by calling ber_getLongLength.
  *      prior to calling this function.
  * @return the number of bytes written to @p pOut.
  */
LIBEMBER_API int ber_encodeLong(BerOutput *pOut, berlong value, int length);

/**
  * Encodes a floating-point value to the passed output.
  * @param pOut pointer to the output to encode to.
  * @param value the floating-point value to encode.
  * @return the number of bytes written to @p pOut.
  */
LIBEMBER_API int ber_encodeReal(BerOutput *pOut, double value);

/**
  * Encodes a zero-terminated string to the passed output.
  * @param pOut pointer to the output to encode to.
  * @param value the zero-terminated string to encode.
  * @return the number of bytes written to @p pOut.
  */
LIBEMBER_API int ber_encodeString(BerOutput *pOut, pcstr pValue);

/**
  * Encodes an octet string value to the passed output.
  * @param pOut pointer to the output to encode to.
  * @param pValue pointer to the first byte in the octet
  *      string to encode.
  * @param length number of bytes in the octet string
  *     pointed to by @p pValue.
  * @return the number of bytes written to @p pOut.
  */
LIBEMBER_API int ber_encodeOctetString(BerOutput *pOut, const byte *pValue, int length);

/**
  * Encodes an relative oid value to the passed output.
  * @param pOut pointer to the output to encode to.
  * @param pValue the relative oid represented by an array of
  *     subidentifiers (integers).
  * @param count number of subidentifiers at @p pValue.
  * @return the number of bytes written to @p pOut.
  */
LIBEMBER_API int ber_encodeRelativeOid(BerOutput *pOut, const berint *pValue, int count);

/**
 * Encodes a null value to the passed output.
 * @param pOut pointer to the output to encode to.
 * @return the number of bytes written to @p pOut.
 */
LIBEMBER_API int ber_encodeNull(BerOutput *pOut);


// ====================================================================
//
// Decode functions
//
// ====================================================================

/**
  * Decodes a ber tag from the passed input.
  * @param pIn pointer to the input to decode from.
  * @return the decoded tag.
  */
LIBEMBER_API BerTag ber_decodeTag(BerInput *pIn);

/**
  * Decodes a ber length (as in a TLV) from the passed input.
  * @param pIn pointer to the input to decode from.
  * @return the decoded length.
  */
LIBEMBER_API int ber_decodeLength(BerInput *pIn);

/**
  * Decodes a multi-byte unsigned int32 from the passed input.
  * @param pIn pointer to the input to decode from.
  * @param pConsumedBytesCount pointer to an integer receiving
  *     the number of bytes consumed. May be NULL.
  * @return the decoded multi-byte integer.
  */
LIBEMBER_API dword ber_decodeMultiByteInteger(BerInput *pIn, int *pConsumedBytesCount);

/**
  * Decodes a boolean value from the passed input.
  * @param pIn pointer to the input to decode from.
  * @return the decoded boolean value.
  */
LIBEMBER_API bool ber_decodeBoolean(BerInput *pIn);

/**
  * Decodes an int32 value from the passed input.
  * @param pIn pointer to the input to decode from.
  * @param length the number of bytes that make up
  *     the encoding.
  * @return the decoded integer.
  */
LIBEMBER_API berint ber_decodeInteger(BerInput *pIn, int length);

/**
  * Decodes an int64 value from the passed input.
  * @param pIn pointer to the input to decode from.
  * @param length the number of bytes that make up
  *     the encoding.
  * @return the decoded integer.
  */
LIBEMBER_API berlong ber_decodeLong(BerInput *pIn, int length);

/**
  * Decodes a floating-point value from the passed input.
  * @param pIn pointer to the input to decode from.
  * @param length the number of bytes that make up
  *     the encoding.
  * @return the decoded floating-point value.
  */
LIBEMBER_API double ber_decodeReal(BerInput *pIn, int length);

/**
  * Decodes a character string from the passed input into
  * a zero-terminated character array.
  * @param pIn pointer to the input to decode from.
  * @param pDest pointer to the buffer to hold the zero-terminated
  *     string.
  * @param length the number of bytes that make up
  *     the encoding.
  *     @p pDest must point to a buffer big enough to hold
  *     this number of bytes.
  */
LIBEMBER_API void ber_decodeString(BerInput *pIn, pstr pDest, int length);

/**
  * Decodes an octet string from the passed input.
  * @param pIn pointer to the input to decode from.
  * @param pDest pointer to the buffer to hold the octets.
  * @param length the number of bytes that make up
  *     the encoding.
  *     @p pDest must point to a buffer big enough to hold
  *     this number of bytes.
  */
LIBEMBER_API void ber_decodeOctetString(BerInput *pIn, byte *pDest, int length);

/**
  * Decodes a relative oid value from the passed input.
  * @param pIn pointer to the input to decode from.
  * @param pDest pointer to an integer array to store the
  *     decoded subidentifiers to.
  * @param destSize maximum number of subidentifiers to decode.
  * @param length the number of bytes that make up
  *     the encoding.
  * @return the number of decoded subidentifiers.
  */
LIBEMBER_API int ber_decodeRelativeOid(BerInput *pIn, berint *pDest, int destSize, int length);

/**
* Decodes a null value from the passed input.
* @param pIn pointer to the input to decode from.
*/
LIBEMBER_API void ber_decodeNull(BerInput *pIn);

#endif
