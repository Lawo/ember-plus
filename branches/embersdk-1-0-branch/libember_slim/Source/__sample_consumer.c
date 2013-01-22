/*
   libember_slim sample
   Copyright (C) 2012  L-S-B Broadcast Technologies GmbH
*/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <limits.h>

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
#define sscanf_s sscanf
#else
#define stringCopy(dest, size, source) \
   do { strncpy_s(dest, size, source, (size) - 1); dest[(size) - 1] = 0; } while(0)
#endif

#define newobj(type) ((type *)malloc(sizeof(type)))
#define newarr(type, size) ((type *)malloc(sizeof(type) * (size)))


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


// ====================================================================
//
// Model
//
// ====================================================================

#define ELEMENT_MAX_CHILDREN (100)

typedef struct SElement
{
   berint number;
   bool isParameter;
   GlowFieldFlags paramFields;

   union
   {
      GlowNode node;
      GlowParameter param;
   };

   int childrenCount;
   struct SElement *children[ELEMENT_MAX_CHILDREN];
   struct SElement *pParent;
} Element;

static void element_init(Element *pThis, Element *pParent, bool isParameter, berint number)
{
   bzero(*pThis);

   pThis->pParent = pParent;
   pThis->isParameter = isParameter;
   pThis->number = number;

   if(pParent != NULL)
   {
      if(pParent->childrenCount < ELEMENT_MAX_CHILDREN)
      {
         pParent->children[pParent->childrenCount] = pThis;
         pParent->childrenCount++;
      }
   }
}

static void element_free(Element *pThis)
{
   int index;
   Element *pChild;

   for(index = 0; index < pThis->childrenCount; index++)
   {
      pChild = pThis->children[index];

      if(pChild != NULL)
      {
         element_free(pChild);
         free(pChild);
      }
   }

   if(pThis->isParameter)
   {
      if(pThis->param.enumeration != NULL)
         free((void *)pThis->param.enumeration);
      if(pThis->param.formula != NULL)
         free((void *)pThis->param.formula);
      if(pThis->param.format != NULL)
         free((void *)pThis->param.format);
   }

   bzero(*pThis);
}

static berint *element_getPath(const Element *pThis, berint *pBuffer, int *pCount)
{
   const Element *pElement;
   berint *pPosition = &pBuffer[*pCount];
   int count = 0;

   for(pElement = pThis; pElement->pParent != NULL; pElement = pElement->pParent)
   {
      pPosition--;

      if(pPosition < pBuffer)
         return NULL;

      *pPosition = pElement->number;
      count++;

   }

   *pCount = count;
   return pPosition;
}

static pcstr element_getIdentifierPath(const Element *pThis, pstr pBuffer, int bufferSize)
{
   const Element *pElement;
   pstr pPosition = &pBuffer[bufferSize - 1];
   int length;
   pcstr pIdentifier;

   *pPosition = 0;

   for(pElement = pThis; pElement->pParent != NULL; pElement = pElement->pParent)
   {
      if(*pPosition != 0)
         *--pPosition = '/';

      if(pElement->isParameter)
         pIdentifier = pElement->param.identifier;
      else
         pIdentifier = pElement->node.identifier;

      length = strlen(pIdentifier);
      pPosition -= length;

      if(pPosition < pBuffer)
         return NULL;

      memcpy(pPosition, pIdentifier, length);
   }

   return pPosition;
}

static Element *element_findChild(const Element *pThis, berint number)
{
   int index;
   Element *pChild;

   for(index = 0; index < pThis->childrenCount; index++)
   {
      pChild = pThis->children[index];

      if(pChild->number == number)
         return pChild;
   }

   return NULL;
}

static Element *element_findChildByIdentifier(const Element *pThis, pcstr pIdentifier)
{
   int index;
   Element *pChild;
   pcstr pIdent;

   for(index = 0; index < pThis->childrenCount; index++)
   {
      pChild = pThis->children[index];

      if(pChild->isParameter)
         pIdent = pChild->param.identifier;
      else
         pIdent = pChild->node.identifier;

      if(_stricmp(pIdent, pIdentifier) == 0)
         return pChild;
   }

   return NULL;
}

static Element *element_findDescendant(const Element *pThis, const berint *pPath, int pathLength, Element **ppParent)
{
   int index;
   Element *pElement = (Element *)pThis;
   Element *pParent = NULL;

   for(index = 0; index < pathLength; index++)
   {
      if(pElement != NULL)
         pParent = pElement;

      pElement = element_findChild(pElement, pPath[index]);

      if(pElement == NULL)
      {
         if(index < pathLength - 1)
            pParent = NULL;

         break;
      }
   }

   if(ppParent != NULL)
      *ppParent = pParent;

   return pElement;
}

static GlowParameterType element_getParameterType(const Element *pThis)
{
   if(pThis->isParameter)
   {
      if(pThis->paramFields & GlowFieldFlag_Enumeration)
         return GlowParameterType_Enum;

      if(pThis->paramFields & GlowFieldFlag_Value)
         return pThis->param.value.flag;

      if(pThis->paramFields & GlowFieldFlag_Type)
         return pThis->param.type;
   }

   return (GlowParameterType)0;
}

static void printValue(const GlowValue *pValue)
{
   switch(pValue->flag)
   {
      case GlowParameterType_Integer:
         printf_s("integer '%lld'", pValue->integer);
         break;

      case GlowParameterType_Real:
         printf_s("real '%lf'", pValue->real);
         break;

      case GlowParameterType_String:
         printf_s("string '%s'", pValue->string);
         break;

      case GlowParameterType_Boolean:
         printf_s("boolean '%d'", pValue->boolean);
         break;

      case GlowParameterType_Octets:
         printf_s("octets length %d", pValue->octets.length);
         break;

      default:
         break;
   }
}

static void printMinMax(const GlowMinMax *pMinMax)
{
   switch(pMinMax->flag)
   {
      case GlowParameterType_Integer:
         printf_s("%lld", pMinMax->integer);
         break;

      case GlowParameterType_Real:
         printf_s("%lf", pMinMax->real);
         break;

      default:
         break;
   }
}

static void element_print(const Element *pElement, bool isVerbose)
{
   GlowFieldFlags fields;
   const GlowParameter *pParameter;

   if(pElement->isParameter)
   {
      pParameter = &pElement->param;
      printf_s("P %04ld %s\n", pElement->number, pParameter->identifier);

      if(isVerbose)
      {
         fields = pElement->paramFields;

         if(fields & GlowFieldFlag_Description)
            printf_s("  description:      %s\n", pParameter->description);

         if(fields & GlowFieldFlag_Value)
         {
            printf_s("  value:            ");
            printValue(&pParameter->value);
            printf_s("\n");
         }

         if(fields & GlowFieldFlag_Minimum)
         {
            printf_s("  minimum:          ");
            printMinMax(&pParameter->minimum);
            printf_s("\n");
         }

         if(fields & GlowFieldFlag_Maximum)
         {
            printf_s("  maximum:          ");
            printMinMax(&pParameter->maximum);
            printf_s("\n");
         }

         if(fields & GlowFieldFlag_Access)
            printf_s("  access:           %d\n", pParameter->access);
         if(fields & GlowFieldFlag_Factor)
            printf_s("  factor:           %d\n", pParameter->factor);
         if(fields & GlowFieldFlag_IsOnline)
            printf_s("  isOnline:         %d\n", pParameter->isOnline);
         if(fields & GlowFieldFlag_Step)
            printf_s("  step:             %d\n", pParameter->step);
         if(fields & GlowFieldFlag_Type)
            printf_s("  type:             %d\n", pParameter->type);
         if(fields & GlowFieldFlag_StreamIdentifier)
            printf_s("  streamIdentifier: %ld\n", pParameter->streamIdentifier);

         if(fields & GlowFieldFlag_StreamDescriptor)
         {
            printf_s("  streamDescriptor:\n");
            printf_s("    format:         %d\n", pParameter->streamDescriptor.format);
            printf_s("    offset:         %d\n", pParameter->streamDescriptor.offset);
         }

         if(pParameter->enumeration != NULL)
            printf_s("  enumeration:\n%s\n", pParameter->enumeration);
         if(pParameter->format != NULL)
            printf_s("  format:           %s\n", pParameter->format);
         if(pParameter->formula != NULL)
            printf_s("  formula:\n%s\n", pParameter->formula);
      }
   }
   else
   {
      printf_s("N %04ld %s\n", pElement->number, pElement->node.identifier);

      if(isVerbose)
         printf_s("  description: %s\n", pElement->node.description);
   }
}


// ====================================================================
//
// glow handlers
//
// ====================================================================

typedef struct __Session
{
   SOCKET sock;
   pcstr remoteAddress;
   int remotePort;
   Element root;
   Element *pCursor;
   berint cursorPathBuffer[MAX_GLOW_TREE_DEPTH];
   berint *pCursorPath;
   int cursorPathLength;

   // some reading state
   pstr pEnumeration;
   pstr pFormula;
   pstr pFormat;
} Session;

static void onNode(const GlowNode *pNode, const berint *pPath, int pathLength, voidptr state)
{
   Session *pSession = (Session *)state;
   Element *pElement;
   Element *pParent;

   // if received element is a child of current cursor, print it
   if(memcmp(pPath, pSession->pCursorPath, pSession->cursorPathLength * sizeof(berint)) == 0
   && pathLength == pSession->cursorPathLength + 1)
      printf_s("* N %04ld %s\n", pPath[pathLength - 1], pNode->identifier);

   if(pNode->identifier[0] != 0)
   {
      pElement = element_findDescendant(&pSession->root, pPath, pathLength, &pParent);

      if(pParent != NULL)
      {
         if(pElement == NULL)
         {
            pElement = newobj(Element);
            element_init(pElement, pParent, false, pPath[pathLength - 1]);
            stringCopy(pElement->node.identifier, GLOW_MAX_IDENTIFIER_LENGTH, pNode->identifier);
         }

         if(pNode->description[0] != 0)
            stringCopy(pElement->node.description, GLOW_MAX_DESCRIPTION_LENGTH, pNode->description);
      }
   }
}

static void onParameter(const GlowParameter *pParameter, GlowFieldFlags fields, const berint *pPath, int pathLength, voidptr state)
{
   Session *pSession = (Session *)state;
   Element *pElement;
   Element *pParent;

   // if received element is a child of current cursor, print it
   if(memcmp(pPath, pSession->pCursorPath, pSession->cursorPathLength * sizeof(berint)) == 0
   && pathLength == pSession->cursorPathLength + 1)
      printf_s("* P %04ld %s\n", pPath[pathLength - 1], pParameter->identifier);

   pElement = element_findDescendant(&pSession->root, pPath, pathLength, &pParent);

   if(pParent != NULL)
   {
      if(pElement == NULL)
      {
         pElement = newobj(Element);
         element_init(pElement, pParent, true, pPath[pathLength - 1]);
      }

      if(fields & GlowFieldFlag_Identifier)
         stringCopy(pElement->param.identifier, GLOW_MAX_IDENTIFIER_LENGTH, pParameter->identifier);
      if(fields & GlowFieldFlag_Description)
         stringCopy(pElement->param.description, GLOW_MAX_DESCRIPTION_LENGTH, pParameter->description);
      if(fields & GlowFieldFlag_Value)
         memcpy(&pElement->param.value, &pParameter->value, sizeof(GlowValue));
      if(fields & GlowFieldFlag_Minimum)
         memcpy(&pElement->param.minimum, &pParameter->minimum, sizeof(GlowMinMax));
      if(fields & GlowFieldFlag_Maximum)
         memcpy(&pElement->param.maximum, &pParameter->maximum, sizeof(GlowMinMax));
      if(fields & GlowFieldFlag_Access)
         pElement->param.access = pParameter->access;
      if(fields & GlowFieldFlag_Factor)
         pElement->param.factor = pParameter->factor;
      if(fields & GlowFieldFlag_IsOnline)
         pElement->param.isOnline = pParameter->isOnline;
      if(fields & GlowFieldFlag_Step)
         pElement->param.step = pParameter->step;
      if(fields & GlowFieldFlag_Type)
         pElement->param.type = pParameter->type;
      if(fields & GlowFieldFlag_StreamIdentifier)
         pElement->param.streamIdentifier = pParameter->streamIdentifier;
      if(fields & GlowFieldFlag_StreamDescriptor)
         memcpy(&pElement->param.streamDescriptor, &pParameter->streamDescriptor, sizeof(GlowStreamDescription));

      if(pSession->pEnumeration != NULL)
      {
         pElement->param.enumeration = pSession->pEnumeration;
         pSession->pEnumeration = NULL;
      }
      if(pSession->pFormat != NULL)
      {
         pElement->param.format = pSession->pFormat;
         pSession->pFormat = NULL;
      }
      if(pSession->pFormula != NULL)
      {
         pElement->param.formula = pSession->pFormula;
         pSession->pFormula = NULL;
      }

      pElement->paramFields = (GlowFieldFlags)(pElement->paramFields | fields);

      // if cursor parameter has updated value, print value
      if(pElement == pSession->pCursor
      && (fields & GlowFieldFlag_Value))
      {
         printValue(&pParameter->value);
         printf_s("\n");
      }
   }
}

static void onPackageReceived(const byte *pPackage, int length, voidptr state)
{
   Session *pSession = (Session *)state;
   SOCKET sock = pSession->sock;
   byte buffer[16];
   unsigned int txLength;

   if(length >= 4
   && pPackage[1] == EMBER_MESSAGE_ID
   && pPackage[2] == EMBER_COMMAND_KEEPALIVE_REQUEST)
   {
      txLength = emberFraming_writeKeepAliveResponse(buffer, sizeof(buffer), pPackage[0]);
      send(sock, (char *)buffer, txLength, 0);
   }
}

void onUnsupportedTltlv(const BerReader *pReader, const berint *pPath, int pathLength, GlowReaderPosition position, voidptr state)
{
   Session *pSession = (Session *)state;
   NonFramingGlowReader *pGlowReader = (NonFramingGlowReader *)pReader;

   if(position == GlowReaderPosition_ParameterContents)
   {
      if(berTag_equals(&pReader->tag, &glowTags.parameterContents.enumeration))
      {
         pSession->pEnumeration = newarr(char, pReader->length + 1);
         berReader_getString(pReader, pSession->pEnumeration, pReader->length);
      }
      else if(berTag_equals(&pReader->tag, &glowTags.parameterContents.formula))
      {
         pSession->pFormula = newarr(char, pReader->length + 1);
         berReader_getString(pReader, pSession->pFormula, pReader->length);
      }
      else if(berTag_equals(&pReader->tag, &glowTags.parameterContents.format))
      {
         pSession->pFormat = newarr(char, pReader->length + 1);
         berReader_getString(pReader, pSession->pFormat, pReader->length);
      }
   }
}


// ====================================================================
//
// main loop
//
// ====================================================================

static void writePrompt(Session *pSession)
{
   const int identPathBufferSize = 256;
   pstr pIdentPathBuffer;
   pcstr pIdentPath;

   pIdentPathBuffer = newarr(char, identPathBufferSize);
   pIdentPath = element_getIdentifierPath(pSession->pCursor, pIdentPathBuffer, identPathBufferSize);

   printf_s("%s:%d/", pSession->remoteAddress, pSession->remotePort);

   if(pIdentPath != NULL)
      printf_s("%s", pIdentPath);

   printf_s(">");

   free(pIdentPathBuffer);
}

static Element *findElement(pcstr pIdentifier, const Session *pSession)
{
   if(strcmp(pIdentifier, ".") == 0)
   {
      return pSession->pCursor;
   }
   else if(strcmp(pIdentifier, "..") == 0)
   {
      return pSession->pCursor->pParent;
   }

   return element_findChildByIdentifier(pSession->pCursor, pIdentifier);
}

static bool setParameterValue(const Session *pSession, pcstr pValueString)
{
   GlowOutput output;
   const int bufferSize = 512;
   byte *pBuffer;
   const Element *pElement = pSession->pCursor;
   GlowParameterType type = element_getParameterType(pElement);
   GlowParameter parameter;

   bzero(parameter);

   switch(type)
   {
      case GlowParameterType_Integer:
      case GlowParameterType_Enum:
         sscanf_s(pValueString, "%lld", &parameter.value.integer);
         parameter.value.flag = GlowParameterType_Integer;
         break;

      case GlowParameterType_Boolean:
         parameter.value.boolean = atoi(pValueString) != 0;
         parameter.value.flag = GlowParameterType_Boolean;
         break;

      case GlowParameterType_Real:
         sscanf_s(pValueString, "%lf", &parameter.value.real);
         parameter.value.flag = GlowParameterType_Real;
         break;

      case GlowParameterType_String:
         stringCopy(parameter.value.string, sizeof(parameter.value.string), pValueString);
         parameter.value.flag = GlowParameterType_String;
         break;

      default:
         return false;
   }

   pBuffer = newarr(byte, bufferSize);
   glowOutput_init(&output, pBuffer, bufferSize, 0);
   glowOutput_beginPackage(&output, true);
   glow_writeQualifiedParameter(&output, &parameter, GlowFieldFlag_Value, pSession->pCursorPath, pSession->cursorPathLength);
   send(pSession->sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);
   free(pBuffer);
   return true;
}

static bool handleInput(Session *pSession, pcstr pInput)
{
   GlowOutput output;
   const int bufferSize = 512;
   byte *pBuffer;
   GlowCommand command;
   pcstr pArgument;
   Element *pElement;
   int index;

   if(strcmp(pInput, "dir") == 0)
   {
      pBuffer = newarr(byte, bufferSize);

      bzero(command);
      command.number = GlowCommandType_GetDirectory;

      glowOutput_init(&output, pBuffer, bufferSize, 0);
      glowOutput_beginPackage(&output, true);
      glow_writeQualifiedCommand(&output, &command, pSession->pCursorPath, pSession->cursorPathLength, pSession->pCursor->isParameter);
      send(pSession->sock, (char *)pBuffer, glowOutput_finishPackage(&output), 0);

      free(pBuffer);

      printf_s("|>\n");
   }
   else if(strcmp(pInput, "ls") == 0)
   {
      for(index = 0; index < pSession->pCursor->childrenCount; index++)
         element_print(pSession->pCursor->children[index], false);

      writePrompt(pSession);
   }
   else if(strncmp(pInput, "cd ", 3) == 0)
   {
      pArgument = &pInput[3];
      pElement = findElement(pArgument, pSession);

      if(pElement != NULL)
      {
         pSession->cursorPathLength = MAX_GLOW_TREE_DEPTH;
         pSession->pCursorPath = element_getPath(pElement, pSession->cursorPathBuffer, &pSession->cursorPathLength);
         pSession->pCursor = pElement;
      }

      writePrompt(pSession);
   }
   else if(strcmp(pInput, "cd..") == 0)
   {
      pElement = pSession->pCursor->pParent;

      if(pElement != NULL)
      {
         pSession->cursorPathLength = MAX_GLOW_TREE_DEPTH;
         pSession->pCursorPath = element_getPath(pElement, pSession->cursorPathBuffer, &pSession->cursorPathLength);
         pSession->pCursor = pElement;
      }
   
      writePrompt(pSession);
   }
   else if(strncmp(pInput, "print ", 6) == 0)
   {
      pArgument = &pInput[6];
      pElement = findElement(pArgument, pSession);

      if(pElement != NULL)
         element_print(pElement, true);

      writePrompt(pSession);
   }
   else if(strcmp(pInput, "print") == 0)
   {
      element_print(pSession->pCursor, true);

      writePrompt(pSession);
   }
   else if(strncmp(pInput, "set ", 4) == 0)
   {
      pArgument = &pInput[4];

      if(setParameterValue(pSession, pArgument))
         printf_s("|>\n");
      else
         writePrompt(pSession);
   }
   else if(*pInput == 0)
   {
      writePrompt(pSession);
   }
   else if(strcmp(pInput, "?") == 0)
   {
      printf_s("dir             - send GetDirectory command\n"
               "cd <ident>      - change to child with identifier <ident>\n"
               "ls              - list children\n"
               "print [<ident>] - print child with identifier <ident>\n"
               "set <value>     - set parameter value to <value>\n"
               "quit            - exit application\n"
               "for <ident> you can also write '.' or '..'.\n");

      writePrompt(pSession);
   }
   else if(strcmp(pInput, "quit") == 0)
   {
      return true;
   }
   else
   {
      printf_s("Unknown command\n");
      writePrompt(pSession);
   }

   return false;
}

static void run(Session *pSession)
{
   static char s_input[256];
   byte buffer[64];
   int read;
   char ch;
   int inputLength = 0;
   bool isQuit = false;
   const struct timeval timeout = {0, 16 * 1000}; // 16 milliseconds timeout for select()
   const int rxBufferSize = 64 * 1024; // max size of unframed package
   const int valueBufferSize = 64; // max size of encoded primitive BER value
   fd_set fdset;
   int fdsReady;
   GlowReader *pReader = newobj(GlowReader);
   byte *pRxBuffer = newarr(byte, rxBufferSize);
   byte *pValueBuffer = newarr(byte, valueBufferSize);
   SOCKET sock = pSession->sock;

   glowReader_init(pReader, pValueBuffer, valueBufferSize, onNode, onParameter, NULL, NULL, (voidptr)pSession, pRxBuffer, rxBufferSize);
   pReader->onPackageReceived = onPackageReceived;
   pReader->base.onUnsupportedTltlv = onUnsupportedTltlv;

   printf_s("Enter '?' for help\n");
   writePrompt(pSession);

   while(isQuit == false)
   {
      if(_kbhit())
      {
         ch = (char)_getch();
         _putch(ch);

         if(ch == '\n' || ch == '\r')
         {
            printf_s("\n");

            s_input[inputLength] = 0;
            isQuit = handleInput(pSession, s_input);

            inputLength = 0;
         }
         else if(ch == '\b')
         {
            if(inputLength > 0)
               inputLength--;
         }
         else
         {
            if(inputLength < sizeof(s_input) - 1)
            {
               s_input[inputLength] = ch;
               inputLength++;
            }
         }
      }
      else
      {
         FD_ZERO(&fdset);
         FD_SET(sock, &fdset);

         fdsReady = select(1, &fdset, NULL, NULL, &timeout);

         if(fdsReady == 1) // socket is ready to read
         {
            if(FD_ISSET(sock, &fdset))
            {
               read = recv(sock, (char *)buffer, sizeof(buffer), 0);

               if(read > 0)
                  glowReader_readBytes(pReader, buffer, read);
               else
                  isQuit = true;
            }
         }
         else if(fdsReady < 0) // connection lost
         {
            isQuit = true;
         }
      }
   }

   free(pValueBuffer);
   free(pRxBuffer);
   free(pReader);
}


// ====================================================================
//
// consumer sample entry point
//
// ====================================================================

void runConsumer(int argc, char **argv)
{
   int port;
   struct sockaddr_in remoteAddr;
   unsigned long uladdr;
   SOCKET sock;
   int result;
   Session session;

   ember_init(onThrowError, onFailAssertion);

   initSockets();

   if(argc >= 3)
   {
      uladdr = inet_addr(argv[1]);
      port = atoi(argv[2]);

      if(port > 0
      && uladdr != INADDR_NONE
      && uladdr != INADDR_ANY)
      {
         remoteAddr.sin_family = AF_INET;
         remoteAddr.sin_port = htons((unsigned short)(port & 0xFFFF));
         remoteAddr.sin_addr.S_un.S_addr = uladdr;

         printf_s("connecting to %s:%d...\n", inet_ntoa(remoteAddr.sin_addr), port);

         sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
         result = connect(sock, (struct sockaddr *)&remoteAddr, sizeof(remoteAddr));

         if(result != SOCKET_ERROR)
         {
            bzero(session);
            session.sock = sock;
            session.pCursor = &session.root;
            session.remoteAddress = argv[1];
            session.remotePort = port;
            element_init(&session.root, NULL, false, 0);

            run(&session);

            element_free(&session.root);
            closesocket(sock);
         }
         else
         {
            printf_s("connect error.\n");
         }
      }
      else
      {
         printf_s("address or port error.\n");
      }
   }

   shutdownSockets();
}
