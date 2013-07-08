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
using System.Xml.Linq;

namespace EmberLib.Glow.PowerPack.Xml
{
   static class NullableExtensions
   {
      public static void Do<T>(this Nullable<T> nullable, Action<T> action) where T : struct
      {
         if(nullable.HasValue)
            action(nullable.Value);
      }
   }

   static class StringExtensions
   {
      public static void Do(this string str, Action<string> action)
      {
         if(str != null)
            action(str);
      }
   }

   static class XElementExtensions
   {
      public static void Do(this XElement xml, Action<string> action)
      {
         if(xml != null)
            action(xml.Value);
      }
   }
}
