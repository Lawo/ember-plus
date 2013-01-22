using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace EmberPlusRouter
{
   static class Int32Extensions
   {
      public static bool HasBits(this int flags, int bits)
      {
         return (flags & bits) == bits;
      }
   }
}
