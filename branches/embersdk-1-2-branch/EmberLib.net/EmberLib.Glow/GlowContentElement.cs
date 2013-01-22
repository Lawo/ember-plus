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
using BerLib;

namespace EmberLib.Glow
{
   public abstract class GlowContentElement : GlowElement
   {
      /// <summary>
      /// Creates a new instance of GlowContentElement.
      /// </summary>
      protected GlowContentElement(BerTag? tag, uint type)
      : base(tag, type)
      {
      }

      /// <summary>
      /// Gets or sets the "children" field of the parameter.
      /// Getter returns null if field not present.
      /// </summary>
      public GlowElementCollection Children
      {
         get
         {
            var tag = ChildrenTag;

            return this[tag] as GlowElementCollection;
         }
         set
         {
            if(value.Tag != ChildrenTag)
               throw new ArgumentException("Tag mismatch");

            Remove(ChildrenTag);
            Insert(value);
         }
      }

      /// <summary>
      /// Ensures that the "children" is present.
      /// </summary>
      /// <returns>The value of the "children" field.</returns>
      public GlowElementCollection EnsureChildren()
      {
         var children = Children;

         if(children == null)
         {
            children = new GlowElementCollection(ChildrenTag);
            Insert(children);
         }

         return children;
      }

      /// <summary>
      /// Gets a value indicating whether a "contents" set is present
      /// in the current GlowContentElement.
      /// </summary>
      public bool HasContents
      {
         get { return GetContents() != null; }
      }

      /// <summary>
      /// Override this to return the tag used for the "contents" field.
      /// </summary>
      internal abstract BerTag ContentsTag
      {
         get;
      }

      /// <summary>
      /// Override this to return the tag used for the "children" field.
      /// </summary>
      internal abstract BerTag ChildrenTag
      {
         get;
      }

      protected EmberSet EnsureContents()
      {
         if(_contents == null)
         {
            var tag = ContentsTag;

            _contents = this[tag] as EmberSet;

            if(_contents == null)
               _contents = new EmberSet(tag, this);
         }

         return _contents;
      }

      protected EmberSet EnsureContentsAndRemove(BerTag tag)
      {
         var contents = EnsureContents();

         if(contents != null)
            contents.Remove(tag);

         return contents;
      }

      protected TValue? GetContentValue<TValue>(BerTag tag) where TValue : struct
      {
         var contents = GetContents();

         if(contents != null)
         {
            var childNode = contents[tag] as EmberLeaf<TValue>;

            if(childNode != null)
               return childNode.Value;
         }

         return null;
      }

      protected TValue GetContentRef<TValue>(BerTag tag) where TValue : class
      {
         var contents = GetContents();

         if(contents != null)
         {
            var childNode = contents[tag] as EmberLeaf<TValue>;

            if(childNode != null)
               return childNode.Value;
         }

         return null;
      }

      #region Implementation
      EmberSet _contents;

      EmberSet GetContents()
      {
         if(_contents == null)
            _contents = this[ContentsTag] as EmberSet;

         return _contents;
      }
      #endregion
   }
}
