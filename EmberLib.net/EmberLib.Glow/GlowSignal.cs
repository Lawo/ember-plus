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
   /// EmberPlus-Glow.Signal type.
   /// </summary>
   public abstract class GlowSignal : GlowSubContainer
   {
      /// <summary>
      /// Creates a new instance of GlowSignal.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a matrix signal collection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      /// <param name="type">ber type of the represented glow type</param>
      protected GlowSignal(BerTag? tag, uint type)
      : base(tag ?? GlowTags.CollectionItem, type)
      {
      }

      /// <summary>
      /// Creates a new instance of GlowSignal, specifying the value
      /// of the "number" field.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a matrix signal collection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      /// <param name="number">The value of the "number" field.</param>
      /// <param name="type">ber type of the represented glow type</param>
      protected GlowSignal(int number, BerTag? tag, uint type)
      : this(tag, type)
      {
         Number = number;
      }

      /// <summary>
      /// Gets the "number" field
      /// </summary>
      public int Number
      {
         get { return GetChildValue<int>(GlowTags.Signal.Number); }
         private set
         {
            var tag = GlowTags.Signal.Number;

            AssertNotPresent(tag);

            Insert(new IntegerEmberLeaf(tag, value));
         }
      }
   }
}
