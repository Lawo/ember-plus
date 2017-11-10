/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_SLIM_EMBERFRAMING_H
#define __LIBEMBER_SLIM_EMBERFRAMING_H

#include "api.h"
#include "berio.h"


// ======================================================
//
// ember framing defines
//
// ======================================================

/**
  * Possible values for the "dtd" field in the package frame.
  * Currently, only the Glow DTD is supported.
  */
#define EMBER_DTD_GLOW   (1)

/**
  * S101 message id for ember
  */
#define EMBER_MESSAGE_ID (0x0E)

/**
  * framing "command" byte: message carries ember payload
  */
#define EMBER_COMMAND_PAYLOAD             (0)

/**
  * framing "command" byte: keepalive request
  */
#define EMBER_COMMAND_KEEPALIVE_REQUEST   (1)

/**
  * framing "command" byte: keepalive response
  */
#define EMBER_COMMAND_KEEPALIVE_RESPONSE  (2)


/**
  * maximum length of an ember+ package.
  */
#define EMBER_MAXIMUM_PACKAGE_LENGTH (1024)


/**
  * framing flags
  */
typedef enum EEmberFramingFlags
{
   EmberFramingFlag_EmptyPackage = 0x20,
   EmberFramingFlag_LastPackage  = 0x40,
   EmberFramingFlag_FirstPackage = 0x80,
} EmberFramingFlags;


// ======================================================
//
// BerFramingOutput
//
// ======================================================

/**
  * Aggregates BerMemoryOutput to provide encoding a framed package
  * to memory.
  * The writeByte and writeBytes functions are initialized when calling
  * berFramingOutput_init.
  */
typedef struct SBerFramingOutput
{
   /**
     * Base output.
     */
   BerMemoryOutput base;

   /**
     * Private field.
     */
   unsigned short crc;

   /**
    * Private field.
    */
   bool useNonEscapingFrames;

   /**
     * The slot id as described in the framing protocol documentation.
     */
   byte slotId;

   /**
     * The identifier of the dtd used by the ember data
     * contained in the frame.
     */
   byte dtd;

   /**
     * Pointer to application-defined bytes to store in the
     * frame header.
     */
   const byte *pAppBytes;

   /**
     * Number of application-defined bytes at pAppBytes.
     */
   byte appBytesCount;
} BerFramingOutput;

/**
  * Initializes a BerFramingOutput instance.
  * Must be called before any other operations on the
  * BerFramingOutput instance are invoked.
  * @param pThis pointer to the object to process.
  * @param pMemory pointer to the memory location to write
  *     the framed package to.
  * @param size number of bytes at @p pMemory.
  * @param slotId the slot id as described in the
  *     framing protocol documentation.
  * @param dtd the identifier of the dtd used by the ember data
  *     contained in the frame.
  * @param pAppBytes Pointer to application-defined bytes to store
  *     in the frame header.
  * @param appBytesCount number of application-defined bytes at
  *     @p pAppBytes.
  */
LIBEMBER_API void berFramingOutput_init(BerFramingOutput *pThis,
                           byte *pMemory,
                           unsigned int size,
                           byte slotId,
                           byte dtd,
                           const byte *pAppBytes,
                           byte appBytesCount);

/**
* Initializes a BerFramingOutput instance which uses the non-escaping variant of the S101 framing.
* Must be called before any other operations on the
* BerFramingOutput instance are invoked.
* @param pThis pointer to the object to process.
* @param pMemory pointer to the memory location to write
*     the framed package to.
* @param size number of bytes at @p pMemory.
* @param slotId the slot id as described in the
*     framing protocol documentation.
* @param dtd the identifier of the dtd used by the ember data
*     contained in the frame.
* @param pAppBytes Pointer to application-defined bytes to store
*     in the frame header.
* @param appBytesCount number of application-defined bytes at
*     @p pAppBytes.
*/
LIBEMBER_API void berFramingOutput_initWithoutEscaping(BerFramingOutput *pThis,
    byte *pMemory,
    unsigned int size,
    byte slotId,
    byte dtd,
    const byte *pAppBytes,
    byte appBytesCount);

/**
  * Writes the framing header to a BerFramingOutput.
  * Must be called before any Ember+ payload is written
  * to the output.
  * @param pThis pointer to the object to process.
  * @param flags contains a combination of package flags
  */
LIBEMBER_API void berFramingOutput_writeHeader(BerFramingOutput *pThis, EmberFramingFlags flags);

/**
  * Finishes the framed package. After this function
  * is called, the output at @p pThis can be re-used
  * to build another package.
  * @param pThis pointer to the object to process.
  * @return the length of the framed package located
  *     at the address passed to berFramingOutput_init.
  * @note you need to call this function when you have
  *     written a complete ember tree to @p pThis.
  */
LIBEMBER_API unsigned int berFramingOutput_finish(BerFramingOutput *pThis);


// ======================================================
//
// Ember framing utilities
//
// ======================================================

/**
  * Frames a Keep-Alive Request message into the passed buffer.
  * @param pBuffer pointer to the buffer where to store the
  *     framed package.
  * @param size the size of @pBuffer. Refer to the framing documentation
  *     to determine how many bytes are needed.
  * @param slotId the S101 slot id of the remote host.
  * @return the number of bytes written to @p pBuffer.
  */
LIBEMBER_API unsigned int emberFraming_writeKeepAliveRequest(byte *pBuffer, unsigned int size, byte slotId);

/**
  * Frames a Keep-Alive Response message into the passed buffer.
  * @param pBuffer pointer to the buffer where to store the
  *     framed package.
  * @param size the size of @pBuffer. Refer to the framing documentation
  *     to determine how many bytes are needed.
  * @param slotId the S101 slot id of the remote host.
  * @return the number of bytes written to @p pBuffer.
  */
LIBEMBER_API unsigned int emberFraming_writeKeepAliveResponse(byte *pBuffer, unsigned int size, byte slotId);

/**
* Frames a Keep-Alive Request message into the passed buffer by using the non-escaping variant of S101.
* @param pBuffer pointer to the buffer where to store the
*     framed package.
* @param size the size of @pBuffer. Refer to the framing documentation
*     to determine how many bytes are needed.
* @param slotId the S101 slot id of the remote host.
* @return the number of bytes written to @p pBuffer.
*/
LIBEMBER_API unsigned int emberFraming_writeKeepAliveRequestWithoutEscaping(byte *pBuffer, unsigned int size, byte slotId);

/**
* Frames a Keep-Alive Response message into the passed buffer by using the non-escaping variant of S101.
* @param pBuffer pointer to the buffer where to store the
*     framed package.
* @param size the size of @pBuffer. Refer to the framing documentation
*     to determine how many bytes are needed.
* @param slotId the S101 slot id of the remote host.
* @return the number of bytes written to @p pBuffer.
*/
LIBEMBER_API unsigned int emberFraming_writeKeepAliveResponseWithoutEscaping(byte *pBuffer, unsigned int size, byte slotId);


// ======================================================
//
// FramingReader
//
// ======================================================

/**
  * Function type used by EmberFramingReader to notify
  * the application that a complete framed package has been
  * read and unframed.
  * @param pPackage pointer to the first byte in the
  *     unframed package.
  * @param length the length of the unframed package
  *     at @p pPackage.
  * @param state application-defined argument.
  */
typedef void (*onPackageReceived_t)(const byte *pPackage, int length, voidptr state);

/**
  * An in-memory reader used to unframe framed packages.
  */
typedef struct SEmberFramingReader
{
   /**
     * The buffer to write unframed data to.
     */
   ByteBuffer buffer;

   /**
     * Private field.
     */
   bool isEscaped;

   /**
    * Private field.
    */
   bool isNonEscapingFrame;

   /**
    * Private field.
    */
   unsigned int payloadLength;

   /**
   * Private field.
   */
   unsigned int payloadLengthLength;

   /**
     * Private field.
     */
   unsigned short crc;

   /**
     * Pointer to application-defined function called when
     * a complete package has been unframed.
     */
   onPackageReceived_t onPackageReceived;

   /**
     * Application-defined argument passed to onPackageReceived.
     */
   voidptr state;
} EmberFramingReader;

/**
  * Initializes an EmberFramingReader instance.
  * Must be called before any other operations on the
  * EmberFramingReader instance are invoked.
  * @param pThis pointer to the object to process.
  * @param pMemory pointer to the memory location to write
  *     the unframed package to.
  * @param size number of bytes at @p pMemory.
  * @param onPackageReceived pointer to application-defined
  *     function called when a complete package has been unframed.
  * @param state application-defined argument passed to onPackageReceived.
  */
LIBEMBER_API void emberFramingReader_init(EmberFramingReader *pThis,
                             byte *pMemory,
                             unsigned int size,
                             onPackageReceived_t onPackageReceived,
                             voidptr state);

/**
  * Resets an EmberFramingReader instance.
  * After this function has been called, another framed package
  * can be unframed.
  * @param pThis pointer to the object to process.
  */
LIBEMBER_API void emberFramingReader_reset(EmberFramingReader *pThis);

/**
  * Feeds multiple bytes of framed data into an EmberFramingReader
  * instance, unframing the bytes on the fly.
  * framed package can be unframed.
  * @param pThis pointer to the object to process.
  * @param pBytes pointer to the first byte to feed into the reader.
  * @param count number of bytes at @p pBytes to feed into the
  *     reader.
  */
LIBEMBER_API void emberFramingReader_readBytes(EmberFramingReader *pThis, const byte *pBytes, int count);

#endif
