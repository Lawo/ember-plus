/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

using System;
using System.Collections.Generic;
using System.Text;
using BerLib;

namespace EmberLib.Glow
{
   /// <summary>
   /// EmberPlus-Glow.StreamDescription [APPLICATION 12] Type
   /// </summary>
   public class GlowStreamDescription : GlowSubContainer
   {
      /// <summary>
      /// Creates a new instance of GlowContainer.
      /// </summary>
      protected internal GlowStreamDescription(BerTag tag)
      : base(tag, GlowType.StreamDescription)
      {
      }

      /// <summary>
      /// Creates an encodable instance of GlowStreamDescription
      /// with the specified format, size and offset.
      /// </summary>
      /// <param name="tag">The field tag of the StreamDescription.</param>
      /// <param name="format">The format of the stream. Usually one of the
      /// values defined in EmberLib.Glow.GlowStreamFormat.</param>
      /// <param name="offset">Byte offset of the value in the stream.</param>
      public GlowStreamDescription(BerTag tag, int format, int offset)
      : base(tag, GlowType.StreamDescription)
      {
         Format = format;
         Offset = offset;
      }

      /// <summary>
      /// Gets or sets the "format" field.
      /// Possible values are defined in class EmberLib.Glow.GlowStreamFormat.
      /// </summary>
      public int Format
      {
         get { return GetChildValue<int>(GlowTags.StreamDescription.Format); }
         set
         {
            var tag = GlowTags.StreamDescription.Format;

            Remove(tag);
            Insert(new IntegerEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "offset" field
      /// </summary>
      public int Offset
      {
         get { return GetChildValue<int>(GlowTags.StreamDescription.Offset); }
         set
         {
            var tag = GlowTags.StreamDescription.Offset;

            Remove(tag);
            Insert(new IntegerEmberLeaf(tag, value));
         }
      }
   }
}
