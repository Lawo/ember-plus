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
   /// EmberPlus-Glow.RootElementCollection [APPLICATION 11] Type
   /// </summary>
   public class GlowRootElementCollection : GlowElementCollectionBase
   {
      /// <summary>
      /// Creates a new instance of GlowRootElementCollection.
      /// </summary>
      /// <remarks>
      /// Usually, objects of type GlowRootElementCollection are created using
      /// the Method CreateRoot(), which assigns the root tag to the new
      /// object.
      /// </remarks>
      protected internal GlowRootElementCollection(BerTag tag)
      : base(tag, GlowType.RootElementCollection)
      {
      }

      /// <summary>
      /// Gets a value indicating whether this stream collection has the outer tag identifying
      /// the object tree root.
      /// </summary>
      public bool IsRoot
      {
         get { return Tag == GlowTags.Root; }
      }

      /// <summary>
      /// Creates a GlowRootElementCollection instance with the outer tag identifying the object tree root.
      /// </summary>
      /// <returns>A new instance of GlowRootElementCollection with the outer tag identifying the object tree root.</returns>
      public static GlowRootElementCollection CreateRoot()
      {
         return new GlowRootElementCollection(GlowTags.Root);
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
   }
}
