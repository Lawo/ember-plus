/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_SLIM_BERIO_H
#define __LIBEMBER_SLIM_BERIO_H

#include <stdio.h>
#include "api.h"
#include "bertypes.h"

// ======================================================
//
// BerInput
//
// ======================================================

/**
  * Defines a basic input facility for ber decoders.
  * The readByte function must be set by derived types.
  */
typedef struct SBerInput
{
   /**
     * Reads one byte from the passed BerInput instance.
     * @param pThis pointer to the object to process.
     */
   byte (*readByte)(struct SBerInput *pThis);
} BerInput;



// ======================================================
//
// BerMemoryInput
//
// ======================================================

/**
  * Aggregates BerInput to provide in-memory decoding.
  * The readByte function is initialized when calling
  * berMemoryInput_init.
  * When calling readByte on a BerMemoryInput which has
  * already consumed all available bytes, 0xFF is
  * returned.
  * The caller should check for this condition prior to
  * calling readByte by calling berMemoryInput_isEof.
  */
typedef struct SBerMemoryInput
{
   /**
     * Base input.
     */
   BerInput base;

   /**
     * Address of first byte in memory to read from.
     */
   const byte *pMemory;

   /**
     * Number of bytes at the location pointed to by pMemory.
     */
   unsigned int size;

   /**
     * Current position of the read cursor.
     */
   unsigned int position;
} BerMemoryInput;

/**
  * Initializes a BerMemoryInput instance.
  * Must be called before any other operations on the
  * BerMemoryInput instance are invoked.
  * @param pThis pointer to the object to process.
  * @param pMemory pointer to the memory location to read from.
  * @param size number of bytes at @p pMemory.
  */
LIBEMBER_API void berMemoryInput_init(BerMemoryInput *pThis, const byte *pMemory, unsigned int size);

/**
  * Returns a value indicating whether all bytes from the
  * passed BerMemoryInput have been read.
  * @param pThis pointer to the object to process.
  */
LIBEMBER_API bool berMemoryInput_isEof(const struct SBerMemoryInput *pThis);


// ======================================================
//
// BerFileInput
//
// ======================================================

/**
  * Aggregates BerInput to provide file decoding.
  * The readByte function is initialized when calling
  * berFileInput_init.
  */
typedef struct SBerFileInput
{
   /**
     * Base input
     */
   BerInput base;

   /**
     * Pointer to the io buffer to read from.
     */
   FILE *pFile;
} BerFileInput;

/**
  * Initializes a BerFileInput instance.
  * Must be called before any other operations on the
  * BerFileInput instance are invoked.
  * @param pThis pointer to the object to process.
  * @param pFile pointer to the io buffer to read from.
  *     Must be open and readable.
  */
LIBEMBER_API void berFileInput_init(BerFileInput *pThis, FILE *pFile);


// ======================================================
//
// BerOutput
//
// ======================================================

/**
  * Defines a basic output facility for ber encoders.
  * The writeByte and writeBytes functions must be set by derived types.
  */
typedef struct SBerOutput
{
   /**
     * Writes one byte to the passed BerOutput instance.
     * @param pThis pointer to the object to process.
     */
   void (*writeByte)(struct SBerOutput *pThis, byte b);

   /**
     * Writes multiple bytes to the passed BerOutput instance.
     * @param pThis pointer to the object to process.
     * @param pBytes pointer to the first byte to write.
     * @param count number of bytes to write.
     */
   void (*writeBytes)(struct SBerOutput *pThis, const byte *pBytes, int count);
} BerOutput;


/**
  * Aggregates BerOutput to provide in-memory encoding.
  * The writeByte and writeBytes functions are initialized when calling
  * berMemoryOutput_init.
  * When writeByte or writeBytes is called, only as many bytes as still
  * available in the target buffer will be written.
  */
typedef struct SBerMemoryOutput
{
   /**
     * Base output.
     */
   BerOutput base;

   /**
     * Address of memory location to write to.
     */
   byte *pMemory;

   /**
     * Number of available bytes at pMemory.
     */
   unsigned int size;

   /**
     * Current position of the write cursor.
     * If this is equals size, the buffer is full
     * and any more write operations will silently fail.
     */
   unsigned int position;
} BerMemoryOutput;

/**
  * Initializes a BerMemoryOutput instance.
  * Must be called before any other operations on the
  * BerMemoryOutput instance are invoked.
  * @param pThis pointer to the object to process.
  * @param pMemory pointer to the memory location to write to.
  * @param size number of bytes at @p pMemory.
  */
LIBEMBER_API void berMemoryOutput_init(BerMemoryOutput *pThis, byte *pMemory, unsigned int size);

/**
  * Resets the position of a BerMemoryOutput instance.
  * @param pThis pointer to the object to process.
  */
LIBEMBER_API void berMemoryOutput_reset(BerMemoryOutput *pThis);


/**
  * Aggregates BerOutput to provide encoding to a file.
  * The writeByte and writeBytes functions are initialized when calling
  * berFileOutput_init.
  */
typedef struct SBerFileOutput
{
   /**
     * Base output.
     */
   BerOutput base;

   /**
     * Pointer to the io buffer to write to.
     */
   FILE *pFile;
} BerFileOutput;

/**
  * Initializes a BerFileOutput instance.
  * Must be called before any other operations on the
  * BerFileOutput instance are invoked.
  * @param pThis pointer to the object to process.
  * @param pFile pointer to the io buffer to write to.
  *     Must be open and writeable.
  */
LIBEMBER_API void berFileOutput_init(BerFileOutput *pThis, FILE *pFile);

#endif
