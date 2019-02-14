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
   /// Base class for EmberPlus-Glow.RootElementCollection and EmberPlus-Glow.ElementCollection types
   /// </summary>
   public abstract class GlowElementCollectionBase : GlowContainer
   {
      /// <summary>
      /// Creates a new instance of GlowElementCollectionBase.
      /// </summary>
      protected GlowElementCollectionBase(BerTag tag, uint type)
      : base(tag, type, isOrdered: false)
      {
      }

      /// <summary>
      /// Gets all children of type GlowElement
      /// </summary>
      public IEnumerable<GlowElement> Elements
      {
         get
         {
            foreach(var child in this)
            {
               var element = child as GlowElement;

               if(element != null)
                  yield return element;
            }
         }
      }

      /// <summary>
      /// Overriden to check if inserted GlowElements have the required tag: GlowTags.CollectionItem
      /// </summary>
      protected override void InsertChildNode(EmberNode node)
      {
         if(node is GlowElement
         && node.Tag != GlowTags.CollectionItem)
            throw new ArgumentException("When inserted into a GlowElementCollection, the GlowElement must have the ElementDefaultTag");

         base.InsertChildNode(node);
      }
   }
}
