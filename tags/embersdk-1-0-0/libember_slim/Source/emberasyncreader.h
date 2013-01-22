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

#ifndef __LIBEMBER_SLIM_EMBERASYNCREADER_H
#define __LIBEMBER_SLIM_EMBERASYNCREADER_H

#include "berreader.h"

#ifndef MAX_EMBER_TREE_DEPTH
/**
  * The maximum depth of an ember tree to decode.
  * Can be set using a compiler option. Default
  * is 65.
  * If this value is exceeded during the decoding
  * process, the throwError callback is called.
  */
#define MAX_EMBER_TREE_DEPTH (65)
#endif

/**
  * private type supporting EmberAsyncReader
  */
typedef enum
{
   DecodeState_Tag,
   DecodeState_Length,
   DecodeState_Value,
   DecodeState_Terminator,
} __EmberAsyncDecodeState;


/**
  * private type supporting EmberAsyncReader
  */
typedef struct
{
   BerTag tag;
   bertype type;
   int length;
   int bytesRead;
} __EmberAsyncContainer;


/**
  * private type supporting EmberAsyncReader
  */
typedef struct
{
   __EmberAsyncContainer items[MAX_EMBER_TREE_DEPTH];
   int length;
} __EmberAsyncContainerStack;


/**
  * Defines a basic asynchronous reader for decoding
  * ember packages.
  * Call emberAsyncReader_readByte or emberAsyncReader_readBytes
  * to decode ember packages.
  * Set the onNewContainer and onItemReady callbacks
  * to evaluate decoded TLTLVs.
  */
typedef struct SEmberAsyncReader
{
   /**
     * Base reader.
     */
   BerReader base;

   /**
     * private field
     */
   __EmberAsyncDecodeState decodeState;

   /**
     * private field
     */
   __EmberAsyncContainerStack *pContainerStack;

   /**
     * private field
     */
   __EmberAsyncContainer *pCurrentContainer;

   /**
     * private field
     */
   int bytesRead;

   /**
     * private field
     */
   int bytesExpected;

   /**
     * May be set to a callback function to evaluate
     * decoded TLTLVs.
     * Called everytime the begin TLTL of a container
     * has benn decoded.
     * Calling one of the berReader_get<Value> functions
     * on @p pReader will call the throwError callback,
     * since the value of a container consists of the
     * container's child nodes, not a primitive value.
     * @param pReader pointer to the BerReader providing
     *      information about the decoded TLTL.
     */
   void (*onNewContainer)(const BerReader *pBase);

   /**
     * May be set to a callback function to evaluate
     * decoded TLTLVs.
     * Called everytime a complete TLTLV (primitive values
     * as well as containers) has been decoded.
     * If the decoded TLTLV is primitive, you can call 
     * one of the berReader_get<Value> functions to get
     * the TLTLV's value.
     * @param pReader pointer to the BerReader providing
     *      information about the decoded TLTLV.
     */
   void (*onItemReady)(const BerReader *pBase);

} EmberAsyncReader;

/**
  * Initializes an EmberAsyncReader instance.
  * Must be called before any other operations on the
  * EmberAsyncReader instance are invoked.
  * @param pThis pointer to the object to process.
  * @param pBuffer pointer to the memory location to store
  *     encoded values to.
  * @param bufferSize maximum length of a encoded value.
  *      If this value is exceeded, the throwError callback
  *      is invoked.
  */
void emberAsyncReader_init(EmberAsyncReader *pThis, byte *pBuffer, int bufferSize);

/**
  * Frees all dynamically allocated memory used by the
  * passed EmberAsyncReader. The memory has been allocated
  * by a prior call to emberAsyncReader_init.
  * @param pThis pointer to the object to process.
  */
void emberAsyncReader_free(EmberAsyncReader *pThis);

/**
  * Feeds a single input byte into the reader. If this byte completes
  * a TLTLV, the callback function pThis->onItemReady is invoked.
  * If the byte begins a new container, the callback function
  * pThis->onNewContainer is invoked.
  * @param pThis pointer to the object to process.
  * @param b The input byte to process.</param>
  */
void emberAsyncReader_readByte(EmberAsyncReader *pThis, byte b);

/**
  * Feeds a multiple input bytes into the reader. Everytime a complete
  * TLTLV is decoded, the callback function pThis->onItemReady is invoked.
  * Everytime a new container has been opened, the callback function
  * pThis->onNewContainer is invoked.
  * @param pThis pointer to the object to process.
  * @param pBytes pointer to the first byte to feed to @p pThis.</param>
  * @param count number of bytes to feed to @p pThis.</param>
  */
void emberAsyncReader_readBytes(EmberAsyncReader *pThis, const byte *pBytes, int count);

/**
  * Resets the internal state of the passed EmberAsyncReader.
  * @param pThis pointer to the object to process.
  */
void emberAsyncReader_reset(EmberAsyncReader *pThis);

#endif
