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
