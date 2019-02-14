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
