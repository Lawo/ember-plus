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
   /// EmberPlus-Glow.StringIntegerCollection [APPLICATION 8] Type
   /// </summary>
   public class GlowStringIntegerCollection : GlowSubContainer
   {
      /// <summary>
      /// Creates a new instance of GlowStringIntegerCollection.
      /// </summary>
      public GlowStringIntegerCollection(BerTag tag)
      : base(tag, GlowType.StringIntegerCollection, isOrdered: false)
      {
      }

      /// <summary>
      /// Gets all children of type GlowEnumEntry
      /// </summary>
      public IEnumerable<GlowStringIntegerPair> EnumEntries
      {
         get
         {
            foreach(var child in this)
            {
               var entry = child as GlowStringIntegerPair;

               if(entry != null)
                  yield return entry;
            }
         }
      }

      /// <summary>
      /// Override to check if inserted StringIntegerPairs have the tag: GlowTags.StringIntegerCollection.StringIntegerPair.
      /// </summary>
      protected override void InsertChildNode(EmberNode node)
      {
         if(node is GlowStringIntegerPair
         && node.Tag != GlowTags.StringIntegerCollection.StringIntegerPair)
            throw new ArgumentException("When inserted into a StringIntegerCollection, the StringIntegerPair must have the default tag!");

         base.InsertChildNode(node);
      }
   }
}
