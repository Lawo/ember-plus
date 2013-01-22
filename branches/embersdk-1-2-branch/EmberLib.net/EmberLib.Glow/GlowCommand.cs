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
   /// EmberPlus-Glow.Command [APPLICATION 2] Type
   /// </summary>
   public class GlowCommand : GlowElement
   {
      /// <summary>
      /// Creates a new instance of GlowCommand.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      protected internal GlowCommand(BerTag? tag)
      : base(tag, GlowType.Command)
      {
      }

      /// <summary>
      /// Creates an encodable instance of GlowCommand, specifying the value
      /// of the "number" field.
      /// </summary>
      /// <param name="number">The value of the "number" field.</param>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      public GlowCommand(int number, BerTag? tag = null)
      : this(tag)
      {
         Number = number;
      }

      /// <summary>
      /// Gets or sets the "number" field
      /// </summary>
      public int Number
      {
         get { return GetChildValue<int>(GlowTags.Command.Number); }
         private set
         {
            var tag = GlowTags.Command.Number;

            AssertNotPresent(tag);

            Insert(new IntegerEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "dirFieldMask" field.
      /// Getter returns null if field not present.
      /// </summary>
      public int? DirFieldMask
      {
         get
         {
            var childNode = this[GlowTags.Command.DirFieldMask] as IntegerEmberLeaf;

            if(childNode != null)
               return childNode.Value;

            return null;
         }
         set
         {
            var tag = GlowTags.Command.DirFieldMask;

            Remove(tag);
            Insert(new IntegerEmberLeaf(tag, value.Value));
         }
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
