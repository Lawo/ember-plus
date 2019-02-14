/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
