/*
   libember_slim sample
   Copyright (C) 2012  L-S-B Broadcast Technologies GmbH
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>

//$$ MSVCRT specific
#include <WinSock2.h>

#include "emberglow.h"

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

static GlowStreamEntry _streams[STREAMS_COUNT];

static void initializePpmStreams()
{
   bzero(_streams);

   _streams[0].streamIdentifier = 0;
   _streams[0].streamValue.flag = GlowParameterType_Integer;

   _streams[1].streamIdentifier = 1;
   _streams[1].streamValue.flag = GlowParameterType_Octets;
   _streams[1].streamValue.octets.length = 8;

   _streams[2].streamIdentifier = 2;
   _streams[2].streamValue.flag = GlowParameterType_String;
}

static void collectPpmData()
{
   // ber integer
   _streams[0].streamValue.integer = ((rand() % 80) - 64) * 32; // from -64 to +15 in db/32

   // octets containing big endian int16
   *(short *)&_streams[1].streamValue.octets.octets[0] = htons(((rand() % 256) - 128) * 32); // from -128 to +127 in db/32

   // octets containing little endian int32
   *(berint *)&_streams[1].streamValue.octets.octets[4] = ((rand() % 256) - 255) * 32; // from -255 to 0 in db/32

   stringCopy(_streams[2].streamValue.string, GLOW_MAX_VALUE_LENGTH, "abc");
   _itoa_s(rand() % 100, &_streams[2].streamValue.string[3], GLOW_MAX_VALUE_LENGTH - 3, 10);
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
   bzero(*pThis);

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
         free(pChild);
      }
   }

   bzero(*pThis);
}

static SampleNode *createNode(SampleNode *pParent, pcstr pIdentifier, pcstr pDescription)
{
   SampleNode *pNode = (SampleNode *)malloc(sizeof(SampleNode));
   dword fields = GlowFieldFlag_Identifier;
   sampleNode_init(pNode, pParent);
   stringCopy(pNode->node.identifier, GLOW_MAX_IDENTIFIER_LENGTH, pIdentifier);

   if(pDescription != NULL)
   {
      stringCopy(pNode->node.description, GLOW_MAX_DESCRIPTION_LENGTH, pDescription);
      fields |= GlowFieldFlag_Description;
   }

   pNode->fields = (GlowFieldFlags)fields;
   return pNode;
}

static SampleNode *createParameter(SampleNode *pParent, pcstr pIdentifier, pcstr pDescription)
{
   SampleNode *pNode = (SampleNode *)malloc(sizeof(SampleNode));
   dword fields = GlowFieldFlag_Identifier;
   sampleNode_init(pNode, pParent);
   pNode->isParameter = true;

   stringCopy(pNode->param.identifier, GLOW_MAX_IDENTIFIER_LENGTH, pIdentifier);

   if(pDescription != NULL)
   {
      stringCopy(pNode->param.description, GLOW_MAX_DESCRIPTION_LENGTH, pDescription);
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

   pParam->param.value.integer = 0;
   pParam->param.value.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Value;

   pParam->param.minimum.integer = -64;
   pParam->param.minimum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Minimum;

   pParam->param.maximum.integer = 15;
   pParam->param.maximum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Maximum;

   pParam->fields = (GlowFieldFlags)fields;
}

static void createVolume(SampleNode *pParent)
{
   SampleNode *pParam = createParameter(pParent, "volume", "loud?");
   dword fields = pParam->fields;

   pParam->param.access = GlowAccess_ReadWrite;
   fields |= GlowFieldFlag_Access;

   pParam->param.value.real = 0.0;
   pParam->param.value.flag = GlowParameterType_Real;
   fields |= GlowFieldFlag_Value;

   pParam->param.minimum.real = 0.0;
   pParam->param.minimum.flag = GlowParameterType_Real;
   fields |= GlowFieldFlag_Minimum;

   pParam->param.maximum.real = 1000.0;
   pParam->param.maximum.flag = GlowParameterType_Real;
   fields |= GlowFieldFlag_Maximum;

   pParam->fields = (GlowFieldFlags)fields;
}

static void createFormat(SampleNode *pParent)
{
   SampleNode *pParam = createParameter(pParent, "format", "simple enum");
   dword fields = pParam->fields;

   pParam->param.access = GlowAccess_ReadWrite;
   fields |= GlowFieldFlag_Access;

   pParam->param.value.integer = 0;
   pParam->param.value.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Value;

   pParam->param.enumeration = "4:3\n16:9\nHD\nFull HD\nCinema";
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
   pParam->param.value.integer = 0;
   fields |= GlowFieldFlag_Value;

   pParam->param.minimum.integer = -64 * 32;
   pParam->param.minimum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Minimum;

   pParam->param.maximum.integer = 15 * 32;
   pParam->param.maximum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Maximum;

   pParam->param.streamIdentifier = 0;
   fields |= GlowFieldFlag_StreamIdentifier;

   pParam->param.formula = "($/32 + log(2) - 1/5 + e^(1/4))\n($*32)";
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

   pParam->param.minimum.integer = -128 * 32;
   pParam->param.minimum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Minimum;

   pParam->param.maximum.integer = 127 * 32;
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

   pParam->param.minimum.integer = -255 * 32;
   pParam->param.minimum.flag = GlowParameterType_Integer;
   fields |= GlowFieldFlag_Minimum;

   pParam->param.maximum.integer = 0 * 32;
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

   pParam->param.maximum.integer = GLOW_MAX_VALUE_LENGTH - 1;
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
      nodeIndex = pPath[pathIndex] - 1;

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
      pOutPath = (berint *)malloc(MAX_GLOW_TREE_DEPTH);
      memcpy(pOutPath, pPath, pathLength * sizeof(berint));
      pBuffer = (byte *)malloc(bufferSize);
      glowOutput_init(&output, pBuffer, bufferSize, 0);

      if(pCursor->isParameter)
      {
         // parameter - send single complete parameter
         fields = pCursor->fields & pCommand->dirFieldMask;
         glowOutput_beginPackage(&output, true);
         glow_writeQualifiedParameter(&output, &pCursor->param, (GlowFieldFlags)fields, pOutPath, pathLength);
         send(sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
      }
      else if(pCursor->childrenCount == 0)
      {
         // leaf node - send single complete node
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
               pOutPath[pathLength] = nodeIndex + 1;
               fields = pCurrent->fields & pCommand->dirFieldMask;

               if(pCurrent->isParameter)
                  glow_writeQualifiedParameter(&output, &pCurrent->param, (GlowFieldFlags)fields, pOutPath, pathLength + 1);
               else
                  glow_writeQualifiedNode(&output, &pCurrent->node, (GlowFieldFlags)fields, pOutPath, pathLength + 1);

               send(sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
            }
         }
      }

      free(pBuffer);
      free(pOutPath);
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
      memcpy(&pCursor->param.value, &pParameter->value, sizeof(GlowValue));

      pBuffer = (byte *)malloc(bufferSize);
      glowOutput_init(&output, pBuffer, bufferSize, 0);
      glowOutput_beginPackage(&output, true);
      glow_writeQualifiedParameter(&output, &pCursor->param, GlowFieldFlag_Value, pPath, pathLength);
      send(sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
      free(pBuffer);
   }
}

static void onPackageReceived(const byte *pPackage, int length, voidptr state)
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
   const int valueBufferSize = 64; // max size of encoded primitive BER value
   const struct timeval timeout = {0, 50 * 1000}; // 50 milliseconds timeout for select()
   const int noDelay = 1;
   const int streamBufferSize = 128;
   fd_set fdset;
   int fdsReady;
   ClientInfo client;

   // allocate large objects on the heap
   GlowReader *pReader = (GlowReader *)malloc(sizeof(GlowReader));
   byte *pRxBuffer = (byte *)malloc(rxBufferSize);
   byte *pValueBuffer = (byte *)malloc(valueBufferSize);
   byte *pStreamBuffer = (byte *)malloc(streamBufferSize);

   srand((unsigned int)time(NULL));

   // turn off nagle algorithm for ppm data
   setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&noDelay, sizeof(noDelay));

   bzero(client);
   client.sock = sock;

   // only handle parameters and commands
   glowReader_init(pReader, pValueBuffer, valueBufferSize, NULL, onParameter, onCommand, NULL, (voidptr)&client, pRxBuffer, rxBufferSize);
   pReader->onPackageReceived = onPackageReceived;

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
   free(pStreamBuffer);
   free(pValueBuffer);
   free(pRxBuffer);
   free(pReader);
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

void runProvider(int argc, char **argv)
{
   int port;

   ember_init(onThrowError, onFailAssertion);
   initializePpmStreams();
   buildTree(&_root);

   if(argc >= 2)
   {
      port = atoi(argv[1]);

      printf_s("accepting client on port %d\n", port);
      acceptClient(port);
   }

   sampleNode_free(&_root);
}
