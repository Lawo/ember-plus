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
using BerLib;
using System.Globalization;

namespace EmberLib.Glow
{
   /// <summary>
   /// A type modelling a conceptual union of all supported parameter value types
   /// </summary>
   public class GlowValue : GlowMinMax
   {
      /// <summary>
      /// Creates a new instance of GlowValue containing an integer value.
      /// Type is set to GlowParameterType.Integer.
      /// </summary>
      /// <param name="value">The integer value to contain.</param>
      public GlowValue(long value)
      : base(value)
      {
      }

      /// <summary>
      /// Creates a new instance of GlowValue containing a real value.
      /// Type is set to GlowParameterType.Real.
      /// </summary>
      /// <param name="value">The real value to contain.</param>
      public GlowValue(double value)
      : base(value)
      {
      }

      /// <summary>
      /// Creates a new instance of GlowValue containing a string value.
      /// Type is set to GlowParameterType.UTF8String.
      /// </summary>
      /// <param name="value">The string value to contain.</param>
      public GlowValue(string value)
      : base(GlowParameterType.String)
      {
         _string = value;
      }

      /// <summary>
      /// Creates a new instance of GlowValue containing a boolean value.
      /// Type is set to GlowParameterType.Boolean.
      /// </summary>
      /// <param name="value">The boolean value to contain</param>
      public GlowValue(bool value)
      : base(GlowParameterType.Boolean)
      {
         _boolean = value;
      }

      /// <summary>
      /// Creates a new instance of GlowValue containing an octet string value.
      /// Type is set to GlowParameterType.Octets.
      /// </summary>
      /// <param name="value">The octet string value to contain</param>
      public GlowValue(byte[] value)
      : base(GlowParameterType.Octets)
      {
         _octets = value;
      }

      /// <summary>
      /// Gets the contained string value. Throws an exception
      /// if type if not GlowParameterType.String.
      /// </summary>
      public string String
      {
         get
         {
            AssertType(GlowParameterType.String);

            return _string;
         }
      }

      /// <summary>
      /// Gets the contained boolean value. Throws an exception
      /// if type if not GlowParameterType.Boolean.
      /// </summary>
      public bool Boolean
      {
         get
         {
            AssertType(GlowParameterType.Boolean);

            return _boolean;
         }
      }

      /// <summary>
      /// Gets the contained octet string value. Throws an exception
      /// if type if not GlowParameterType.Octets.
      /// </summary>
      public byte[] Octets
      {
         get
         {
            AssertType(GlowParameterType.Octets);

            return _octets;
         }
      }

      /// <summary>
      /// Converts the contained value into a string, according to
      /// the format provided.
      /// </summary>
      /// <param name="provider">The format to use.</param>
      /// <returns>A string representation of the contained value.</returns>
      public override string ToString(IFormatProvider provider)
      {
         switch(Type)
         {
            case GlowParameterType.String:
               return String;

            case GlowParameterType.Boolean:
               return Boolean.ToString();

            case GlowParameterType.Octets:
            {
               var buffer = new StringBuilder();

               foreach(var b in Octets)
                  buffer.AppendFormat("{0:X2}", b);

               return buffer.ToString();
            }
         }

         return base.ToString(provider);
      }

      /// <summary>
      /// Tries to parse a GlowValue value of the specified type from a string.
      /// </summary>
      /// <param name="str">The string to parse.</param>
      /// <param name="type">The expected parameter type of the value. Must be either GlowParameterType.Integer,
      /// GlowParameterType.Real, GlowParameterType.String, GlowParameterType.Boolean or GlowParameterType.Octets.</param>
      /// <param name="provider">The string format to use.</param>
      /// <param name="value">If successful, receives the parsed value.</param>
      /// <returns>True if the passed string could be parsed, otherwise false.</returns>
      public static bool TryParse(string str, int type, IFormatProvider provider, out GlowValue value)
      {
         if(str == null)
            throw new ArgumentNullException("str");

         switch(type)
         {
            case GlowParameterType.Integer:
            {
               long integer;

               if(Int64.TryParse(str, NumberStyles.Any, provider, out integer))
               {
                  value = new GlowValue(integer);
                  return true;
               }

               break;
            }

            case GlowParameterType.Real:
            {
               double real;

               if(Double.TryParse(str, NumberStyles.Any, provider, out real))
               {
                  value = new GlowValue(real);
                  return true;
               }

               break;
            }

            case GlowParameterType.String:
               value = new GlowValue(str);
               return true;

            case GlowParameterType.Boolean:
            {
               bool boolean;

               if(Boolean.TryParse(str, out boolean))
               {
                  value = new GlowValue(boolean);
                  return true;
               }

               break;
            }

            case GlowParameterType.Octets:
            {
               if((str.Length & 1) == 0) // dividable by 2
               {
                  var octets = new List<byte>();
                  var success = true;

                  for(int index = 0; index < str.Length; index += 2)
                  {
                     var sub = str.Substring(index, 2);
                     byte b;

                     if(Byte.TryParse(sub, NumberStyles.HexNumber, provider, out b))
                     {
                        octets.Add(b);
                     }
                     else
                     {
                        success = false;
                        break;
                     }
                  }

                  if(success)
                  {
                     value = new GlowValue(octets.ToArray());
                     return true;
                  }
               }

               break;
            }

            default:
               throw new ArgumentException("Unsupported type");
         }

         value = null;
         return false;
      }

      #region Implementation
      string _string;
      bool _boolean;
      byte[] _octets;
      #endregion
   }
}
