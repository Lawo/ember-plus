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
   /// EmberPlus-Glow.QualifiedNode [APPLICATION 10] Type
   /// </summary>
   public class GlowQualifiedNode : GlowNodeBase
   {
      /// <summary>
      /// Creates a new instance of GlowQualifiedNode.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      protected internal GlowQualifiedNode(BerTag? tag)
      : base(tag, GlowType.QualifiedNode)
      {
      }

      /// <summary>
      /// Creates a new instance of GlowQualifiedNode, specifying the value
      /// of the "path" field.
      /// </summary>
      /// <param name="path">The value of the "path" (RELATIVE-OID) field.</param>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      public GlowQualifiedNode(int[] path, BerTag? tag = null)
      : this(tag)
      {
         if(path == null)
            throw new ArgumentNullException("path");

         if(path.Length == 0)
            throw new ArgumentException("path must always contain at least one sub-identifier: the number of the current node!");

         Path = path;
      }

      /// <summary>
      /// Gets the "path" (RELATIVE-OID) field
      /// </summary>
      public int[] Path
      {
         get { return GetChildValue<int[]>(GlowTags.QualifiedNode.Path); }
         private set
         {
            var tag = GlowTags.QualifiedNode.Path;

            AssertNotPresent(tag);

            Insert(new RelativeOidEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Overriden to return GlowTags.QualifiedNode.Children
      /// </summary>
      internal override BerTag ChildrenTag
      {
         get { return GlowTags.QualifiedNode.Children; }
      }

      /// <summary>
      /// Overriden to return GlowTags.QualifiedNode.Children
      /// </summary>
      internal override BerTag ContentsTag
      {
         get { return GlowTags.QualifiedNode.Contents; }
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
