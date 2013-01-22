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
   /// EmberPlus-Glow.QualifiedMatrix [APPLICATION 17] Type
   /// </summary>
   public class GlowQualifiedMatrix : GlowMatrixBase
   {
      /// <summary>
      /// Creates a new instance of GlowQualifiedMatrix.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      protected internal GlowQualifiedMatrix(BerTag? tag)
      : base(tag, GlowType.QualifiedMatrix)
      {
      }

      /// <summary>
      /// Creates a new instance of GlowQualifiedMatrix, specifying the value
      /// of the "path" field.
      /// </summary>
      /// <param name="path">The value of the "path" (RELATIVE-OID) field.</param>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      public GlowQualifiedMatrix(int[] path, BerTag? tag = null)
      : this(tag)
      {
         if(path == null)
            throw new ArgumentNullException("path");

         if(path.Length == 0)
            throw new ArgumentException("path must always contain at least one sub-identifier: the number of the current parameter!");

         Path = path;
      }

      /// <summary>
      /// Gets the "path" (RELATIVE-OID) field
      /// </summary>
      public int[] Path
      {
         get { return GetChildValue<int[]>(GlowTags.QualifiedMatrix.Path); }
         private set
         {
            var tag = GlowTags.QualifiedMatrix.Path;

            AssertNotPresent(tag);

            Insert(new RelativeOidEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Overridden to return GlowTags.QualifiedMatrix.Targets.
      /// </summary>
      internal override BerTag TargetsTag
      {
         get { return GlowTags.QualifiedMatrix.Targets; }
      }

      /// <summary>
      /// Overridden to return GlowTags.QualifiedMatrix.Sources.
      /// </summary>
      internal override BerTag SourcesTag
      {
         get { return GlowTags.QualifiedMatrix.Sources; }
      }

      /// <summary>
      /// Overridden to return GlowTags.QualifiedMatrix.Connection.
      /// </summary>
      internal override BerTag ConnectionsTag
      {
         get { return GlowTags.QualifiedMatrix.Connections; }
      }

      /// <summary>
      /// Overridden to return GlowTags.QualifiedMatrix.Contents.
      /// </summary>
      internal override BerTag ContentsTag
      {
         get { return GlowTags.QualifiedMatrix.Contents; }
      }

      /// <summary>
      /// Overridden to return GlowTags.QualifiedMatrix.Children.
      /// </summary>
      internal override BerTag ChildrenTag
      {
         get { return GlowTags.QualifiedMatrix.Children; }
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
