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
using System.Collections.Generic;
using System.Text;
using BerLib;

namespace EmberLib.Glow
{
   /// <summary>
   /// EmberPlus-Glow.Source [APPLICATION 15] type.
   /// </summary>
   public class GlowSource : GlowSignal
   {
      /// <summary>
      /// Creates a new instance of GlowSource.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a matrix signal collection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      protected internal GlowSource(BerTag? tag)
      : base(tag, GlowType.Source)
      {
      }

      /// <summary>
      /// Creates a new instance of GlowSource.
      /// </summary>
      /// <param name="number">Value of the "number" field.</param>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a matrix signal collection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      public GlowSource(int number, BerTag? tag = null)
      : base(number, tag, GlowType.Source)
      {
      }
   }
}
