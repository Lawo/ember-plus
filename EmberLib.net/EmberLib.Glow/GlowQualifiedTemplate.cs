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

using BerLib;

namespace EmberLib.Glow
{
   /// <summary>
   /// EmberPlus-Glow.QualifiedTemplate [Application 25] type.
   /// </summary>
   public class GlowQualifiedTemplate : GlowTemplateBase
   {
      /// <summary>
      /// Initializes a new instance of the <see cref="GlowQualifiedTemplate"/> class.
      /// </summary>
      /// <param name="tag">The tag to assign. For example <see cref="GlowTags.NodeContents.TemplateReference"/>.</param>
      internal GlowQualifiedTemplate(BerTag tag)
         : base(tag, GlowType.QualifiedTemplate)
      {
      }

      /// <summary>
      /// Initializes a new instance of the <see cref="GlowQualifiedTemplate"/> class.
      /// </summary>
      /// <param name="tag">The tag to assign. For example <see cref="GlowTags.NodeContents.TemplateReference"/>.</param>
      /// <param name="path">The path to assign.</param>
      public GlowQualifiedTemplate(int[] path, BerTag? tag)
         : base(tag ?? tag ?? GlowTags.CollectionItem, GlowType.QualifiedTemplate)
      {
         Path = path;
      }

      /// <summary>
      /// Gets the "path" (RELATIVE-OID) field
      /// </summary>
      public int[] Path
      {
         get { return GetChildValue<int[]>(GlowTags.QualifiedTemplate.Path); }
         private set
         {
            var tag = GlowTags.QualifiedTemplate.Path;

            AssertNotPresent(tag);
            Insert(new RelativeOidEmberLeaf(tag, value));
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
