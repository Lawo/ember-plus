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
   /// EmberPlus-Glow.EnumEntry [APPLICATION 7] Type
   /// </summary>
   public class GlowStringIntegerPair : GlowSubContainer
   {
      /// <summary>
      /// Creates a new instance of GlowStringIntegerPair.
      /// </summary>
      protected internal GlowStringIntegerPair(BerTag tag)
      : base(tag, GlowType.StringIntegerPair)
      {
      }

      /// <summary>
      /// Creates an encodable instance of GlowEnumEntry with
      /// the specified name/value pair.
      /// </summary>
      /// <param name="tag">The field tag of the enum entry.</param>
      /// <param name="name">The name of the enum entry.</param>
      /// <param name="value">The value of the enum entry.</param>
      public GlowStringIntegerPair(BerTag tag, string name, int value)
      : this(tag)
      {
         EntryName = name;
         EntryInteger = value;
      }

      /// <summary>
      /// Gets or sets the "entryValue" field
      /// </summary>
      public int EntryInteger
      {
         get { return GetChildValue<int>(GlowTags.StringIntegerPair.EntryInteger); }
         private set
         {
            var tag = GlowTags.StringIntegerPair.EntryInteger;

            AssertNotPresent(tag);

            Insert(new IntegerEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "entryName" field
      /// </summary>
      public string EntryName
      {
         get { return GetChildValue<string>(GlowTags.StringIntegerPair.EntryString); }
         private set
         {
            var tag = GlowTags.StringIntegerPair.EntryString;

            AssertNotPresent(tag);

            Insert(new StringEmberLeaf(tag, value));
         }
      }

      public override string ToString()
      {
         return String.Format("{0} • {1}", EntryName, EntryInteger);
      }
   }
}
