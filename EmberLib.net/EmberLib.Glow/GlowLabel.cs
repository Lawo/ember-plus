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
