/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol
   Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

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
