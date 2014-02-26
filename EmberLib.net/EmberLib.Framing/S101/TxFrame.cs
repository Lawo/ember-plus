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
using System.IO;

namespace EmberLib.Framing.S101
{
   /// <summary>
   /// Used to encode S101 messages. This can be either done
   /// byte per byte (using an instance of TxFrame and calling WriteByte())
   /// or en block (using the static WriteMessage() method).
   /// </summary>
   public sealed class TxFrame : IDisposable
   {
      /// <summary>
      /// Primary ctor
      /// </summary>
      public TxFrame()
      {
         _buffer = new MemoryStream();

         Reset();
      }

      /// <summary>
      /// Writes a byte to the s101 message, taking care of crc calculation and escaping.
      /// </summary>
      /// <param name="b">The byte to write</param>
      public void WriteByte(byte b)
      {
         _crc = Crc.CrcCCITT16(_crc, b);

         WriteEscapedByte(b);
      }

      /// <summary>
      /// When writing byte per byte, call this method to finish
      /// the s101 message. The finished message is returned.
      /// </summary>
      /// <returns>The finished, encoded s101 message ready for transmit.</returns>
      public byte[] Finish()
      {
         var crc = ~_crc;

         WriteEscapedByte((byte)((crc >> 0) & 0xFF)); // lo8
         WriteEscapedByte((byte)((crc >> 8) & 0xFF)); // hi8
         _buffer.WriteByte(Constants.Eof);

         var memory = _buffer.ToArray();

         Reset();

         return memory;
      }

      /// <summary>
      /// Encode a memory block and return the encoded s101 message.
      /// </summary>
      /// <param name="package">the memory block to encode</param>
      /// <param name="length">the number of bytes in package to encode</param>
      /// <param name="newLength">the number of bytes in the encoded package</param>
      /// <returns>the encoded s101 package</returns>
      public static byte[] WriteMessage(IList<byte> package, int length, out int newLength)
      {
         byte data = 0;
         var txLength = 0;
         var txBuffer = new byte[6 + length * 2];
         var crc = (ushort)~Crc.CrcCCITT16(Crc.InitialValue, package, 0, length);

         txBuffer[txLength++] = Constants.Bof;

         for(int loop = 0; loop < length + 2; loop++)
         {
            if(loop == length)
            {
               data = (byte)((crc >> 0) & 0xFF); // lo8
            }
            else if(loop == length + 1)
            {
               data = (byte)((crc >> 8) & 0xFF); // hi8
            }
            else
            {
               data = package[loop];
            }

            if(data >= Constants.Invalid)
            {
               txBuffer[txLength++] = Constants.Ce;
               data ^= 0x20;
            }

            txBuffer[txLength++] = data;
         }

         txBuffer[txLength++] = Constants.Eof;

         newLength = txLength;
         return txBuffer;
      }

      #region Implementation
      MemoryStream _buffer;
      ushort _crc;

      void Reset()
      {
         _buffer.SetLength(0);
         _crc = Crc.InitialValue;
      }

      void WriteEscapedByte(byte b)
      {
         if(_buffer.Length == 0)
            _buffer.WriteByte(Constants.Bof);

         if(b >= Constants.Invalid)
         {
            _buffer.WriteByte(Constants.Ce);
            _buffer.WriteByte((byte)(b ^ 0x20));
         }
         else
         {
            _buffer.WriteByte(b);
         }
      }
      #endregion

      #region IDisposable Members
      public void Dispose()
      {
         if(_buffer != null)
            _buffer.Dispose();

         _buffer = null;
      }
      #endregion
   }
}
