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
   /// EmberPlus-Glow.TupleItemDescription [APPLICATION 21] Type
   /// </summary>
   public class GlowTupleItemDescription : GlowSubContainer
   {
      /// <summary>
      /// Creates a new instance of GlowTupleItemDescription.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a TupleDescription. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      protected internal GlowTupleItemDescription(BerTag? tag)
      : base(tag ?? GlowTags.CollectionItem, GlowType.TupleItemDescription)
      {
      }

      /// <summary>
      /// Creates a new instance of GlowTupleItemDescription.
      /// </summary>
      /// <param name="type">The value of the "type" field.</param>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a TupleDescription. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      public GlowTupleItemDescription(int type, BerTag? tag = null)
      : this(tag)
      {
         Type = type;
      }

      /// <summary>
      /// Gets or sets the "type" field. The value must be one of the
      /// constants defined in the GlowParameterType class.
      /// </summary>
      public int Type
      {
         get { return GetChildValue<int>(GlowTags.TupleItemDescription.Type); }
         set
         {
            var tag = GlowTags.TupleItemDescription.Type;

            Remove(tag);
            Insert(new IntegerEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "name" field
      /// </summary>
      public string Name
      {
         get
         {
            var childNode = this[GlowTags.TupleItemDescription.Name] as StringEmberLeaf;

            if(childNode != null)
               return childNode.Value;

            return null;
         }
         set
         {
            var tag = GlowTags.TupleItemDescription.Name;

            Remove(tag);
            Insert(new StringEmberLeaf(tag, value));
         }
      }
   }
}
