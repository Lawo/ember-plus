/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
         newLength = 6 + length;

         var txBuffer = new byte[newLength];

         txBuffer[0] = Constants.Invalid;
         txBuffer[1] = 0x04;
         txBuffer[2] = (byte)((length >> 24) & 0xFF);
         txBuffer[3] = (byte)((length >> 16) & 0xFF);
         txBuffer[4] = (byte)((length >> 8) & 0xFF);
         txBuffer[5] = (byte)((length >> 0) & 0xFF);
         package.CopyTo(txBuffer, 6);

         return txBuffer;
      }
   }
}
