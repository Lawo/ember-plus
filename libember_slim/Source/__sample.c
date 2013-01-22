/*
   libember_slim sample
   Copyright (C) 2012  L-S-B Broadcast Technologies GmbH
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "emberglow.h"

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
            "libember_slim.exe -provider <listenPort>\n"
            "libember_slim.exe -consumer <remoteAddress> <remotePort>\n");
}

int main(int argc, char **argv)
{
   printf_s("libember_slim sample v%d.%d (EmBER Encoding %d.%d GlowDTD %d.%d)\n",
            VERSION >> 8,
            VERSION & 0xFF,
            EMBER_ENCODING_VERSION >> 8,
            EMBER_ENCODING_VERSION & 0xFF,
            GLOW_SCHEMA_VERSION >> 8,
            GLOW_SCHEMA_VERSION & 0xFF);

   //printf_s("GlowReader: %d\nNonFramingGlowReader: %d\nGlowParameter: %d\nGlowNode: %d\n",
   //         sizeof(GlowReader),
   //         sizeof(NonFramingGlowReader),
   //         sizeof(GlowParameter),
   //         sizeof(GlowNode));

   if(argc >= 2)
   {
      if(strcmp(argv[1], "-consumer") == 0)
      {
         if(argc >= 4)
         {
            runConsumer(argc - 1, argv + 1);
            return 0;
         }
      }
      else if(strcmp(argv[1], "-provider") == 0)
      {
         if(argc >= 3)
         {
            runProvider(argc - 1, argv + 1);
            return 0;
         }
      }
   }

   displayHelp();
   return 0;
}
