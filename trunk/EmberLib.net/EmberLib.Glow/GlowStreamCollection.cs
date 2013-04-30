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
   /// EmberPlus-Glow.StreamCollection [APPLICATION 6] Type
   /// </summary>
   public class GlowStreamCollection : GlowContainer
   {
      /// <summary>
      /// Creates a new instance of GlowStreamCollection.
      /// </summary>
      /// <param name="tag">The tag of the new StreamCollection.</param>
      /// <remarks>
      /// Usually, objects of type GlowStreamCollection are created using
      /// the Method CreateRoot(), which assigns the root tag to the new
      /// object.
      /// </remarks>
      protected internal GlowStreamCollection(BerTag tag)
      : base(tag, GlowType.StreamCollection, isOrdered: false)
      {
      }

      /// <summary>
      /// Gets all children of type GlowStreamEntry
      /// </summary>
      public IEnumerable<GlowStreamEntry> StreamEntries
      {
         get
         {
            foreach(var child in this)
            {
               var streamEntry = child as GlowStreamEntry;

               if(streamEntry != null)
                  yield return streamEntry;
            }
         }
      }

      /// <summary>
      /// Creates a GlowStreamCollection instance with the outer tag identifying the object tree root.
      /// </summary>
      /// <returns>A new instance of GlowStreamCollection with the outer tag identifying the object tree root.</returns>
      public static GlowStreamCollection CreateRoot()
      {
         return new GlowStreamCollection(GlowTags.Root);
      }

      /// <summary>
      /// Accepts a visitor of type IGlowVisitor
      /// </summary>
      /// <typeparam name="TState">Visitor state type</typeparam>
      /// <typeparam name="TResult">Visitor result type</typeparam>
      /// <param name="visitor">The visitor instance to accept</param>
      /// <param name="state">State object to pass to the visitor</param>
      /// <returns>The result of the visiting method</returns>
      public override sealed TResult Accept<TState, TResult>(IGlowVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      /// <summary>
      /// Override to check if inserted GlowStreamEntries have the tag: GlowTags.StreamCollection.StreamEntry
      /// </summary>
      protected override void InsertChildNode(EmberNode node)
      {
         if(node is GlowStreamEntry
         && node.Tag != GlowTags.StreamCollection.StreamEntry)
            throw new ArgumentException("When inserted into a StreamCollection, the StreamEntry must have the ElementDefaultTag");

         base.InsertChildNode(node);
      }
   }
}
