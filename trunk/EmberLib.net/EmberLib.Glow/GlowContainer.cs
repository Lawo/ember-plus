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
   /// Base class for Glow types
   /// </summary>
   public abstract class GlowContainer : EmberSequence
   {
      /// <summary>
      /// Creates a new instance of GlowContainer.
      /// </summary>
      protected GlowContainer(BerTag tag, uint type, bool isOrdered = true)
      : base(tag, type)
      {
         IsOrdered = isOrdered;
      }

      public bool HasValidationErrors { get; protected set; }

      /// <summary>
      /// Accepts a visitor of type IGlowVisitor
      /// </summary>
      /// <typeparam name="TState">Visitor state type</typeparam>
      /// <typeparam name="TResult">Visitor result type</typeparam>
      /// <param name="visitor">The visitor instance to accept</param>
      /// <param name="state">State object to pass to the visitor</param>
      /// <returns>The result of the visiting method</returns>
      public abstract TResult Accept<TState, TResult>(IGlowVisitor<TState, TResult> visitor, TState state);

      /// <summary>
      /// Asserts that no node with the specified tag is already present in this container.
      /// </summary>
      protected void AssertNotPresent(BerTag tag)
      {
         if(this[tag] != null)
            throw new InvalidOperationException("Node with specified tag is already present");
      }

      internal TValue GetChildValue<TValue>(BerTag tag)
      {
         return ((EmberLeaf<TValue>)this[tag]).Value;
      }
   }
}
