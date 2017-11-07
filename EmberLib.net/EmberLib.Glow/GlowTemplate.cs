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
