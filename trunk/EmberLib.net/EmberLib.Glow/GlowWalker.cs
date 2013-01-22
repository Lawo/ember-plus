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
using System.Diagnostics.CodeAnalysis;

namespace EmberLib.Glow
{
   /// <summary>
   /// Provides a way to walk a glow tree easier than implementing the IGlowVisitor interface.
   /// The GlowWalker class itself implements the IGlowVisitor interface, therefore you can
   /// pass an instance  of a class derived from GlowWalker to the Accept method of the glow
   /// root container.
   /// You can also call the Walk method of the GlowWalker class.
   /// </summary>
   public abstract class GlowWalker : IGlowVisitor<object, object>
   {
      /// <summary>
      /// Walks a complete glow tree and calls the abstract methods
      /// OnCommand, OnNode, OnParameter, OnMatrix and OnStreamEntry
      /// in the process.
      /// </summary>
      /// <param name="glow">The root of the glow tree to walk.</param>
      public void Walk(GlowContainer glow)
      {
         if(glow == null)
            throw new ArgumentNullException("glow");

         glow.Accept(this, null);
      }

      /// <summary>
      /// Called for every GlowCommand found in the glow tree.
      /// </summary>
      /// <param name="glow">The GlowCommand to process.</param>
      /// <param name="path">The path of the element containing the GlowCommand.</param>
      protected abstract void OnCommand(GlowCommand glow, int[] path);

      /// <summary>
      /// Called for every GlowNode containing a "contents" set found in the glow tree.
      /// </summary>
      /// <param name="glow">The GlowNode to process.</param>
      /// <param name="path">The path of the node. The last number in the path
      /// is the node's number.</param>
      protected abstract void OnNode(GlowNodeBase glow, int[] path);

      /// <summary>
      /// Called for every GlowParameter found in the glow tree.
      /// </summary>
      /// <param name="glow">The GlowParameter to process.</param>
      /// <param name="path">The path of the parameter. The last number in the path
      /// is the parameter's number.</param>
      protected abstract void OnParameter(GlowParameterBase glow, int[] path);

      /// <summary>
      /// Called for every GlowMatrix found in the glow tree.
      /// </summary>
      /// <param name="glow">The GlowMatrix to process.</param>
      /// <param name="path">The path of the matrix. The last number in the path
      /// is the matrix's number.</param>
      protected abstract void OnMatrix(GlowMatrixBase glow, int[] path);

      /// <summary>
      /// Called for every GlowStreamEntry found in the glow tree.
      /// Since stream entries can only be contained in a GlowStreamCollection,
      /// which itself is the root of the glow tree, there is no path parameter
      /// passed to this method.
      /// </summary>
      /// <param name="glow">The GlowStreamEntry to process.</param>
      protected abstract void OnStreamEntry(GlowStreamEntry glow);

      #region Implementation
      LinkedList<int> _path = new LinkedList<int>();

      void Push(int number)
      {
         _path.AddLast(number);
      }

      void Pop()
      {
         _path.RemoveLast();
      }

      int[] PathToArray()
      {
         var array = new int[_path.Count];
         var index = 0;

         foreach(var number in _path)
         {
            array[index] = number;
            index++;
         }

         return array;
      }
      #endregion

      #region IGlowVisitor<object,object> Members
      // Suppress "interface members should be visible to sub-classes".
      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowCommand glow, object state)
      {
         OnCommand(glow, PathToArray());

         return null;
      }

      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowElementCollection glow, object state)
      {
         foreach(var glowElement in glow.Elements)
            glowElement.Accept(this, state);

         return null;
      }

      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowNode glow, object state)
      {
         Push(glow.Number);

         if(glow.HasContents)
            OnNode(glow, PathToArray());

         var glowChildren = glow.Children;

         if(glowChildren != null)
            glowChildren.Accept(this, state);

         Pop();
         return null;
      }

      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowParameter glow, object state)
      {
         Push(glow.Number);

         OnParameter(glow, PathToArray());

         var glowChildren = glow.Children;

         if(glowChildren != null)
            glowChildren.Accept(this, state);

         Pop();
         return null;
      }

      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowStreamCollection glow, object state)
      {
         foreach(var glowEntry in glow.StreamEntries)
            OnStreamEntry(glowEntry);

         return null;
      }

      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowQualifiedParameter glow, object state)
      {
         var glowPath = glow.Path;

         OnParameter(glow, glowPath);

         var glowChildren = glow.Children;

         if(glowChildren != null)
         {
            foreach(var number in glowPath)
               Push(number);

            glowChildren.Accept(this, state);

            _path.Clear();
         }

         return null;
      }

      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowQualifiedNode glow, object state)
      {
         var glowPath = glow.Path;

         if(glow.HasContents)
            OnNode(glow, glowPath);

         var glowChildren = glow.Children;

         if(glowChildren != null)
         {
            foreach(var number in glowPath)
               Push(number);

            glowChildren.Accept(this, state);

            _path.Clear();
         }

         return null;
      }

      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowRootElementCollection glow, object state)
      {
         foreach(var glowElement in glow.Elements)
            glowElement.Accept(this, state);

         return null;
      }

      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowSubContainer glow, object state)
      {
         return null;
      }

      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowMatrix glow, object state)
      {
         Push(glow.Number);

         OnMatrix(glow, PathToArray());

         var glowChildren = glow.Children;

         if(glowChildren != null)
            glowChildren.Accept(this, state);

         Pop();
         return null;
      }

      [SuppressMessage("Microsoft.Design", "CA1033")]
      object IGlowVisitor<object, object>.Visit(GlowQualifiedMatrix glow, object state)
      {
         var glowPath = glow.Path;

         OnMatrix(glow, glowPath);

         var glowChildren = glow.Children;

         if(glowChildren != null)
         {
            foreach(var number in glowPath)
               Push(number);

            glowChildren.Accept(this, state);

            _path.Clear();
         }

         return null;
      }
      #endregion
   }
}
