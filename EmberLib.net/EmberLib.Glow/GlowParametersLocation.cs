/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
