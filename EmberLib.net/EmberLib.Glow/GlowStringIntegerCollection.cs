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
