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
using System.Diagnostics;
using BerLib;


namespace EmberLib
{
   /// <summary>
   /// Specialized container class to hold SETs.
   /// </summary>
   public class EmberSet : EmberContainer
   {
      /// <summary>
      /// Creates a new instance of EmberSet.
      /// </summary>
      /// <param name="tag">The tag for the newly created node.</param>
      /// <param name="parent">The parent container to insert this node into.
      /// It is usually better to use one of the other constructors
      /// and insert the sequence into its parent container only when
      /// all children have been added to the sequence.</param>
      /// <param name="isSafeMode">If true, multiple occurrences of one tag
      /// are disallowed for the children of the EmberSet. If a node with
      /// an already existing tag is inserted into the set, an exception is
      /// thrown.</param>
      public EmberSet(BerTag tag, EmberContainer parent, bool isSafeMode)
      : base(tag, parent, BerType.Set)
      {
         IsMapUsed = isSafeMode;
      }

      /// <summary>
      /// Creates a new instance of EmberSet, turning on safe mode.
      /// </summary>
      /// <param name="tag">The tag for the newly created node.</param>
      /// <param name="parent">The parent container to insert this node into.
      /// It is usually better to use one of the other constructors
      /// and insert the sequence into its parent container only when
      /// all children have been added to the sequence.</param>
      public EmberSet(BerTag tag, EmberContainer parent)
      : this(tag, parent, true)
      {
      }

      /// <summary>
      /// Creates a new instance of EmberSet, turning on safe mode.
      /// </summary>
      /// <param name="tag">The tag for the newly created node.</param>
      public EmberSet(BerTag tag)
      : this(tag, null, true)
      {
      }

      /// <summary>
      /// Removes the child node with the specified tag.
      /// </summary>
      /// <param name="tag">The tag of the child node to remove.</param>
      /// <returns>True if the child node was found and removed, otherwise false.</returns>
      public override sealed bool Remove(BerTag tag)
      {
         var result = base.Remove(tag);

         if(IsMapUsed)
            _nodesMap.Remove(tag);

         return result;
      }

      /// <summary>
      /// Gets the child node with the specified tag. If safe mode is used, lookup
      /// of child nodes is much more efficient since children are stored in a
      /// dictionary.
      /// </summary>
      /// <param name="tag">The tag of the child node to get.</param>
      /// <returns>The child node with the specified tag or null if no such
      /// node exists.</returns>
      internal override EmberNode GetChildNode(BerTag tag)
      {
         if(IsMapUsed)
         {
            EmberNode node;

            if(_nodesMap.TryGetValue(tag, out node))
               return node;

            return null;
         }

         return base.GetChildNode(tag);
      }

      public override sealed TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      protected IDictionary<BerTag, EmberNode> NodesMap
      {
         get { return _nodesMap; }
      }

      protected bool IsMapUsed { get; set; }

      protected void InsertChildNodeBase(EmberNode node)
      {
         base.InsertChildNode(node);
      }

      protected internal override void InsertChildNode(EmberNode node)
      {
         Debug.Assert(node != null);

         if(IsMapUsed)
         {
            var tag = node.Tag;

            if(_nodesMap.ContainsKey(tag))
               throw new BerException(1001, "A node with the specified tag already exists in this container");

            _nodesMap[tag] = node;
         }

         InsertChildNodeBase(node);
      }

      #region Implementation
      IDictionary<BerTag, EmberNode> _nodesMap = new Dictionary<BerTag, EmberNode>();
      #endregion
   }
}
