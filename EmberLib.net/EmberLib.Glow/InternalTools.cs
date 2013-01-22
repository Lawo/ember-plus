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
   static class InternalTools
   {
      public static GlowValue GetValue(EmberContainer container, BerTag tag)
      {
         var node = container[tag];
         var value = null as GlowValue;

         if(node != null)
         {
            switch(node.Type)
            {
               case BerType.Integer:
                  value = new GlowValue(GetIntegerNodeValue(node));
                  break;

               case BerType.Real:
                  value = new GlowValue(((RealEmberLeaf)node).Value);
                  break;

               case BerType.UTF8String:
                  value = new GlowValue(((StringEmberLeaf)node).Value);
                  break;

               case BerType.Boolean:
                  value = new GlowValue(((BooleanEmberLeaf)node).Value);
                  break;

               case BerType.OctetString:
                  value = new GlowValue(((OctetStringEmberLeaf)node).Value);
                  break;
            }
         }

         return value;
      }

      public static void InsertValue(EmberContainer container, BerTag tag, GlowValue value)
      {
         switch(value.Type)
         {
            case GlowParameterType.Integer:
               container.Insert(new LongEmberLeaf(tag, value.Integer));
               break;

            case GlowParameterType.Real:
               container.Insert(new RealEmberLeaf(tag, value.Real));
               break;

            case GlowParameterType.String:
               container.Insert(new StringEmberLeaf(tag, value.String));
               break;

            case GlowParameterType.Boolean:
               container.Insert(new BooleanEmberLeaf(tag, value.Boolean));
               break;

            case GlowParameterType.Octets:
               container.Insert(new OctetStringEmberLeaf(tag, value.Octets));
               break;

            default:
               throw new ArgumentException("Type not supported");
         }
      }

      public static GlowMinMax GetMinMax(EmberContainer container, BerTag tag)
      {
         var node = container[tag];
         var value = null as GlowMinMax;

         if(node != null)
         {
            switch(node.Type)
            {
               case BerType.Integer:
                  value = new GlowMinMax(GetIntegerNodeValue(node));
                  break;

               case BerType.Real:
                  value = new GlowMinMax(((RealEmberLeaf)node).Value);
                  break;
            }
         }

         return value;
      }

      public static void InsertMinMax(EmberContainer container, BerTag tag, GlowMinMax value)
      {
         switch(value.Type)
         {
            case GlowParameterType.Integer:
               container.Insert(new LongEmberLeaf(tag, value.Integer));
               break;

            case GlowParameterType.Real:
               container.Insert(new RealEmberLeaf(tag, value.Real));
               break;

            default:
               throw new ArgumentException("Type not supported");
         }
      }

      public static bool IsIdentifierValid(string identifier)
      {
         if(identifier == null
         || identifier.Length == 0
         || IsValidIdentifierBegin(identifier[0]) == false
         || identifier.Contains("/"))
            return false;

         return true;
      }

      public static void AssertIdentifierValid(string identifier)
      {
         if(identifier == null)
            throw new ArgumentNullException("identifier");

         if(identifier.Length == 0)
            throw new ArgumentException("identifier must not be null!");

         if(IsValidIdentifierBegin(identifier[0]) == false)
            throw new ArgumentException("identifier must begin with a letter or underscore!");

         if(identifier.Contains("/"))
            throw new ArgumentException("identifier must not contain the '/' character!");
      }

      public static IEnumerable<T> EnumerateChildren<T>(EmberContainer container) where T : EmberNode
      {
         foreach(var ember in container)
         {
            var item = ember as T;

            if(item != null)
               yield return item;
         }
      }

      #region Implementation
      static long GetIntegerNodeValue(EmberNode node)
      {
         if(node is LongEmberLeaf)
            return ((LongEmberLeaf)node).Value;

         return ((IntegerEmberLeaf)node).Value;
      }

      static bool IsValidIdentifierBegin(char ch)
      {
         return ch >= 'a' && ch <= 'z'
             || ch >= 'A' && ch <= 'Z'
             || ch == '_';
      }
      #endregion
   }
}
