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
using System.Diagnostics;

namespace EmberLib
{
   /// <summary>
   /// Implements an asynchronous Ember reader that creates a tree of EmberNode objects
   /// from the incoming BER data.
   /// </summary>
   public class AsyncDomReader : AsyncEmberReader
   {
      /// <summary>
      /// Creates a new instance of AsyncDomReader.
      /// </summary>
      /// <param name="application">The application interface responsible for creating nodes
      /// with application-defined types. If null, containers with application-defined types
      /// will be decoded to object of type EmberContainer.</param>
      public AsyncDomReader(EmberApplicationInterface application)
      {
         _application = application;
      }

      /// <summary>
      /// Gets a value indicating whether a complete tree has been decoded.
      /// </summary>
      protected bool IsRootReady { get; private set; }

      #region RootReady Event
      public class RootReadyArgs : EventArgs
      {
         public EmberNode Root { get; private set; }

         public RootReadyArgs(EmberNode root)
         {
            Root = root;
         }
      }

      /// <summary>
      /// Raised when a complete tree has been decoded. The root of the decoded tree
      /// is included in the RootReadyArgs.
      /// </summary>
      public event EventHandler<RootReadyArgs> RootReady;

      protected virtual void OnRootReady(RootReadyArgs oArgs)
      {
         if(RootReady != null)
            RootReady(this, oArgs);
      }
      #endregion

      /// <summary>
      /// Returns the root node of the decoded tree and resets the reader.
      /// </summary>
      /// <returns>The root node of the decoded tree if IsRootReady is true, otherwise null.</returns>
      public EmberNode DetachRoot()
      {
         if(IsRootReady)
         {
            var root = _rootNode;

            _rootNode = null;
            _currentNode = null;

            IsRootReady = false;
            return root;
         }

         return null;
      }

      /// <summary>
      /// Resets the reader state. Overridden to free all memory assigned for
      /// DOM nodes being decoded.
      /// </summary>
      public override void Reset()
      {
         base.Reset();

         _rootNode = null;
         _currentNode = null;
         IsRootReady = false;
      }

      /// <summary>
      /// Overridden to create DOM nodes from the decoded TLV.
      /// In derived classes, override the OnNewContainer(EmberNode)
      /// overload.
      /// </summary>
      protected override sealed void OnNewContainer()
      {
         Debug.Assert(_currentNode == null || _currentNode is EmberContainer);

         var container = EmberNode.FromReader(this, _application);

         Debug.Assert(container != null);

         if(IsRootReady)
         {
            Debug.Assert(_rootNode != null);
            Debug.Assert(_currentNode == null);

            _rootNode = null;
            IsRootReady = false;
         }

         if(_rootNode == null)
         {
            Debug.Assert(_currentNode == null);

            _rootNode = container;
         }
         else
         {
            Debug.Assert(_currentNode != null);

            _currentNode.InsertChildNode(container);
         }

         _currentNode = container;

         OnNewContainer(container);
      }

      /// <summary>
      /// Overridden to create DOM nodes from the decoded TLV.
      /// In derived classes, override the OnItemReady(EmberNode)
      /// overload.
      /// </summary>
      protected override sealed void OnItemReady()
      {
         Debug.Assert(_rootNode != null);
         Debug.Assert(_currentNode != null);

         if(IsContainer)
         {
            EmberNode readyNode;

            if(_currentNode == _rootNode)
            {
               IsRootReady = true;

               OnRootReady(new RootReadyArgs(_currentNode));

               readyNode = _currentNode;
               _currentNode = null;
            }
            else
            {
               readyNode = _currentNode;
               _currentNode = _currentNode.Parent;
            }

            RaiseItemReady(readyNode);
         }
         else
         {
            var node = EmberNode.FromReader(this, _application);

            if(node != null)
            {
               _currentNode.InsertChildNode(node);

               RaiseItemReady(node);
            }
         }
      }

      /// <summary>
      /// Called when the opening tag and length of a new container have been
      /// read.
      /// The opened container can be accessed through <paramref name="node" />.
      /// </summary>
      /// <param name="node">The newly created EmberNode to hold the container's children.
      /// The children will not be included in the node until OnItemReady(EmberNode) is
      /// called.
      /// You can use the EmberNode.Accept() method to pass an IEmberVisitor implementation to
      /// examine the node.</param>
      protected virtual void OnNewContainer(EmberNode node)
      {
      }

      /// <summary>
      /// Called when a TLV (either container or primitive) has been decoded.
      /// The read TLV can be examined through <paramref name="node"/>.
      /// </summary>
      /// <param name="node">An EmberNode object containing the decoded information.
      /// This object is ready for use, all significant properties (like Tag, Parent, Type)
      /// have valid values.
      /// You can use the EmberNode.Accept() method to pass an IEmberVisitor implementation to
      /// examine the node.</param>
      protected virtual void OnItemReady(EmberNode node)
      {
      }

      #region Implementation
      EmberNode _rootNode;
      EmberNode _currentNode;
      EmberApplicationInterface _application;

      void RaiseItemReady(EmberNode node)
      {
         node.ValidateAfterDecode();
         OnItemReady(node);
      }
      #endregion
   }
}
