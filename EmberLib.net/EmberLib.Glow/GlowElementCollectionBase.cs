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
