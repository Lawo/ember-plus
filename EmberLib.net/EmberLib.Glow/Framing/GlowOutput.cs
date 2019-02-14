/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
      /// Constructs a new instance of GlowOutput
      /// </summary>
      /// <param name="useEscaping">Specifies whether the non-escaping variant of S101 shall be used.</param>
      /// <param name="maximumPackageLength">The maximum payload length of a package</param>
      /// <param name="slotId">The S101 slot id. Default is 0.</param>
      /// <param name="packageReadyHandler">A callback that is attached to the PackageReady event. May be null.</param>
      public GlowOutput(bool useEscaping, int maximumPackageLength, int slotId, EventHandler<PackageReadyArgs> packageReadyHandler)
      : base(useEscaping, maximumPackageLength, slotId, Dtd.Glow, packageReadyHandler)
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
      /// Constructs a new instance of GlowOutput with the maximum package length.
      /// </summary>
      /// <param name="useEscaping">Specifies whether the non-escaping variant of S101 shall be used.</param>
      /// <param name="slotId">The S101 slot id. Default is 0.</param>
      /// <param name="packageReadyHandler">A callback that is attached to the PackageReady event. May be null.</param>
      public GlowOutput(bool useEscaping, int slotId, EventHandler<PackageReadyArgs> packageReadyHandler)
      : base(useEscaping, ProtocolParameters.MaximumPackageLength, slotId, Dtd.Glow, packageReadyHandler)
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
