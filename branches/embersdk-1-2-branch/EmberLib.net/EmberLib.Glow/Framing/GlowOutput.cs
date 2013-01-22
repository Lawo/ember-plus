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
using EmberLib.Framing;

namespace EmberLib.Glow.Framing
{
   public class GlowOutput : FramingBerOutput
   {
      /// <summary>
      /// Constructs a new instance of GlowOutput
      /// </summary>
      /// <param name="maximumPackageLength">The maximum payload length of a package</param>
      /// <param name="slotId">The S101 slot id. Default is 0.</param>
      /// <param name="packageReadyHandler">A callback that is attached to the PackageReady event. May be null.</param>
      public GlowOutput(int maximumPackageLength, int slotId, EventHandler<PackageReadyArgs> packageReadyHandler)
      : base(maximumPackageLength, slotId, Dtd.Glow, packageReadyHandler)
      {
         _applicationBytes = CreateApplicationBytes();
      }

      /// <summary>
      /// Constructs a new instance of GlowOutput with the maximum package length.
      /// </summary>
      /// <param name="slotId">The S101 slot id. Default is 0.</param>
      /// <param name="packageReadyHandler">A callback that is attached to the PackageReady event. May be null.</param>
      public GlowOutput(int slotId, EventHandler<PackageReadyArgs> packageReadyHandler)
      : base(ProtocolParameters.MaximumPackageLength, slotId, Dtd.Glow, packageReadyHandler)
      {
         _applicationBytes = CreateApplicationBytes();
      }

      /// <summary>
      /// Overridden to return glow-specific application bytes:
      /// 0 - version lo8
      /// 1 - version hi8
      /// </summary>
      protected override byte[] GetApplicationBytes(out byte length)
      {
         length = (byte)(_applicationBytes.Length & 0xFF);
         return _applicationBytes;
      }

      #region Implementation
      readonly byte[] _applicationBytes;

      static byte[] CreateApplicationBytes()
      {
         var version = GlowDtd.Version;

         return new byte[]
         {
            (byte)((version >> 0) & 0xFF),
            (byte)((version >> 8) & 0xFF),
         };
      }
      #endregion
   }
}
