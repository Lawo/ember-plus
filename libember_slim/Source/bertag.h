/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_SLIM_BERTAG_H
#define __LIBEMBER_SLIM_BERTAG_H

#include "api.h"
#include "bertypes.h"

// ======================================================
//
// BER types
//
// ======================================================

/**
  * The pre-defined ber tag classes.
  */
typedef enum EBerClass
{
   BerClass_Universal         = 0x00,     // Predefined Types for BER ( do not use! )
   BerClass_Application       = 0x40,     // Application specific Tags that have the same meaning wherever seen and used.
   BerClass_ContextSpecific   = 0x80,     // Tags that have a meaning specific to the location where seen and used.
   BerClass_Private           = 0xC0,     // Special version of Context Specific Tag. 

   BerClass_Mask              = 0xC0,
} BerClass;

/**
  * The pre-defined asn.1/ber types.
  */
typedef enum EBerType
{
   BerType_Invalid             = 0,
   BerType_Boolean             = 1,
   BerType_Integer             = 2,
   BerType_Bitstring           = 3,
   BerType_OctetString         = 4,
   BerType_Null                = 5,
   BerType_ObjectIdentifier    = 6,
   BerType_ObjectDescriptor    = 7,
   BerType_External            = 8,
   BerType_Real                = 9,
   BerType_Enumerated          = 10,
   BerType_EmbeddedPdv         = 11,
   BerType_UTF8String          = 12,
   BerType_RelativeOid         = 13,

   BerType_Sequence            = 16,
   BerType_Set                 = 17,
   BerType_NumericString       = 18,
   BerType_PrintableString     = 19,
   BerType_TeletexString       = 20,
   BerType_VideotexString      = 21,
   BerType_IA5String           = 22,
   BerType_UTCTime             = 23,
   BerType_GeneralizedTime     = 24,
   BerType_GraphicString       = 25,
   BerType_VisibleString       = 26,
   BerType_GeneralString       = 27,
   BerType_UniversalString     = 28,
   BerType_UnspecifiedString   = 29,
   BerType_BMPString           = 30,

   BerType_LastUniversal       = 31,

   BerType_ApplicationFlag     = 0x8000,
} BerType;


/**
  * Preprocessor macro to determine whether the passed type is application-defined or not.
  * @param dwType A bertype value.
  */
#define IsApplicationDefinedBerType(dwType) (((dwType) & BerType_ApplicationFlag) != 0)

/**
  * The ber container flag used in ber tags.
  */
#define BER_CONTAINER_FLAG        (0x20)

/**
  * The integer value indicating an indefinite length.
  * This value is only used in the implementation, and
  * is not related to the encoding.
  */
#define BER_INDEFINITE_LENGTH     (-1)


// ======================================================
//
// BER tag
//
// ======================================================

/**
  * Defines a ber tag.
  */
typedef struct SBerTag
{
   /**
     * The ber tag's preamble, which holds the tag class
     * and the container bit.
     */
   byte preamble;

   /**
     * Unsigned integer to hold the number of the tag.
     */
   tagnumber number;
} BerTag;

/**
  * Initializes a BerTag instance with a tag class and a
  * tag number.
  * @param pThis pointer to the object to process.
  * @berClass the tag class to initialize BerTag.preamble with.
  * @param number the tag number to initialize BerTag.number with.
  */
LIBEMBER_API void berTag_init(BerTag *pThis, BerClass berClass, tagnumber number);

/**
  * Gets a value indicating whether the passed tag has the
  * container flag set.
  * @param pThis pointer to the object to process.
  * @return True if the passed tag has the container flag set.
  */
LIBEMBER_API bool berTag_isContainer(const BerTag *pThis);

/**
  * Sets a value indicating whether the passed tag has the
  * container flag set.
  * @param pThis pointer to the object to process.
  */
LIBEMBER_API void berTag_setContainer(BerTag *pThis, bool value);

/**
  * Gets the ber class of the passed tag.
  * @param pThis pointer to the object to process.
  * @return The ber class of the passed tag.
  */
LIBEMBER_API BerClass berTag_getClass(const BerTag *pThis);

/**
  * Sets the ber class of the passed tag.
  * @param pThis pointer to the object to process.
  * @param value The new ber class to set.
  */
LIBEMBER_API void berTag_setClass(BerTag *pThis, BerClass value);

/**
  * Sets the ber class of the passed tag.
  * @param pThis pointer to the object to process.
  * @param value The new ber class to set.
  */
LIBEMBER_API bool berTag_isZero(const BerTag *pThis);

/**
  * Converts the passed tag to a tag with the same class
  * and number, but with the container flag set.
  * @param pThis pointer to the object to process.
  * @return A copy of the tag pointed to by @p pThis with
  *      the container flag set.
  */
LIBEMBER_API BerTag berTag_toContainer(const BerTag *pThis);

/**
  * Gets the number of the tag as a bertype value,
  * taking the tag class into account.
  * If the tag class is APPLICATION, BerType_ApplicationFlag is set.
  * @param pThis pointer to the object to process.
  * @return The number of the tag as a bertype value,
  *      APPLICATION-1 will be returned as (BerType_ApplicationFlag | 1).
  *      UNIVERSAL-1 will be returned as 1.
  */
LIBEMBER_API bertype berTag_numberAsType(const BerTag *pThis);

#ifdef SECURE_CRT
/**
  * Writes a human-readable string representation of
  * this tag to the passed buffer.
  * @param pThis pointer to the object to process.
  * @param pBuffer pointer to the buffer to store
  *      the zero-terminated string to.
  */
LIBEMBER_API void berTag_toString(const BerTag *pThis, pstr pBuffer, int bufferSize);
#else
/**
  * Writes a human-readable string representation of
  * this tag to the passed buffer.
  * @param pThis pointer to the object to process.
  * @param pBuffer pointer to the buffer to store
  *      the zero-terminated string to.
  * @param bufferSize size of the buffer pointed to
  *      by @p pBuffer.
  */
LIBEMBER_API void berTag_toString(const BerTag *pThis, pstr pBuffer);
#endif

/**
  * Gets a value indicating whether two BerTag instances
  * contain the same information or not.
  * @param pThis pointer to the object to process.
  * @param pThat pointer to the BerTag to compare to @p pThis.
  */
LIBEMBER_API bool berTag_equals(const BerTag *pThis, const BerTag *pThat);

#endif
