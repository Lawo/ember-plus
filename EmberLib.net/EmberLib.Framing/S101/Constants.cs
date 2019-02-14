/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

using System;
using System.Collections.Generic;
using System.Text;

namespace EmberLib.Framing.S101
{
   class Constants
   {
      public const byte Bof = 0xFE;
      public const byte Eof = 0xFF;
      public const byte Ce = 0xFD;

      public const byte Syn = 0xFC;
      public const byte Enq = 0xFB;

      public const byte Ack = 0xFA;
      public const byte Nak = 0xF9;

      public const byte PadBytes = 2;
      public const byte Clients = 0xF0;
      public const byte Invalid = 0xF8;

      public static readonly byte[] Eofs = { Eof, Eof, Eof };
      public static readonly byte[] Bofs = { Bof };
   }
}
