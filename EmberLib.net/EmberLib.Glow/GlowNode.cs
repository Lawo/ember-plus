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
   /// EmberPlus-Glow.Node [APPLICATION 3] Type
   /// </summary>
   public class GlowNode : GlowNodeBase
   {
      /// <summary>
      /// Creates a new instance of GlowNode.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      protected internal GlowNode(BerTag? tag)
      : base(tag, GlowType.Node)
      {
      }

      /// <summary>
      /// Creates an encodable instance of GlowNode, specifying the value
      /// of the "number" field.
      /// </summary>
      /// <param name="number">The value of the "number" field.</param>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      public GlowNode(int number, BerTag? tag = null)
      : this(tag)
      {
         Number = number;
      }

      /// <summary>
      /// Gets the "number" field
      /// </summary>
      public int Number
      {
         get { return GetChildValue<int>(GlowTags.Node.Number); }
         private set
         {
            var tag = GlowTags.Node.Number;

            AssertNotPresent(tag);

            Insert(new IntegerEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Overriden to return GlowTags.Node.Contents
      /// </summary>
      internal override BerTag ContentsTag
      {
         get { return GlowTags.Node.Contents; }
      }

      /// <summary>
      /// Overriden to return GlowTags.Node.Children
      /// </summary>
      internal override BerTag ChildrenTag
      {
         get { return GlowTags.Node.Children; }
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
