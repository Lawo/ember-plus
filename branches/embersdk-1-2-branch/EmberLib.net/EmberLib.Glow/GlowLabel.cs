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
   /// EmberPlus-Glow.Label [APPLICATION 18] Type
   /// </summary>
   public class GlowLabel : GlowSubContainer
   {
      /// <summary>
      /// Creates a new instance of GlowLabel.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a LabelCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      public GlowLabel(BerTag? tag = null)
      : base(tag ?? GlowTags.CollectionItem, GlowType.Label)
      {
      }

      /// <summary>
      /// Gets or sets the "basePath" field
      /// </summary>
      public int[] BasePath
      {
         get { return GetChildValue<int[]>(GlowTags.Label.BasePath); }
         set
         {
            var tag = GlowTags.Label.BasePath;

            Remove(tag);
            Insert(new RelativeOidEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "description" field
      /// </summary>
      public string Description
      {
         get { return GetChildValue<string>(GlowTags.Label.Description); }
         set
         {
            var tag = GlowTags.Label.Description;

            Remove(tag);
            Insert(new StringEmberLeaf(tag, value));
         }
      }
   }
}
