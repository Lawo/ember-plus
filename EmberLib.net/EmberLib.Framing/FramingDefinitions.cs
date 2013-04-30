/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol
   Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

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

namespace EmberLib.Framing
{
   /// <summary>
   /// Defines possible values for the "dtd" field in the protocol frame
   /// </summary>
   public enum Dtd : byte
   {
      Glow = 1,
   }

   /// <summary>
   /// Constants used in the protocol frame
   /// </summary>
   public static class ProtocolParameters
   {
      /// <summary>
      /// The message id that identifies an Ember+ package
      /// </summary>
      public const byte MessageId = 0x0E;

      public const byte FirstPackageFlag = 0x80;
      public const byte LastPackageFlag = 0x40;
      public const byte EmptyPackageFlag = 0x20;

      /// <summary>
      /// The maximum size of an Ember+ package
      /// </summary>
      public const int MaximumPackageLength = 1024;

      /// <summary>
      /// The length of the framing header without any application-defined header bytes.
      /// </summary>
      public const int MinimumHeaderLength = 7;

      /// <summary>
      /// Possible values for the "command" field in the framing header.
      /// </summary>
      public static class Commands
      {
         public const byte Payload = 0;
         public const byte KeepAliveRequest = 1;
         public const byte KeepAliveResponse = 2;
         public const byte ProviderState = 3;
      }

      /// <summary>
      /// Possible values for the "providerState" field in the framing header.
      /// </summary>
      public static class ProviderState
      {
         public const byte Passive = 0;
         public const byte Active = 1;
      }
   }
}
