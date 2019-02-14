/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

using System;
using BerLib;

namespace EmberLib.Glow
{
   /// <summary>
   /// EmberPlus-Glow.Template [Application 24] Type.
   /// </summary>
   public class GlowTemplate : GlowTemplateBase
   {
      /// <summary>
      /// Initializes a new instance of the <see cref="GlowTemplate"/> class.
      /// </summary>
      /// <param name="tag">The tag to assign. For example <see cref="GlowTags.NodeContents.TemplateReference"/>.</param>
      internal GlowTemplate(BerTag tag)
         : base(tag, GlowType.Template)
      {
      }

      /// <summary>
      /// Initializes a new instance of the <see cref="GlowTemplate"/> class.
      /// </summary>
      /// <param name="number">The number of the template.</param>
      /// <param name="tag">The tag to assign. For example <see cref="GlowTags.NodeContents.TemplateReference"/>. If <c>nukk</c>,
      /// the tag will be set to <see cref="GlowTags.CollectionItem"/></param>
      public GlowTemplate(int number, BerTag? tag = null)
         : base(tag ?? GlowTags.CollectionItem, GlowType.Template)
      {
         Number = number;
      }

      /// <summary>
      /// Gets or sets the number of this template
      /// </summary>
      public int Number
      {
         get
         {
            return GetChildValue<int>(GlowTags.Template.Number);
         }
         set
         {
            var tag = GlowTags.Template.Number;

            AssertNotPresent(tag);
            Insert(new IntegerEmberLeaf(tag, value));
         }
      }

      #region Overrides
      public override TResult Accept<TState, TResult>(IGlowVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }
      #endregion
   }
}
