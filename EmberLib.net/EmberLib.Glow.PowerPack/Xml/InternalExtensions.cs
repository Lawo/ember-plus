/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
