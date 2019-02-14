/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
      /// <param name="path">The path to assign.</param>
      /// <param name="tag">The tag to assign. For example <see cref="GlowTags.NodeContents.TemplateReference"/>. If set
      /// to <c>null</c>, the tag will be set to <see cref="GlowTags.CollectionItem"/>.</param>
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
         get
         {
            return GetChildValue<int[]>(GlowTags.QualifiedTemplate.Path);
         }
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
