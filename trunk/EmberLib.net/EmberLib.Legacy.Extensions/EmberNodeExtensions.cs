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
using BerLib;

namespace EmberLib.Legacy.Extensions
{
   /// <summary>
   /// Defines extensions most import for legacy purposes (Ember1) for EmberLib.EmberNode.
   /// </summary>
   public static class EmberNodeExtensions
   {
      /// <summary>
      /// Inserts a new boolean leaf into this node's collection of children.
      /// </summary>
      /// <param name="tag">The tag of the leaf to create and insert.</param>
      /// <param name="value">The value of the leaf to create and insert.</param>
      public static void Insert(this EmberNode node, BerTag tag, bool value)
      {
         node.Insert(new BooleanEmberLeaf(tag, value));
      }

      /// <summary>
      /// Inserts a new INTEGER (32bit) leaf into this node's collection of children.
      /// </summary>
      /// <param name="tag">The tag of the leaf to create and insert.</param>
      /// <param name="value">The value of the leaf to create and insert.</param>
      public static void Insert(this EmberNode node, BerTag tag, int value)
      {
         node.Insert(new IntegerEmberLeaf(tag, value));
      }

      /// <summary>
      /// Inserts a new INTEGER (64bit) leaf into this node's collection of children.
      /// </summary>
      /// <param name="tag">The tag of the leaf to create and insert.</param>
      /// <param name="value">The value of the leaf to create and insert.</param>
      public static void Insert(this EmberNode node, BerTag tag, long value)
      {
         node.Insert(new LongEmberLeaf(tag, value));
      }

      /// <summary>
      /// Inserts a new REAL leaf into this node's collection of children.
      /// </summary>
      /// <param name="tag">The tag of the leaf to create and insert.</param>
      /// <param name="value">The value of the leaf to create and insert.</param>
      public static void Insert(this EmberNode node, BerTag tag, double value)
      {
         node.Insert(new RealEmberLeaf(tag, value));
      }

      /// <summary>
      /// Inserts a new UTF8String leaf into this node's collection of children.
      /// </summary>
      /// <param name="tag">The tag of the leaf to create and insert.</param>
      /// <param name="value">The value of the leaf to create and insert.</param>
      public static void Insert(this EmberNode node, BerTag tag, string value)
      {
         node.Insert(new StringEmberLeaf(tag, value));
      }

      /// <summary>
      /// Inserts a new OCTET STRING leaf into this node's collection of children.
      /// </summary>
      /// <param name="tag">The tag of the leaf to create and insert.</param>
      /// <param name="value">The value of the leaf to create and insert.</param>
      public static void Insert(this EmberNode node, BerTag tag, byte[] value)
      {
         node.Insert(new OctetStringEmberLeaf(tag, value));
      }

      /// <summary>
      /// Inserts a new OBJECT IDENTIFIER leaf into this node's collection of children.
      /// </summary>
      /// <param name="tag">The tag of the leaf to create and insert.</param>
      /// <param name="values">The value of the leaf to create and insert.</param>
      public static void InsertOid(this EmberNode node, BerTag tag, int[] values)
      {
         node.Insert(new ObjectIdentifierEmberLeaf(tag, values));
      }

      /// <summary>
      /// Inserts a new RELATIVE OBJECT leaf into this node's collection of children.
      /// </summary>
      /// <param name="tag">The tag of the leaf to create and insert.</param>
      /// <param name="value">The value of the leaf to create and insert.</param>
      public static void InsertRelativeOid(this EmberNode node, BerTag tag, int[] value)
      {
         node.Insert(new RelativeOidEmberLeaf(tag, value));
      }

      /// <summary>
      /// Gets the value of the child leaf with the specified tag.
      /// Throws an exception if this EmberNode is not derived from EmberContainer.
      /// </summary>
      /// <typeparam name="TValue">The type of the leaf value to get.</typeparam>
      /// <param name="tag">The tag of the leaf whose value to return.</param>
      /// <param name="value">Received the value of the child node with the specified tag
      /// if a child leaf of type TValue exists.</param>
      /// <returns>True if a fitting child leaf exists and <paramref name="value"/>
      /// has received its value.</returns>
      public static bool Get<TValue>(this EmberNode node, BerTag tag, out TValue value)
      {
         var leaf = node[tag] as EmberLeaf<TValue>;

         if(leaf != null)
         {
            value = leaf.Value;
            return true;
         }
         else
         {
            value = default(TValue);
            return false;
         }
      }

      /// <summary>
      /// Gets the value of the child leaf with the specified tag.
      /// Throws an exception if this EmberNode is not derived from EmberContainer or
      /// if no child node of type EmberLeaf&lt;TValue&gt; with the specified tag exists.
      /// It is strongly recommended to use type specific methods such as GetInteger(),
      /// GetDateTime() etc. Only use this method if there is no specialized
      /// method for the type you want to get.
      /// </summary>
      /// <typeparam name="TValue">The type of the leaf value to get.</typeparam>
      /// <param name="tag">The tag of the leaf whose value to return.</param>
      /// <returns>The value of the child node with the specified tag.</returns>
      public static TValue Get<TValue>(this EmberNode node, BerTag tag)
      {
         var leaf = node[tag] as EmberLeaf<TValue>;

         if(leaf != null)
            return leaf.Value;

         throw new BerException(5, "Leaf not found or type mismatch");
      }

      /// <summary>
      /// Gets the integer value of the child leaf with the specified tag.
      /// Throws an exception if this EmberNode is not derived from EmberContainer or
      /// if no child node of type EmberLeaf&lt;int&gt; or EmberLeaf&lt;long&gt; with the specified tag exists.
      /// </summary>
      /// <param name="tag">The tag of the leaf whose value to return.</param>
      /// <returns>The value of the child node with the specified tag.</returns>
      public static long GetInteger(this EmberNode node, BerTag tag)
      {
         long value;

         if(GetInteger(node, tag, out value))
            return value;

         throw new BerException(5, "Leaf not found or type mismatch");
      }

      /// <summary>
      /// Gets the integer value of the child leaf with the specified tag.
      /// Throws an exception if this EmberNode is not derived from EmberContainer.
      /// </summary>
      /// <param name="tag">The tag of the leaf whose value to return.</param>
      /// <param name="value">Received the value of the child node with the specified tag
      /// if a child leaf of type int or long exists.</param>
      /// <returns>True if a fitting child leaf exists and <paramref name="value"/>
      /// has received its value.</returns>
      public static bool GetInteger(this EmberNode node, BerTag tag, out long value)
      {
         var child = node[tag];

         if(child != null
         && child.BerTypeNumber == BerType.Integer)
         {
            var integerLeaf = child as EmberLeaf<int>;

            if(integerLeaf != null)
            {
               value = integerLeaf.Value;
               return true;
            }

            var longLeaf = child as EmberLeaf<long>;

            if(longLeaf != null)
            {
               value = longLeaf.Value;
               return true;
            }
         }

         value = 0;
         return false;
      }

      /// <summary>
      /// Gets the RELATIVE OBJECT value of the child leaf with the specified tag.
      /// Throws an exception if this EmberNode is not derived from EmberContainer.
      /// </summary>
      /// <param name="tag">The tag of the leaf whose value to return.</param>
      /// <param name="value">Receives the value of the child node with the specified tag
      /// if a child leaf of type RelativeOidEmberLeaf exists.</param>
      /// <returns>True if a fitting child leaf exists and <paramref name="value"/>
      /// has received its value.</returns>
      public static bool GetRelativeOid(this EmberNode node, BerTag tag, out int[] value)
      {
         var child = node[tag];

         if(child != null
         && child.BerTypeNumber == BerType.RelativeOid)
         {
            var leaf = child as RelativeOidEmberLeaf;

            if(leaf != null)
            {
               value = leaf.Value;
               return true;
            }
         }

         value = null;
         return false;
      }

      /// <summary>
      /// Gets the RELATIVE OBJECT value of the child leaf with the specified tag.
      /// Throws an exception if this EmberNode is not derived from EmberContainer or
      /// if no child node of type EmberLeaf&lt;int[]&gt; with the specified tag exists.
      /// </summary>
      /// <param name="tag">The tag of the leaf whose value to return.</param>
      /// <returns>The value of the child node with the specified tag.</returns>
      public static int[] GetRelativeOid(this EmberNode node, BerTag tag)
      {
         int[] value;

         if(GetRelativeOid(node, tag, out value))
            return value;

         throw new BerException(5, "leaf not found or leaf type mismatch");
      }
   }
}
