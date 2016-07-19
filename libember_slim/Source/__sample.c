/*
   libember_slim sample

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "emberplus.h"

#ifndef SECURE_CRT
#define printf_s printf
#endif

#define VERSION (0x0104)

void runConsumer(int argc, char **argv);
void runProvider(int argc, char **argv);

// ====================================================================
//
// entry point
//
// ====================================================================

static void displayHelp()
{
   printf_s("USAGE:\n"
            "emberplus.exe <remoteAddress> <remotePort>\n"
            "emberplus.exe -provider <listenPort>\n");
}

int main(int argc, char **argv)
{
   printf_s("emberplus v%d.%d (EmBER Encoding %d.%d GlowDTD %d.%d)\n",
            VERSION >> 8,
            VERSION & 0xFF,
            EMBER_ENCODING_VERSION >> 8,
            EMBER_ENCODING_VERSION & 0xFF,
            GLOW_SCHEMA_VERSION >> 8,
            GLOW_SCHEMA_VERSION & 0xFF);

#ifdef _DEBUG
   printf_s("GlowReader: %d\nNonFramingGlowReader: %d\nEmberAsyncReader: %d\nGlowParameter: %d\nGlowNode: %d\nGlowMatrix: %d\nGlowConnection: %d\n",
            sizeof(GlowReader),
            sizeof(NonFramingGlowReader),
            sizeof(EmberAsyncReader),
            sizeof(GlowParameter),
            sizeof(GlowNode),
            sizeof(GlowMatrix),
            sizeof(GlowConnection));
#endif

   if(argc >= 2)
   {
      if(strcmp(argv[1], "-provider") == 0)
      {
         if(argc >= 3)
         {
            runProvider(argc - 1, argv + 1);
            return 0;
         }
      }
      else
      {
         if(argc >= 3)
         {
            runConsumer(argc, argv);
            return 0;
         }
      }
   }

   displayHelp();
   return 0;
}
