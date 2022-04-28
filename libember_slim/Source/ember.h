/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_SLIM_EMBER_H
#define __LIBEMBER_SLIM_EMBER_H

#include "api.h"
#include "ber.h"
#include "emberasyncreader.h"
#include "emberframing.h"

// ====================================================================
//
// Ember definitions
//
// ====================================================================

/**
  * The EmBER version this library complies to as an uint16.
  * The upper byte is the major version number.
  * The lower byte is the minor version number.
  */
#define EMBER_ENCODING_VERSION (0x0153)


// ====================================================================
//
// Library Initialization
//
// ====================================================================

/**
  * Type of the callback used to signal fatal runtime errors.
  * @param error the error code.
  * @param pMessage pointer to the human-readable error message.
  */
typedef void (*throwError_t)(int error, pcstr pMessage);

/**
  * Type of the callback used to signal runtime assertion failures.
  * @param pFileName the name of the source file where the assertion failed.
  * @param lineNumber the line number where the assertion failed.
  */
typedef void (*failAssertion_t)(pcstr pFileName, int lineNumber);


/**
  * Type of the callback used to allocate memory.
  * @param size number of bytes to allocate.
  */
typedef void *(*allocMemory_t)(size_t size);

/**
  * Type of the callback used to free memory.
  * @param pMemory pointer to the chunk of memory to free.
  */
typedef void (*freeMemory_t)(void *pMemory);


// ====================================================================
//
// Writer functions
//
// ====================================================================

/**
  * Writes the begin TLTL of a container, using the indefinite
  * length form for both Ls.
  * @param pOut pointer to the output to write to.
  * @param pTag pointer to the outer tag of the TLTL.
  * @param type the BerType of the container. The inner tag
  *      of the TLTL is initialized with this value.
  * @note Each call to ember_WriteContainerBegin must be matched
  *      with a later call to ember_WriteContainerEnd.
  */
LIBEMBER_API void ember_writeContainerBegin(BerOutput *pOut, const BerTag *pTag, bertype type);

/**
  * Writes the begin TLTL of a SEQUENCE. Shortcut for
  * ember_WriteContainerBegin(pOut, pTag, BerType_Sequence).
  * The inner T is 'UNIVERSAL BerType_Sequence'.
  * @param pOut pointer to the output to write to.
  * @param pTag pointer to the outer tag of the TLTL.
  * @note Each call to ember_WriteSequenceBegin must be matched
  *      with a later call to ember_WriteContainerEnd.
  */
LIBEMBER_API void ember_writeSequenceBegin(BerOutput *pOut, const BerTag *pTag);

/**
  * Writes the begin TLTL of a SET. Shortcut for
  * ember_WriteContainerBegin(pOut, pTag, BerType_Set).
  * The inner T is 'UNIVERSAL BerType_Set'.
  * @param pOut pointer to the output to write to.
  * @param pTag pointer to the outer tag of the TLTL.
  * @note Each call to ember_WriteSetBegin must be matched
  *      with a later call to ember_WriteContainerEnd.
  */
LIBEMBER_API void ember_writeSetBegin(BerOutput *pOut, const BerTag *pTag);

/**
  * Writes a container terminator.
  * @param pOut pointer to the output to write to.
  * @note Each call to ember_WriteContainerEnd must
  *      be matched with a prior call to ember_WriteContainerBegin.
  */
LIBEMBER_API void ember_writeContainerEnd(BerOutput *pOut);

/**
  * Writes a TLTLV with a boolean value, using the definite
  * length form for both Ls.
  * The inner T is 'UNIVERSAL BerType_Boolean'.
  * @param pOut pointer to the output to write to.
  * @param pTag pointer to the outer tag of the TLTLV.
  * @param value the value to write.
  */
LIBEMBER_API void ember_writeBoolean(BerOutput *pOut, const BerTag *pTag, bool value);

/**
  * Writes a TLTLV with an int32 value, using the definite
  * length form for both Ls.
  * The inner T is 'UNIVERSAL BerType_Integer'.
  * @param pOut pointer to the output to write to.
  * @param pTag pointer to the outer tag of the TLTLV.
  * @param value the value to write.
  */
LIBEMBER_API void ember_writeInteger(BerOutput *pOut, const BerTag *pTag, berint value);

/**
  * Writes a TLTLV with a int64 value, using the definite
  * length form for both Ls.
  * The inner T is 'UNIVERSAL BerType_Integer'.
  * @param pOut pointer to the output to write to.
  * @param pTag pointer to the outer tag of the TLTLV.
  * @param value the value to write.
  */
LIBEMBER_API void ember_writeLong(BerOutput *pOut, const BerTag *pTag, berlong value);

/**
  * Writes a TLTLV with a double value, using the definite
  * length form for both Ls.
  * The inner T is 'UNIVERSAL BerType_Real'.
  * @param pOut pointer to the output to write to.
  * @param pTag pointer to the outer tag of the TLTLV.
  * @param value the value to write.
  */
LIBEMBER_API void ember_writeReal(BerOutput *pOut, const BerTag *pTag, double value);

/**
  * Writes a TLTLV with a zero-terminated string value, using the definite
  * length form for both Ls.
  * The encoding of the string is supposed to be utf-8.
  * The inner T is 'UNIVERSAL BerType_UTF8String'.
  * @param pOut pointer to the output to write to.
  * @param pTag pointer to the outer tag of the TLTLV.
  * @param pValue pointer to the first character of
  *      the zero-terminated string to write.
  */
LIBEMBER_API void ember_writeString(BerOutput *pOut, const BerTag *pTag, pcstr pValue);

/**
  * Writes a TLTLV with an octet string value, using the definite
  * length form for both Ls.
  * The inner T is 'UNIVERSAL BerType_OctetString'.
  * @param pOut pointer to the output to write to.
  * @param pTag pointer to the outer tag of the TLTLV.
  * @param pValue pointer to the first byte of the octet
  *      string to write.
  */
LIBEMBER_API void ember_writeOctetString(BerOutput *pOut, const BerTag *pTag, const byte *pValue, int valueLength);

/**
  * Writes a TLTLV with a RelativeOid value, using the definite
  * length form for both Ls.
  * The inner T is 'UNIVERSAL BerType_RelativeOid'.
  * @param pOut pointer to the output to write to.
  * @param pTag pointer to the outer tag of the TLTLV.
  * @param pValue pointer to the first subidentifier of the RelativeOid.
  */
LIBEMBER_API void ember_writeRelativeOid(BerOutput *pOut, const BerTag *pTag, const berint *pValue, int count);

/**
* Writes a TLTLV with a Null value, using the definite
* length form for both Ls.
* The inner T is 'UNIVERSAL BerType_Null'.
* @param pOut pointer to the output to write to.
* @param pTag pointer to the outer tag of the TLTLV.
*/
LIBEMBER_API void ember_writeNull(BerOutput *pOut, const BerTag *pTag);

#endif
