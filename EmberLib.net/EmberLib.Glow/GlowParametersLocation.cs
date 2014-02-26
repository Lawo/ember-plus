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

namespace EmberLib.Glow
{
   /// <summary>
   /// A type modelling a conceptual union of all types supported
   /// by the EmberPlus-Glow.MatrixContents.parametersLocation field.
   /// </summary>
   public class GlowParametersLocation
   {
      /// <summary>
      /// Creates a new instance of GlowParametersLocation, containing
      /// an integer array value for the "basePath" choice, which is
      /// encoded as RELATIVE-OID.
      /// Successive calls to property Kind will return GlowParametersLocationKind.BasePath.
      /// </summary>
      /// <param name="basePath">An integer array value for the "basePath" choice.</param>
      public GlowParametersLocation(int[] basePath)
      {
         Kind = GlowParametersLocationKind.BasePath;
         _basePath = basePath;
      }

      /// <summary>
      /// Creates a new instance of GlowParametersLocation, containing
      /// an integer value for the "inline" choice, which is
      /// encoded as INTEGER.
      /// Successive calls to property Kind will return GlowParametersLocationKind.Inline.
      /// </summary>
      /// <param name="inline">An integer array value for the "basePath" choice.</param>
      public GlowParametersLocation(int inline)
      {
         Kind = GlowParametersLocationKind.Inline;
         _inline = inline;
      }

      /// <summary>
      /// Gets the kind of this ParametersLocation object. Either BasePath or Inline.
      /// </summary>
      public GlowParametersLocationKind Kind { get; private set; }

      /// <summary>
      /// Gets the contained "basePath" value. Throws an exception if Kind is
      /// not GlowParametersLocationKind.BasePath.
      /// </summary>
      public int[] BasePath
      {
         get
         {
            if(Kind != GlowParametersLocationKind.BasePath)
               throw new InvalidOperationException("kind mismatch");

            return _basePath;
         }
      }

      /// <summary>
      /// Gets the contained "inline" value. Throws an exception if Kind is
      /// not GlowParametersLocationKind.Inline.
      /// </summary>
      public int Inline
      {
         get
         {
            if(Kind != GlowParametersLocationKind.Inline)
               throw new InvalidOperationException("kind mismatch");

            return _inline;
         }
      }

      #region Implementation
      int[] _basePath;
      int _inline;
      #endregion
   }

   /// <summary>
   /// Possible values for the GlowParametersLocation.Kind property.
   /// </summary>
   public enum GlowParametersLocationKind
   {
      BasePath,
      Inline,
   }
}
