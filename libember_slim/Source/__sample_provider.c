/*
   libember_slim sample

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>

//$$ MSVCRT specific
#include <WinSock2.h>

#include "emberplus.h"
#include "emberinternal.h"

//$$ MSVCRT specific
// Need to link with Ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")


#ifndef SECURE_CRT
#define stringCopy(dest, size, source) \
   do { strncpy(dest, source, size); dest[(size) - 1] = 0; } while(0)

#define printf_s printf
#define _itoa_s(value, dest, size, radix) \
   itoa(value, dest, radix)
#else
#define stringCopy(dest, size, source) \
   do { strncpy_s(dest, size, source, (size) - 1); dest[(size) - 1] = 0; } while(0)
#endif

#define stringDup(pStr) \
   (pStr != NULL ? _strdup(pStr) : NULL)


// ====================================================================
//
// utils
//
// ====================================================================

static void onThrowError(int error, pcstr pMessage)
{
   printf_s("ber error %d: '%s'\n", error, pMessage);
}

static void onFailAssertion(pcstr pFileName, int lineNumber)
{
   printf_s("Debug assertion failed @ '%s' line %d", pFileName, lineNumber);
}


// ====================================================================
//
// streams
//
// ====================================================================

#define STREAMS_COUNT (3)
#define STREAMS_MAX_STRING_LENGTH (16)

static GlowStreamEntry _streams[STREAMS_COUNT];

static void initializePpmStreams()
{
   bzero_item(_streams);

   _streams[0].streamIdentifier = 0;
   _streams[0].streamValue.flag = GlowParameterType_Integer;

   _streams[1].streamIdentifier = 1;
   _streams[1].streamValue.flag = GlowParameterType_Octets;
   _streams[1].streamValue.choice.octets.pOctets = newarr(byte, 8);
   _streams[1].streamValue.choice.octets.length = 8;

   _streams[2].streamIdentifier = 2;
   _streams[2].streamValue.flag = GlowParameterType_String;
   _streams[2].streamValue.choice.pString = newarr(char, STREAMS_MAX_STRING_LENGTH);
}

static void freePpmStreams()
{
   freeMemory(_streams[1].streamValue.choice.octets.pOctets);
   freeMemory(_streams[2].streamValue.choice.pString);
}

static void collectPpmData()
{
   // ber integer
   _streams[0].streamValue.choice.integer = ((rand() % 80) - 64) * 32; // from -64 to +15 in db/32

   // octets containing big endian int16
   *(short *)&_streams[1].streamValue.choice.octets.pOctets[0] = htons(((rand() % 256) - 128) * 32); // from -128 to +127 in db/32

   // octets containing little endian int32
   *(berint *)&_streams[1].streamValue.choice.octets.pOctets[4] = ((rand() % 256) - 255) * 32; // from -255 to 0 in db/32

   stringCopy(_streams[2].streamValue.choice.pString, STREAMS_MAX_STRING_LENGTH, "abc");
   _itoa_s(rand() % 100, &_streams[2].streamValue.choice.pString[3], STREAMS_MAX_STRING_LENGTH - 3, 10);
}


// ====================================================================
//
// Sample tree
//
// ====================================================================

#define SampleNode_MaxChildren (16)

typedef struct __SampleNode
{
   bool isParameter;
   GlowFieldFlags fields;

   union
   {
      GlowNode node;
      GlowParameter param;
   };

   int childrenCount;
   struct __SampleNode *children[SampleNode_MaxChildren];
} SampleNode;

static void sampleNode_init(SampleNode *pThis, SampleNode *pParent)
{
   bzero_item(*pThis);

   if(pParent != NULL)
   {
      if(pParent->childrenCount < SampleNode_MaxChildren)
      {
         pParent->children[pParent->childrenCount] = pThis;
         pParent->childrenCount++;
      }
   }
}

static void sampleNode_free(SampleNode *pThis)
{
   int index;
   SampleNode *pChild;

   for(index = 0; index < pThis->childrenCount; index++)
   {
      pChild = pThis->children[index];

      if(pChild != NULL)
      {
         sampleNode_free(pChild);
         freeMemory(pChild);
      }
   }

   if(pThis->isParameter)
      glowParameter_free(&pThis->param);
   else
      glowNode_free(&pThis->node);

   bzero_item(*pThis);
}

static SampleNode *createNode(SampleNode *pParent, pcstr pIdentifier, pcstr pDescription)
{
   SampleNode *pNode = newobj(SampleNode);
   dword fields = GlowFieldFlag_Identifier;
   sampleNode_init(pNode, pParent);
   pNode->node.pIdentifier = stringDup(pIdentifier);

   if(pDescription != NULL)
   {
      pNode->node.pDescription = stringDup(pDescription);
      fields |= GlowFieldFlag_Description;
   }

   pNode->fields = (GlowFieldFlags)fields;
   return pNode;
}

static SampleNode *createParameter(SampleNode *pParent, pcstr pIdentifier, pcstr pDescription)
{
   SampleNode *pNode = newobj(SampleNode);
   dword fields = GlowFieldFlag_Identifier;
   sampleNode_init(pNode, pParent);
   pNode->isParameter = true;

   pNode->param.pIdentifier = stringDup(pIdentifier);

   if(pDescription != NULL)
   {
      pNode->param.pDescription = stringDup(pDescription);
      fields |= GlowFieldFlag_Description;
   }

   pNode->fields = (GlowFieldFlags)fields;
   return pNode;
}

static void createGain(SampleNode *pParent)
{
   SampleNode *pParam = createParameter(pParent, "gain", "power in db");
   dword fields = pParam->fields;

   pParam->param.access = GlowAccess_ReadWrite;
   fields |= GlowFieldFlag_Access;

   pParam->param.value.choice.integer = 0;
   pParam->param.value.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Value;

   pParam->param.minimum.choice.integer = 0;
   pParam->param.minimum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Minimum;

   pParam->param.maximum.choice.integer = 65535;
   pParam->param.maximum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Maximum;

   pParam->param.factor = 64;
   fields |= GlowFieldFlag_Factor;

   pParam->param.pFormat = "%Lf db";
   fields |= GlowFieldFlag_Format;

   pParam->param.pSchemaIdentifiers = stringDup("de.l-s-b.emberplus.samples.gain");
   fields |= GlowFieldFlag_SchemaIdentifier;

   pParam->fields = (GlowFieldFlags)fields;
}

static void createVolume(SampleNode *pParent)
{
   SampleNode *pParam = createParameter(pParent, "volume", "loud?");
   dword fields = pParam->fields;

   pParam->param.access = GlowAccess_ReadWrite;
   fields |= GlowFieldFlag_Access;

   pParam->param.value.choice.real = 0.0;
   pParam->param.value.flag = GlowParameterType_Real;
   fields |= GlowFieldFlag_Value;

   pParam->param.minimum.choice.real = -1000.0;
   pParam->param.minimum.flag = GlowParameterType_Real;
   fields |= GlowFieldFlag_Minimum;

   pParam->param.maximum.choice.real = 1000.0;
   pParam->param.maximum.flag = GlowParameterType_Real;
   fields |= GlowFieldFlag_Maximum;

   pParam->param.pFormat = "%Lf db";
   fields |= GlowFieldFlag_Format;

   pParam->fields = (GlowFieldFlags)fields;
}

static void createFormat(SampleNode *pParent)
{
   SampleNode *pParam = createParameter(pParent, "format", "simple enum");
   dword fields = pParam->fields;

   pParam->param.access = GlowAccess_ReadWrite;
   fields |= GlowFieldFlag_Access;

   pParam->param.value.choice.integer = 0;
   pParam->param.value.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Value;

   pParam->param.pEnumeration = "4:3\n16:9\nHD\nFull HD\nCinema";
   fields |= GlowFieldFlag_Enumeration;

   pParam->fields = (GlowFieldFlags)fields;
}

static void createStream1(SampleNode *pParent)
{
   SampleNode *pParam = createParameter(pParent, "stream1", "1/32 db");
   dword fields = pParam->fields;

   pParam->param.access = GlowAccess_Read;
   fields |= GlowFieldFlag_Access;

   pParam->param.value.flag = GlowParameterType_Integer;
   pParam->param.value.choice.integer = 0;
   fields |= GlowFieldFlag_Value;

   pParam->param.minimum.choice.integer = -64 * 32;
   pParam->param.minimum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Minimum;

   pParam->param.maximum.choice.integer = 15 * 32;
   pParam->param.maximum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Maximum;

   pParam->param.streamIdentifier = 0;
   fields |= GlowFieldFlag_StreamIdentifier;

   pParam->param.pFormula = "($/32 + log(2) - 1/5 + e^(1/4))\n($*32)";
   fields |= GlowFieldFlag_Formula;

   pParam->fields = (GlowFieldFlags)fields;
}

static void createStream2(SampleNode *pParent)
{
   SampleNode *pParam = createParameter(pParent, "stream2", "1/32 db");
   dword fields = pParam->fields;

   pParam->param.access = GlowAccess_None;
   fields |= GlowFieldFlag_Access;

   pParam->param.type = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Type;

   pParam->param.factor = 32;
   fields |= GlowFieldFlag_Factor;

   pParam->param.minimum.choice.integer = -128 * 32;
   pParam->param.minimum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Minimum;

   pParam->param.maximum.choice.integer = 127 * 32;
   pParam->param.maximum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Maximum;

   pParam->param.streamIdentifier = 1;
   fields |= GlowFieldFlag_StreamIdentifier;

   pParam->param.streamDescriptor.format = GlowStreamFormat_SignedInt16BigEndian;
   pParam->param.streamDescriptor.offset = 0;
   fields |= GlowFieldFlag_StreamDescriptor;

   pParam->fields = (GlowFieldFlags)fields;
}

static void createStream3(SampleNode *pParent)
{
   SampleNode *pParam = createParameter(pParent, "stream3", "1/32 db");
   dword fields = pParam->fields;

   pParam->param.access = GlowAccess_None;
   fields |= GlowFieldFlag_Access;

   pParam->param.type = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Type;

   pParam->param.factor = 32;
   fields |= GlowFieldFlag_Factor;

   pParam->param.minimum.choice.integer = -255 * 32;
   pParam->param.minimum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Minimum;

   pParam->param.maximum.choice.integer = 0 * 32;
   pParam->param.maximum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Maximum;

   pParam->param.streamIdentifier = 1;
   fields |= GlowFieldFlag_StreamIdentifier;

   pParam->param.streamDescriptor.format = GlowStreamFormat_SignedInt32LittleEndian;
   pParam->param.streamDescriptor.offset = 4;
   fields |= GlowFieldFlag_StreamDescriptor;

   pParam->fields = (GlowFieldFlags)fields;
}

static void createStream4(SampleNode *pParent)
{
   SampleNode *pParam = createParameter(pParent, "stream4", "strings");
   dword fields = pParam->fields;

   pParam->param.access = GlowAccess_None;
   fields |= GlowFieldFlag_Access;

   pParam->param.type = GlowParameterType_String;
   fields |= GlowFieldFlag_Type;

   pParam->param.maximum.choice.integer = STREAMS_MAX_STRING_LENGTH - 1;
   pParam->param.maximum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Maximum;

   pParam->param.streamIdentifier = 2;
   fields |= GlowFieldFlag_StreamIdentifier;

   pParam->fields = (GlowFieldFlags)fields;
}

static void buildTree(SampleNode *pRoot)
{
   SampleNode *pVisualRoot, *pFood, *pNonFood, *pStreams, *pAudio;

   pVisualRoot = createNode(pRoot, "root", "root");

   pFood = createNode(pVisualRoot, "food", "Some food");
   createNode(pFood, "parsley", "tasty herb");
   createNode(pFood, "salad", "eat a lot of it!");

   pNonFood = createNode(pVisualRoot, "non-food", "tech stuff");

   pAudio = createNode(pNonFood, "audio", "ask Lawo");
   createGain(pAudio);
   createVolume(pAudio);
   createFormat(pAudio);
   createNode(pAudio, "dangling", NULL);

   pStreams = createNode(pNonFood, "streams", "see 'em move");
   pStreams->node.pSchemaIdentifiers = stringDup("de.l-s-b.emberplus.samples.streams");
   pStreams->fields |= GlowFieldFlag_SchemaIdentifier;
   createStream1(pStreams);
   createStream2(pStreams);
   createStream3(pStreams);
   createStream4(pStreams);
}

static SampleNode *findNode(SampleNode *pRoot, const berint *pPath, int *pPathLength)
{
   int pathIndex;
   int nodeIndex;
   SampleNode *pCursor = pRoot;

   for(pathIndex = 0; pathIndex < *pPathLength; pathIndex++)
   {
      nodeIndex = pPath[pathIndex];

      if(nodeIndex < pCursor->childrenCount)
         pCursor = pCursor->children[nodeIndex];
      else
         break;
   }

   *pPathLength = pathIndex;
   return pCursor;
}


// ====================================================================
//
// glow provider (tcp/ip)
//
// ====================================================================

typedef struct
{
   SOCKET sock;
   unsigned int streamSubscriptions[STREAMS_COUNT];
   pstr pEnumeration;
} ClientInfo;


// ====================================================================
//
// glow communication
//
// ====================================================================

SampleNode _root;

static void onCommand(const GlowCommand *pCommand, const berint *pPath, int pathLength, voidptr state)
{
   int bufferSize = 512;
   byte *pBuffer;
   GlowOutput output;
   ClientInfo *pClientInfo = (ClientInfo *)state;
   berint *pOutPath;
   int nodeIndex;
   SampleNode *pCurrent;
   SampleNode *pCursor = findNode(&_root, pPath, &pathLength);
   SOCKET sock = pClientInfo->sock;
   int fields;

   if(pCommand->number == GlowCommandType_GetDirectory)
   {
      pOutPath = newarr(berint, GLOW_MAX_TREE_DEPTH);
      memcpy(pOutPath, pPath, pathLength * sizeof(berint));
      pBuffer = newarr(byte, bufferSize);
      glowOutput_init(&output, pBuffer, bufferSize, 0);

      if(pCursor->isParameter)
      {
         // parameter - send single complete parameter
         fields = pCursor->fields & pCommand->options.dirFieldMask;
         glowOutput_beginPackage(&output, true);
         glow_writeQualifiedParameter(&output, &pCursor->param, (GlowFieldFlags)fields, pOutPath, pathLength);
         send(sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
      }
      else if(pCursor->childrenCount == 0)
      {
         // leaf node - send single empty node
         fields = 0;
         glowOutput_beginPackage(&output, true);
         glow_writeQualifiedNode(&output, &pCursor->node, (GlowFieldFlags)fields, pOutPath, pathLength);
         send(sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
      }
      else
      {
         // node - send children
         for(nodeIndex = 0; nodeIndex < pCursor->childrenCount; nodeIndex++)
         {
            pCurrent = pCursor->children[nodeIndex];

            if(pCurrent != NULL)
            {
               glowOutput_beginPackage(&output, nodeIndex == pCursor->childrenCount - 1);
               pOutPath[pathLength] = nodeIndex;
               fields = pCurrent->fields & pCommand->options.dirFieldMask;

               if(pCurrent->isParameter)
                  glow_writeQualifiedParameter(&output, &pCurrent->param, (GlowFieldFlags)fields, pOutPath, pathLength + 1);
               else
                  glow_writeQualifiedNode(&output, &pCurrent->node, (GlowFieldFlags)fields, pOutPath, pathLength + 1);

               send(sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
            }
         }
      }

      freeMemory(pBuffer);
      freeMemory(pOutPath);
   }
   else if(pCommand->number == GlowCommandType_Subscribe)
   {
      if(pCursor->isParameter
      && pCursor->param.streamIdentifier >= 0 && pCursor->param.streamIdentifier < STREAMS_COUNT)
         pClientInfo->streamSubscriptions[pCursor->param.streamIdentifier]++;
   }
   else if(pCommand->number == GlowCommandType_Unsubscribe)
   {
      if(pCursor->isParameter
      && pCursor->param.streamIdentifier >= 0 && pCursor->param.streamIdentifier < STREAMS_COUNT)
         pClientInfo->streamSubscriptions[pCursor->param.streamIdentifier]--;
   }
}

static void onParameter(const GlowParameter *pParameter, GlowFieldFlags fields, const berint *pPath, int pathLength, voidptr state)
{
   GlowOutput output;
   const int bufferSize = 512;
   byte *pBuffer;
   ClientInfo *pClientInfo = (ClientInfo *)state;
   SampleNode *pCursor = findNode(&_root, pPath, &pathLength);
   SOCKET sock = pClientInfo->sock;

   if(pCursor->isParameter
   && (fields & GlowFieldFlag_Value) == GlowFieldFlag_Value)
   {
      glowValue_free(&pCursor->param.value);
      glowValue_copyFrom(&pCursor->param.value, &pParameter->value);

      pBuffer = newarr(byte, bufferSize);
      glowOutput_init(&output, pBuffer, bufferSize, 0);
      glowOutput_beginPackage(&output, true);
      glow_writeQualifiedParameter(&output, &pCursor->param, GlowFieldFlag_Value, pPath, pathLength);
      send(sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
      freeMemory(pBuffer);
   }
}

static void onOtherPackageReceived(const byte *pPackage, int length, voidptr state)
{
   ClientInfo *pClientInfo = (ClientInfo *)state;
   byte buffer[16];
   unsigned int txLength;

   if(length >= 4
   && pPackage[1] == EMBER_MESSAGE_ID
   && pPackage[2] == EMBER_COMMAND_KEEPALIVE_REQUEST)
   {
      txLength = emberFraming_writeKeepAliveResponse(buffer, sizeof(buffer), pPackage[0]);
      send(pClientInfo->sock, (char *)buffer, txLength, 0);
   }
}

static void sendPpmStreams(SOCKET sock, byte *pBuffer, int bufferSize, const ClientInfo *pClientInfo)
{
   GlowOutput output;
   int index;
   int entriesWritten = 0;

   for(index = 0; index < STREAMS_COUNT; index++)
   {
      if(pClientInfo->streamSubscriptions[index] > 0)
      {
         if(entriesWritten == 0)
         {
            glowOutput_init(&output, pBuffer, bufferSize, 0);
            glowOutput_beginStreamPackage(&output, true);
         }

         glow_writeStreamEntry(&output, &_streams[index]);
         entriesWritten++;
      }
   }

   if(entriesWritten > 0)
      send(sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
}

static void handleClient(SOCKET sock)
{
   byte buffer[64];
   int read;
   const int rxBufferSize = 1024; // max size of unfamed package
   const struct timeval timeout = {0, 50 * 1000}; // 50 milliseconds timeout for select()
   const int noDelay = 1;
   const int streamBufferSize = 128;
   fd_set fdset;
   int fdsReady;
   ClientInfo client;

   // allocate large objects on the heap
   GlowReader *pReader = newobj(GlowReader);
   byte *pRxBuffer = newarr(byte, rxBufferSize);
   byte *pStreamBuffer = newarr(byte, streamBufferSize);

   srand((unsigned int)time(NULL));

   // turn off nagle algorithm for ppm data
   setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&noDelay, sizeof(noDelay));

   bzero_item(client);
   client.sock = sock;

   // only handle parameters and commands
   glowReader_init(pReader, NULL, onParameter, onCommand, NULL, (voidptr)&client, pRxBuffer, rxBufferSize);
   pReader->onOtherPackageReceived = onOtherPackageReceived;

   while(true)
   {
      FD_ZERO(&fdset);
      FD_SET(sock, &fdset);

      // wait for input, if none received -> send ppm data
      fdsReady = select(1, &fdset, NULL, NULL, &timeout);

      if(fdsReady == 1) // socket is ready to read
      {
         if(FD_ISSET(sock, &fdset))
         {
            read = recv(sock, (char *)buffer, sizeof(buffer), 0);

            printf_s("received %d bytes\n", read);

            if(read > 0)
               glowReader_readBytes(pReader, buffer, read);
            else
               break;
         }
      }
      else if(fdsReady == 0) // timeout
      {
         // generate some random ppm data for this sample. usually, ppm data is delivered by some ppm driver
         collectPpmData();

         sendPpmStreams(sock, pStreamBuffer, streamBufferSize, &client);
      }
      else
      {
         break;
      }
   }

   closesocket(sock);
   glowReader_free(pReader);
   freeMemory(pStreamBuffer);
   freeMemory(pRxBuffer);
   freeMemory(pReader);
}

static void initSockets()
{
   //$$ MSVCRT specific
   WSADATA wsaData;

   WSAStartup(MAKEWORD(2, 2), &wsaData);
}

static void shutdownSockets()
{
   //$$ MSVCRT specific
   WSACleanup();
}

static void acceptClient(int port)
{
   SOCKET listenSock, clientSock;
   struct sockaddr_in localAddr;

   initSockets();

   listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   localAddr.sin_family = AF_INET;
   localAddr.sin_addr.s_addr = INADDR_ANY;
   localAddr.sin_port = htons((unsigned short)port);

   bind(listenSock, (struct sockaddr *)&localAddr, sizeof(localAddr));

   listen(listenSock, 1);

   clientSock = accept(listenSock, NULL, NULL);

   if(clientSock > 0)
   {
      printf_s("client accepted\n");
      handleClient(clientSock);
   }

   closesocket(listenSock);

   shutdownSockets();
}


// ====================================================================
//
// provider sample entry point
//
// ====================================================================

static int allocCount = 0;

static void *allocMemoryImpl(size_t size)
{
   void *pMemory = malloc(size);

   //if(sizeof(void *) == 8)
   //   printf("allocate %lu bytes: %llX\n", size, (unsigned long long)pMemory);
   //else
   //   printf("allocate %lu bytes: %lX\n", size, (unsigned long)pMemory);

   allocCount++;
   return pMemory;
}

static void freeMemoryImpl(void *pMemory)
{
   //if(sizeof(void *) == 8)
   //   printf("free: %llX\n", (unsigned long long)pMemory);
   //else
   //   printf("free: %lX\n", (unsigned long)pMemory);

   allocCount--;
   free(pMemory);
}

void runProvider(int argc, char **argv)
{
   int port;

   ember_init(onThrowError, onFailAssertion, malloc, free);
   initializePpmStreams();
   buildTree(&_root);

   if(argc >= 2)
   {
      port = atoi(argv[1]);

      printf_s("accepting client on port %d\n", port);
      acceptClient(port);
   }

   sampleNode_free(&_root);
   freePpmStreams();
}
