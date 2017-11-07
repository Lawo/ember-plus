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

using System.Collections.Generic;

namespace EmberLib.Framing.S101
{
   /// <summary>
   /// Used to encode S101 messages without the use of the S101 escaping.
   /// </summary>
   public sealed class TxFrameWithoutEscaping
   {
      /// <summary>
      /// Encode a memory block and return the encoded s101 message.
      /// </summary>
      /// <param name="package">the memory block to encode</param>
      /// <param name="length">the number of bytes in package to encode</param>
      /// <param name="newLength">the number of bytes in the encoded package</param>
      /// <returns>the encoded s101 package</returns>
      public static byte[] WriteMessage(IList<byte> package, int length, out int newLength)
      {
         newLength = 5 + length;

         var txBuffer = new byte[newLength];

         txBuffer[0] = Constants.Invalid;
         txBuffer[1] = (byte)((length >> 24) & 0xFF);
         txBuffer[2] = (byte)((length >> 16) & 0xFF);
         txBuffer[3] = (byte)((length >> 8) & 0xFF);
         txBuffer[4] = (byte)((length >> 0) & 0xFF);
         package.CopyTo(txBuffer, 5);

         return txBuffer;
      }
   }
}
