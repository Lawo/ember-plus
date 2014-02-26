/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol
   Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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
