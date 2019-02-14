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
   /// EmberPlus-Glow.Source [APPLICATION 14] type.
   /// </summary>
   public class GlowTarget : GlowSignal
   {
      /// <summary>
      /// Creates a new instance of GlowTarget.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a matrix signal collection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      protected internal GlowTarget(BerTag? tag)
      : base(tag, GlowType.Target)
      {
      }

      /// <summary>
      /// Creates a new instance of GlowTarget.
      /// </summary>
      /// <param name="number">Value of the "number" field.</param>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a matrix signal collection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      public GlowTarget(int number, BerTag? tag = null)
      : base(number, tag, GlowType.Target)
      {
      }
   }
}
