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
using System.Xml;
using System.Diagnostics;
using BerLib;


namespace EmberLib
{
   /// <summary>
   /// A node that may contain child nodes.
   /// </summary>
   public class EmberContainer : EmberNode
   {
      /// <summary>
      /// Creates a new instance of EmberContainer.
      /// </summary>
      /// <param name="tag">The tag of the new node.</param>
      /// <param name="parent">The parent to insert the new node into.</param>
      /// <param name="type">The BerType of the new node.</param>
      public EmberContainer(BerTag tag, EmberContainer parent, uint type)
      : base(tag)
      {
         Type = type;

         if(parent != null)
            parent.InsertChildNode(this);
      }

      /// <summary>
      /// Gets the number of child nodes this node contains.
      /// </summary>
      public override sealed int Count
      {
         get { return _nodes.Count; }
      }

      /// <summary>
      /// Gets a value indicating whether this container needs to be pre-encoded
      /// before being written out.
      /// </summary>
      internal override bool IsDirty
      {
         get { return _encodedHeader == null; }
      }

      /// <summary>
      /// Removes the child node with the specified tag.
      /// </summary>
      /// <param name="tag">The tag of the child node to remove.</param>
      /// <returns>True if the child node was found and removed, otherwise false.</returns>
      public override bool Remove(BerTag tag)
      {
         var foundNode = GetChildNode(tag);

         if(foundNode != null)
         {
            _childrenLength -= foundNode.EncodedLength;
            _nodes.Remove(foundNode);

            MarkDirty();

            return true;
         }

         return false;
      }

      /// <summary>
      /// Gets the child node with the specified tag.
      /// </summary>
      /// <param name="tag">The tag of the child node to get.</param>
      /// <returns>The child node with the specified tag or null if no such
      /// node exists.</returns>
      internal override EmberNode GetChildNode(BerTag tag)
      {
         foreach(var child in _nodes)
         {
            if(child.Tag == tag)
               return child;
         }

         return null;
      }

      /// <summary>
      /// Writes the binary BER representaion of this node and its children to output.
      /// </summary>
      /// <param name="output">The output to write to.</param>
      internal override void EncodeOverride(IBerOutput output)
      {
         if(IsDirty)
            Update();

         output.WriteBytes(_encodedHeader);

         var orderedNodes = OrderChildren(_nodes);

         foreach(var node in orderedNodes)
            node.Encode(output);

         if(Parent == null) // root node
            output.Flush();
      }

      /// <summary>
      /// Accepts a visitor
      /// </summary>
      /// <typeparam name="TState">The type of the visitor's state</typeparam>
      /// <typeparam name="TResult">The type the visitor methods return.</typeparam>
      /// <param name="visitor">The visitor</param>
      /// <param name="state">The visitor's initial state</param>
      /// <returns>Whatever the visitor methods return</returns>
      public override TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      /// <summary>
      /// Inserts a node into this node's collection of children.
      /// </summary>
      /// <param name="node">The node to insert.</param>
      protected internal override void InsertChildNode(EmberNode node)
      {
         if(node == null)
            throw new ArgumentNullException("node");

         var nodeLength = node.EncodedLength;

         node.Parent = this;

         _nodes.AddLast(node);

         MarkDirty();
      }

      /// <summary>
      /// Gives deriving classes the chance to change the order of child nodes.
      /// Called before the children are written to a IBerOutput instance.
      /// </summary>
      /// <param name="children">The child nodes in their current order.</param>
      /// <returns>A collection containing the passed child nodes in the
      /// desired order.</returns>
      internal virtual IEnumerable<EmberNode> OrderChildren(IEnumerable<EmberNode> children)
      {
         return children;
      }

      /// <summary>
      /// Pre-encodes the BER header and returns the byte length of the encoded node.
      /// Overridden to recursively pre-encode all descendants.
      /// </summary>
      /// <returns>The length of the encoded node.</returns>
      internal override int Update()
      {
         var output = new BerMemoryOutput();
         var implicitTag = BerType.IsApplicationDefined(Type)
                           ? new BerTag(BerClass.Application, Type & ~BerType.ApplicationFlag, true)
                           : new BerTag(BerClass.Universal, Type, true);

         _childrenLength = 0;

         foreach(var child in _nodes)
         {
            _childrenLength += child.IsDirty
                               ? child.Update()
                               : child.EncodedLength;
         }

         var implicitLength = _childrenLength + BerEncoding.GetHeaderLength(implicitTag, _childrenLength);

         BerEncoding.EncodeTag(output, Tag.ToContainer());
         BerEncoding.EncodeLength(output, implicitLength);

         BerEncoding.EncodeTag(output, implicitTag);
         BerEncoding.EncodeLength(output, _childrenLength);

         _encodedHeader = output.ToArray();
         EncodedLength = _childrenLength + _encodedHeader.Length;

         return EncodedLength;
      }

      #region Implementation
      LinkedList<EmberNode> _nodes = new LinkedList<EmberNode>();
      int _childrenLength;
      byte[] _encodedHeader;

      /// <summary>
      /// Notifies this container that the length of a direct child has changed. This
      /// forces the container to update its header data and notify its own parent of
      /// the change when written out.
      /// </summary>
      void MarkDirty()
      {
         _encodedHeader = null;

         var parent = Parent;

         if(parent != null)
            parent.MarkDirty();
      }
      #endregion

      #region IEnumerable<EmberNode> Members
      public override sealed IEnumerator<EmberNode> GetEnumerator()
      {
         return _nodes.GetEnumerator();
      }
      #endregion
   }
}
