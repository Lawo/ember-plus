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
   /// EmberPlus-Glow.StreamEntry [APPLICATION 5] Type
   /// </summary>
   public class GlowStreamEntry : GlowSubContainer
   {
      /// <summary>
      /// Creates a new instance of GlowStreamEntry.
      /// <param name="tag">Tag of the new GlowStreamEntry or
      /// GlowTags.StreamCollection.StreamEntry if null.</param>
      /// </summary>
      public GlowStreamEntry(BerTag? tag = null)
      : base(tag ?? GlowTags.StreamCollection.StreamEntry, GlowType.StreamEntry)
      {
      }

      /// <summary>
      /// Gets or sets the "streamIdentifier" field
      /// </summary>
      public int StreamIdentifier
      {
         get{ return GetChildValue<int>(GlowTags.StreamEntry.StreamIdentifier); }
         set
         {
            var tag = GlowTags.StreamEntry.StreamIdentifier;

            Remove(tag);
            Insert(new IntegerEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "value" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public GlowValue StreamValue
      {
         get { return InternalTools.GetValue(this, GlowTags.StreamEntry.StreamValue); }
         set
         {
            var tag = GlowTags.StreamEntry.StreamValue;

            Remove(tag);
            InternalTools.InsertValue(this, tag, value);
         }
      }
   }
}
