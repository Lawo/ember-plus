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
            case GlowType.Template:
               break;

            default:
               throw new ArgumentException("A GlowElementCollection must not contain this node type!");
         }

         base.InsertChildNode(node);
      }
   }
}
