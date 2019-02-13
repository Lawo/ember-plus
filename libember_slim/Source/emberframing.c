/*
   libember_slim -- ANSI C implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <string.h>
#include "bytebuffer.h"
#include "emberframing.h"
#include "emberinternal.h"

// ======================================================
//
// S101 defines
//
// ======================================================

#define S101_BOF        (0xFE)
#define S101_EOF        (0xFF)
#define S101_CE         (0xFD)
#define S101_Invalid    (0xF8)
#define S101_Xor        (0x20)

#define S101_InvalidID  (0xF0)

#define S101_ENQ        (0xFB)
#define S101_SYN        (0xFC)


// ======================================================
//
// crc CCITT
//
// ======================================================

static const unsigned short _crcTable[256] =
{
   0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
   0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
   0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
   0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
   0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
   0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
   0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
   0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
   0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
   0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
   0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
   0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
   0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
   0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
   0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
   0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
   0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
   0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
   0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
   0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
   0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
   0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
   0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
   0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
   0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
   0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
   0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
   0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
   0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
   0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
   0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
   0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78,
};


static unsigned short crc_addByte(unsigned short crc, byte b)
{
   return (unsigned short)((crc >> 8) ^ _crcTable[(byte)((crc ^ b) & 0xFF)]);
}


// ======================================================
//
// BerFramingOutput locals
//
// ======================================================

void berMemoryOutput_writeByte(BerOutput *pBase, byte b);

static void writeEscapedByte(BerOutput *pBase, byte b)
{
   if(b >= S101_Invalid)
   {
      berMemoryOutput_writeByte(pBase, S101_CE);
      b ^= S101_Xor;
   }

   berMemoryOutput_writeByte(pBase, b);
}

static void writeEscapedByteWithCrc(BerFramingOutput *pThis, byte b)
{
   BerOutput *pBase = &pThis->base.base;

   pThis->crc = crc_addByte(pThis->crc, b);

   writeEscapedByte(pBase, b);
}

static void berFramingOutput_writeByte(BerOutput *pBase, byte b)
{
   writeEscapedByteWithCrc((BerFramingOutput *)pBase, b);
}

static void berFramingOutput_writeBytes(BerOutput *pBase, const byte *pBytes, int count)
{
   BerFramingOutput *pThis = (BerFramingOutput *)pBase;

   for( ; count > 0; count--, pBytes++)
      writeEscapedByteWithCrc(pThis, *pBytes);
}


// Non-Escaping Framing Output

static void berFramingOutput_writeByteWithoutEscaping(BerOutput *pBase, byte b)
{
    berMemoryOutput_writeByte(pBase, b);
}

static void berFramingOutput_writeBytesWithoutEscaping(BerOutput *pBase, byte const *pBytes, int count)
{
    for ( ; count > 0; count--, pBytes++)
        berMemoryOutput_writeByte(pBase, *pBytes);
}


// ======================================================
//
// BerFramingOutput globals
//
// ======================================================


void berFramingOutput_initImpl(BerFramingOutput *pThis,
    byte *pMemory,
    unsigned int size,
    byte slotId,
    byte dtd,
    const byte *pAppBytes,
    byte appBytesCount,
    bool useNonEscapingFrames)
{
    ASSERT(pThis != NULL);
    ASSERT(size <= EMBER_MAXIMUM_PACKAGE_LENGTH);

    if (size > EMBER_MAXIMUM_PACKAGE_LENGTH)
        size = EMBER_MAXIMUM_PACKAGE_LENGTH;

    bzero_item(*pThis);
    berMemoryOutput_init(&pThis->base, pMemory, size);

    if (useNonEscapingFrames)
    {
        pThis->base.base.writeByte = berFramingOutput_writeByteWithoutEscaping;
        pThis->base.base.writeBytes = berFramingOutput_writeBytesWithoutEscaping;
    }
    else
    {
        pThis->base.base.writeByte = berFramingOutput_writeByte;
        pThis->base.base.writeBytes = berFramingOutput_writeBytes;
    }

    pThis->crc = 0xFFFF;
    pThis->slotId = slotId;
    pThis->dtd = dtd;

    pThis->pAppBytes = pAppBytes;
    pThis->appBytesCount = appBytesCount;
    pThis->useNonEscapingFrames = useNonEscapingFrames;
}

void berFramingOutput_init(
    BerFramingOutput *pThis,
    byte *pMemory,
    unsigned int size,
    byte slotId,
    byte dtd,
    const byte *pAppBytes,
    byte appBytesCount)
{
    berFramingOutput_initImpl(pThis, pMemory, size, slotId, dtd, pAppBytes, appBytesCount, false);
}

void berFramingOutput_initWithoutEscaping(
    BerFramingOutput *pThis,
    byte *pMemory,
    unsigned int size,
    byte slotId,
    byte dtd,
    const byte *pAppBytes,
    byte appBytesCount)
{
    berFramingOutput_initImpl(pThis, pMemory, size, slotId, dtd, pAppBytes, appBytesCount, true);
}

void berFramingOutput_writeHeader(BerFramingOutput *pThis, EmberFramingFlags flags)
{
   BerOutput *pBase = &pThis->base.base;
   unsigned short index;

   ASSERT(pThis != NULL);
   ASSERT(pThis->base.position == 0);

   if (pThis->useNonEscapingFrames)
   {
       berMemoryOutput_writeByte(pBase, S101_Invalid);
       berMemoryOutput_writeByte(pBase, 0x04);                  // Length Length
       berMemoryOutput_writeByte(pBase, 0x00);                  // Payload Length 0
       berMemoryOutput_writeByte(pBase, 0x00);                  // Payload Length 1
       berMemoryOutput_writeByte(pBase, 0x00);                  // Payload Length 2
       berMemoryOutput_writeByte(pBase, 0x00);                  // Payload Length 3
       berMemoryOutput_writeByte(pBase, 0x00);                  // slotid
       berMemoryOutput_writeByte(pBase, EMBER_MESSAGE_ID);      // message
       berMemoryOutput_writeByte(pBase, EMBER_COMMAND_PAYLOAD); // command
       berMemoryOutput_writeByte(pBase, 0x01);                  // framing version
       berMemoryOutput_writeByte(pBase, (byte)(flags & 0xFF));  // flags: first_package | last_package;
       berMemoryOutput_writeByte(pBase, pThis->dtd);

       if (pThis->pAppBytes != NULL)
       {
           berMemoryOutput_writeByte(pBase, pThis->appBytesCount);

           for (index = 0; index < pThis->appBytesCount; index++)
               berMemoryOutput_writeByte(pBase, pThis->pAppBytes[index]);
       }
       else
       {
           berMemoryOutput_writeByte(pBase, 0x00);
       }
   }
   else
   {
       pThis->crc = 0xFFFF;

       berMemoryOutput_writeByte(pBase, S101_BOF);
       writeEscapedByteWithCrc(pThis, 0x00);                  // slotid
       writeEscapedByteWithCrc(pThis, EMBER_MESSAGE_ID);      // message
       writeEscapedByteWithCrc(pThis, EMBER_COMMAND_PAYLOAD); // command
       writeEscapedByteWithCrc(pThis, 0x01);                  // framing version
       writeEscapedByteWithCrc(pThis, (byte)(flags & 0xFF));  // flags: first_package | last_package
       writeEscapedByteWithCrc(pThis, pThis->dtd);            // dtd

       if (pThis->pAppBytes != NULL)
       {
           writeEscapedByteWithCrc(pThis, pThis->appBytesCount);

           for (index = 0; index < pThis->appBytesCount; index++)
               writeEscapedByteWithCrc(pThis, pThis->pAppBytes[index]);
       }
       else
       {
           writeEscapedByteWithCrc(pThis, 0x00);            // appbytes count
       }
   }
}

unsigned int berFramingOutput_finish(BerFramingOutput *pThis)
{
   BerOutput *pBase;
   unsigned short crc;
   unsigned int position;
   unsigned int payloadLength = pThis->base.position - 6;

   ASSERT(pThis != NULL);

   pBase = &pThis->base.base;

   if (pThis->useNonEscapingFrames)
   {
       pThis->base.pMemory[2] = (payloadLength >> 24) & 0xFF;
       pThis->base.pMemory[3] = (payloadLength >> 16) & 0xFF;
       pThis->base.pMemory[4] = (payloadLength >> 8) & 0xFF;
       pThis->base.pMemory[5] = (payloadLength >> 0) & 0xFF;
   }
   else
   {
       crc = ~pThis->crc;
       writeEscapedByte(pBase, (byte)((crc >> 0) & 0xFF));
       writeEscapedByte(pBase, (byte)((crc >> 8) & 0xFF));
       berMemoryOutput_writeByte(pBase, S101_EOF);
   }

   position = pThis->base.position;
   berMemoryOutput_reset(&pThis->base);
   return position;
}


// ======================================================
//
// Ember framing utilities locals
//
// ======================================================

static unsigned int writeKeepAlivePackage(BerFramingOutput *pOut, byte command)
{
   berMemoryOutput_writeByte(&pOut->base.base, S101_BOF);
   writeEscapedByteWithCrc(pOut, pOut->slotId);
   writeEscapedByteWithCrc(pOut, EMBER_MESSAGE_ID); // message
   writeEscapedByteWithCrc(pOut, command);          // command
   writeEscapedByteWithCrc(pOut, 0x01);             // framing version

   return berFramingOutput_finish(pOut);
}

static unsigned int writeKeepAlivePackageWithoutEscaping(BerFramingOutput *pOut, byte command)
{
    berMemoryOutput_writeByte(&pOut->base.base, S101_Invalid);
    berMemoryOutput_writeByte(&pOut->base.base, pOut->slotId);
    berMemoryOutput_writeByte(&pOut->base.base, EMBER_MESSAGE_ID); // message
    berMemoryOutput_writeByte(&pOut->base.base, command);          // command
    berMemoryOutput_writeByte(&pOut->base.base, 0x01);             // framing version

    return berFramingOutput_finish(pOut);
}


// ======================================================
//
// Ember framing utilities globals
//
// ======================================================

unsigned int emberFraming_writeKeepAliveRequest(byte *pBuffer, unsigned int size, byte slotId)
{
   BerFramingOutput output;
   berFramingOutput_init(&output, pBuffer, size, slotId, 0, NULL, 0);

   return writeKeepAlivePackage(&output, EMBER_COMMAND_KEEPALIVE_REQUEST);
}

unsigned int emberFraming_writeKeepAliveResponse(byte *pBuffer, unsigned int size, byte slotId)
{
   BerFramingOutput output;
   berFramingOutput_init(&output, pBuffer, size, slotId, 0, NULL, 0);

   return writeKeepAlivePackage(&output, EMBER_COMMAND_KEEPALIVE_RESPONSE);
}


unsigned int emberFraming_writeKeepAliveRequestWithoutEscaping(byte *pBuffer, unsigned int size, byte slotId)
{
    BerFramingOutput output;
    berFramingOutput_initWithoutEscaping(&output, pBuffer, size, slotId, 0, NULL, 0);

    return writeKeepAlivePackage(&output, EMBER_COMMAND_KEEPALIVE_REQUEST);
}

unsigned int emberFraming_writeKeepAliveResponseWithoutEscaping(byte *pBuffer, unsigned int size, byte slotId)
{
    BerFramingOutput output;
    berFramingOutput_initWithoutEscaping(&output, pBuffer, size, slotId, 0, NULL, 0);

    return writeKeepAlivePackage(&output, EMBER_COMMAND_KEEPALIVE_RESPONSE);
}


// ======================================================
//
// EmberFramingReader locals
//
// ======================================================

static void readFramedByte(EmberFramingReader *pThis, byte b)
{
   unsigned int loop = 0;
   unsigned int shift = 0;
   ByteBuffer *pBuffer = &pThis->buffer;

   if(byteBuffer_isEmpty(pBuffer))
   {
      if(b == S101_BOF)
      {
         byteBuffer_reset(pBuffer);
         byteBuffer_add(pBuffer, S101_BOF);
         pThis->payloadLength = 0;
         pThis->payloadLengthLength = 0;
         pThis->isNonEscapingFrame = false;
         pThis->isEscaped = false;
         pThis->crc = 0xFFFF;
      }
      else if (b == S101_Invalid)
      {
          byteBuffer_reset(pBuffer);
          byteBuffer_add(pBuffer, S101_Invalid);
          pThis->payloadLength = 0;
          pThis->payloadLengthLength = 0;
          pThis->isNonEscapingFrame = true;
          pThis->isEscaped = false;
          pThis->crc = 0xFFFF;
      }

      return;
   }

   if (pThis->isNonEscapingFrame)
   {
       byteBuffer_add(pBuffer, b);

       if (pBuffer->position == 2)
       {
           pThis->payloadLengthLength = b & 0x07;
       }
       else if (pBuffer->position > 1 && pBuffer->position - 2 == pThis->payloadLengthLength)
       {
           pThis->payloadLength = 0;

           for (loop = 0; loop < pThis->payloadLengthLength; loop++)
           {
               shift = (pThis->payloadLengthLength - loop - 1) * 8;
               pThis->payloadLength |= (pBuffer->pMemory[2 + loop] << shift);
           }
       }

       if (pBuffer->position >= 2 &&
           pBuffer->position - (2 + pThis->payloadLengthLength) == pThis->payloadLength)
       {
           pThis->onPackageReceived(
               pBuffer->pMemory + (2 + pThis->payloadLengthLength),
               pBuffer->position - (2 + pThis->payloadLengthLength),
               pThis->state);
           emberFramingReader_reset(pThis);
       }
   }
   else
   {
       if (b == S101_BOF)
       {
           byteBuffer_reset(pBuffer);
           byteBuffer_add(pBuffer, S101_BOF);
           pThis->isEscaped = false;
           pThis->crc = 0xFFFF;
           return;
       }

       if (b == S101_EOF)
       {
           if (pBuffer->position >= 2)
           {
               if (pThis->crc == 0xF0B8)
                   pThis->onPackageReceived(pBuffer->pMemory + 1, pBuffer->position - 3, pThis->state);
           }

           emberFramingReader_reset(pThis);
           return;
       }

       if (b == S101_CE)
       {
           pThis->isEscaped = true;
           return;
       }

       if (b >= S101_Invalid)
       {
           byteBuffer_reset(pBuffer);
           return;
       }

       if (pThis->isEscaped)
       {
           pThis->isEscaped = false;
           b ^= 0x20;
       }

       byteBuffer_add(pBuffer, b);
       pThis->crc = crc_addByte(pThis->crc, b);
   }
}


// ======================================================
//
// EmberFramingReader globals
//
// ======================================================

void emberFramingReader_init(EmberFramingReader *pThis,
                             byte *pMemory,
                             unsigned int size,
                             onPackageReceived_t onPackageReceived,
                             voidptr state)
{
   ASSERT(pThis != NULL);
   ASSERT(pMemory != NULL);
   ASSERT(size > 0);

   bzero_item(*pThis);

   byteBuffer_init(&pThis->buffer, pMemory, size);

   pThis->crc = 0xFFFF;
   pThis->onPackageReceived = onPackageReceived;
   pThis->state = state;
   pThis->isEscaped = false;
   pThis->isNonEscapingFrame = false;
   pThis->payloadLength = 0;
   pThis->payloadLengthLength = 0;
}

void emberFramingReader_reset(EmberFramingReader *pThis)
{
   ASSERT(pThis != NULL);

   byteBuffer_reset(&pThis->buffer);

   pThis->crc = 0xFFFF;
   pThis->isEscaped = false;
   pThis->isNonEscapingFrame = false;
   pThis->payloadLength = 0;
   pThis->payloadLengthLength = 0;
}

void emberFramingReader_readBytes(EmberFramingReader *pThis, const byte *pBytes, int count)
{
   ASSERT(pThis != NULL);
   ASSERT(pBytes != NULL);

   for( ; count > 0; count--, pBytes++)
      readFramedByte(pThis, *pBytes);
}
