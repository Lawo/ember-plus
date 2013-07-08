using System;
using System.Net;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Ink;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Animation;
using System.Windows.Shapes;
using System.Xml.Linq;

namespace SilverlightEmber
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
