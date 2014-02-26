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


namespace EmberLib
{
   /// <summary>
   /// Specialized container class to hold SEQUENCEs.
   /// </summary>
   public class EmberSequence : EmberContainer
   {
      /// <summary>
      /// Creates a new instance of EmberSequence.
      /// </summary>
      /// <param name="tag">The tag for the newly created node.</param>
      protected EmberSequence(BerTag tag, uint type)
      : base(tag, null, type)
      {
      }

      /// <summary>
      /// Creates a new instance of EmberSequence.
      /// </summary>
      /// <param name="tag">The tag for the newly created node.</param>
      /// <param name="parent">The parent container to insert this node into.</param>
      public EmberSequence(BerTag tag, EmberContainer parent = null)
      : base(tag, parent, BerType.Sequence)
      {
      }

      /// <summary>
      /// Gets or sets a value indicating whether children should be encoded
      /// ordered by tag or not.
      /// </summary>
      protected internal bool IsOrdered { get; set; }

      /// <summary>
      /// Removes the child node with the specified tag.
      /// </summary>
      /// <param name="tag">The tag of the child node to remove.</param>
      /// <returns>True if the child node was found and removed, otherwise false.</returns>
      public override sealed bool Remove(BerTag tag)
      {
         return base.Remove(tag);
      }

      /// <summary>
      /// Creates a new instance of EmberSequence with the IsOrdered flag
      /// set to true.
      /// </summary>
      /// <param name="tag">The tag of the new EmberSequence.</param>
      /// <returns>A new instance of EmberSequence with the IsOrdered flag
      /// set to true.</returns>
      public static EmberSequence CreateOrderedSequence(BerTag tag)
      {
         var sequence = new EmberSequence(tag)
         {
            IsOrdered = true,
         };

         return sequence;
      }

      /// <summary>
      /// Overriden to order child nodes by tag, since in a sequence child order is
      /// significant.
      /// </summary>
      /// <param name="children">The child nodes in the original order.</param>
      /// <returns>A collection containing the passed child nodes ordered by tag.</returns>
      internal override IEnumerable<EmberNode> OrderChildren(IEnumerable<EmberNode> children)
      {
         if(IsOrdered)
         {
            var nodes = new List<EmberNode>(children);
            nodes.Sort((x, y) => x.Tag.CompareTo(y.Tag));
            return nodes;
         }

         return children;
      }

      public override sealed TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }
   }
}
