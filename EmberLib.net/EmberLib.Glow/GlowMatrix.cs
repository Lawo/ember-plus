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
   /// EmberPlus-Glow.Matrix [APPLICATION 13] Type
   /// </summary>
   public class GlowMatrix : GlowMatrixBase
   {
      /// <summary>
      /// Creates a new instance of GlowMatrix.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      protected internal GlowMatrix(BerTag? tag)
      : base(tag, GlowType.Matrix)
      {
      }

      /// <summary>
      /// Creates a new instance of GlowMatrix, specifying the value
      /// of the "number" field.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      /// <param name="number">The value of the "number" field.</param>
      public GlowMatrix(int number, BerTag? tag = null)
      : this(tag)
      {
         Number = number;
      }

      /// <summary>
      /// Gets the "number" field
      /// </summary>
      public int Number
      {
         get { return GetChildValue<int>(GlowTags.Matrix.Number); }
         private set
         {
            var tag = GlowTags.Matrix.Number;

            AssertNotPresent(tag);

            Insert(new IntegerEmberLeaf(tag, value));
         }
      }

      #region Overrides
      /// <summary>
      /// Overriden to return GlowTags.Matrix.Contents
      /// </summary>
      internal override BerLib.BerTag ContentsTag
      {
         get { return GlowTags.Matrix.Contents; }
      }

      /// <summary>
      /// Overriden to return GlowTags.Matrix.Children
      /// </summary>
      internal override BerTag ChildrenTag
      {
         get { return GlowTags.Matrix.Children; }
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
      /// Overriden to return GlowTags.Matrix.Targets
      /// </summary>
      internal override BerTag TargetsTag
      {
         get { return GlowTags.Matrix.Targets; }
      }

      /// <summary>
      /// Overriden to return GlowTags.Matrix.Sources
      /// </summary>
      internal override BerTag SourcesTag
      {
         get { return GlowTags.Matrix.Sources; }
      }

      /// <summary>
      /// Overriden to return GlowTags.Matrix.Connections
      /// </summary>
      internal override BerTag ConnectionsTag
      {
         get { return GlowTags.Matrix.Connections; }
      }
      #endregion
   }
}
