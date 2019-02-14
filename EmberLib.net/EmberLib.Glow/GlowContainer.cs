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
