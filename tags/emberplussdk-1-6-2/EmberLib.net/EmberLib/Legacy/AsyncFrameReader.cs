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
using System.Diagnostics;
using BerLib;


namespace EmberLib.Legacy
{
   [Obsolete("This class is a feature exclusively used by the Glow DTD version 1. Please consider using Glow DTD version 2 or higher.")]
   public class AsyncFrameReader : AsyncDomReader
   {
      internal static readonly byte[] FrameBeginBytes =
      {
         0x7F, 0x8F, 0xFF, 0xFE, 0xD9, 0x5C, // frame tag
         0x80,                               // indefinite length
         0x30,                               // type sequence
         0x80,                               // indefinite length
      };

      int _index;
      ushort _crc;

      public AsyncFrameReader(EmberApplicationInterface application)
      : base(application)
      {
         ResetFraming();
      }

      public AsyncFrameReader()
      : this(null)
      {
         ResetFraming();
      }

      void ResetFraming()
      {
         _index = 0;
         _crc = 0xFFFF;
      }

      public override void ReadByte(byte b)
      {
         if(_index >= 0)
         {
            if(b == FrameBeginBytes[_index])
            {
               _index++;

               if(_index == FrameBeginBytes.Length)
               {
                  _index = -1;
                  _crc = Crc.CrcCCITT16(_crc, FrameBeginBytes, 0, FrameBeginBytes.Length);

                  base.ReadBytes(FrameBeginBytes);
               }
            }
            else
            {
               ResetFraming();
            }

            return;
         }

         try
         {
            _crc = Crc.CrcCCITT16(_crc, b);

            base.ReadByte(b);
         }
         catch(BerException oException)
         {
            OnError(oException);

            Reset();

            ResetFraming();
         }
      }

      protected override sealed void OnRootReady(RootReadyArgs oArgs)
      {
         base.OnRootReady(oArgs);

         ResetFraming();
      }

      protected virtual void OnError(BerException ex)
      {
         Debug.WriteLine(ex);
      }
   }
}
