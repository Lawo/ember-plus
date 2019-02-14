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
   public abstract class GlowElement : GlowContainer
   {
      /// <summary>
      /// Creates a new instance of GlowElement.
      /// </summary>
      protected GlowElement(BerTag? tag, uint type)
      : base(tag ?? GlowTags.CollectionItem, type)
      {
      }
   }
}
