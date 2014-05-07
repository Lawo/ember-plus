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
   /// EmberPlus-Glow.ElementCollection [APPLICATION 4] Type
   /// </summary>
   public class GlowElementCollection : GlowElementCollectionBase
   {
      /// <summary>
      /// Creates a new instance of GlowElementCollection.
      /// </summary>
      public GlowElementCollection(BerTag tag)
      : base(tag, GlowType.ElementCollection)
      {
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
      /// Overriden to check if inserted GlowElements have the required tag: GlowTags.CollectionItem
      /// </summary>
      protected override void InsertChildNode(EmberNode node)
      {
         if(node is GlowElement
         && node.Tag != GlowTags.CollectionItem)
            throw new ArgumentException("When inserted into a GlowElementCollection, the GlowElement must have the ElementDefaultTag");

         switch(node.BerTypeNumber)
         {
            case GlowType.Command:
            case GlowType.Parameter:
            case GlowType.Node:
            case GlowType.Matrix:
            case GlowType.Function:
               break;

            default:
               throw new ArgumentException("A GlowElementCollection must not contain this node type!");
         }

         base.InsertChildNode(node);
      }
   }
}
