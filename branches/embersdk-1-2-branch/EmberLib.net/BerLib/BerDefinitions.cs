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

namespace BerLib
{
   /// <summary>
   /// BER tag classes
   /// </summary>
   [Flags]
   public enum BerClass : byte
   {
      /// <summary>
      /// UNIVERSAL tag class
      /// </summary>
      Universal = 0x00,

      /// <summary>
      /// APPLICATION tag class
      /// </summary>
      Application = 0x40,

      /// <summary>
      /// CONTEXT-SPECIFIC tag class
      /// </summary>
      ContextSpecific = 0x80,

      /// <summary>
      /// PRIVATE tag class
      /// </summary>
      Private = 0xC0,

      /// <summary>
      /// Can be used in a bitwise-and operation to
      /// extract the tag class from a tag literal.
      /// </summary>
      Mask = 0xC0,
   }


   /// <summary>
   /// Defines constants for universal ASN.1/BER types and for
   /// defining application-defined BER types.
   /// </summary>
   public static class BerType
   {
      /// <summary>
      /// Uninitialized (invalid) value
      /// </summary>
      public const uint Invalid = 0;

      /// <summary>
      /// ASN.1 type BOOLEAN
      /// </summary>
      public const uint Boolean = 1;

      /// <summary>
      /// ASN.1 type INTEGER
      /// </summary>
      public const uint Integer = 2;
      
      /// <summary>
      /// ASN.1 type BIT STRING
      /// </summary>
      public const uint Bitstring = 3;

      /// <summary>
      /// ASN.1 type OCTET STRING
      /// </summary>
      public const uint OctetString = 4;

      /// <summary>
      /// ASN.1 type NULL
      /// </summary>
      public const uint Null = 5;

      /// <summary>
      /// ASN.1 type OBJECT IDENTIFIER
      /// </summary>
      public const uint ObjectIdentifier = 6;
      
      /// <summary>
      /// ASN.1 type ObjectDescriptor
      /// </summary>
      public const uint ObjectDescriptor = 7;

      /// <summary>
      /// ASN.1 type EXTERNAL
      /// </summary>
      public const uint External = 8;
      
      /// <summary>
      /// ASN.1 type REAL
      /// </summary>
      public const uint Real = 9;

      /// <summary>
      /// ASN.1 type ENUMERATED
      /// </summary>
      public const uint Enumerated = 10;

      /// <summary>
      /// ASN.1 type EMBEDDED PDV
      /// </summary>
      public const uint EmbeddedPdv = 11;
      
      /// <summary>
      /// ASN.1 type UTF8String
      /// </summary>
      public const uint UTF8String = 12;

      /// <summary>
      /// ASN.1 type RELATIVE-OID
      /// </summary>
      public const uint RelativeOid = 13;

      /// <summary>
      /// ASN.1 type SEQUENCE, SEQUENCE OF
      /// </summary>
      public const uint Sequence = 16;

      /// <summary>
      /// ASN.1 type SET, SET OF
      /// </summary>
      public const uint Set = 17;

      /// <summary>
      /// ASN.1 type NumericString
      /// </summary>
      public const uint NumericString = 18;

      /// <summary>
      /// ASN.1 type PrintableString
      /// </summary>
      public const uint PrintableString = 19;

      /// <summary>
      /// ASN.1 type TeletexString
      /// </summary>
      public const uint TeletexString = 20;

      /// <summary>
      /// ASN.1 type VideotexString
      /// </summary>
      public const uint VideotexString = 21;

      /// <summary>
      /// ASN.1 type IA5String
      /// </summary>
      public const uint IA5String = 22;

      /// <summary>
      /// ASN.1 type UTCTime
      /// </summary>
      public const uint UTCTime = 23;

      /// <summary>
      /// ASN.1 type GeneralizedTime
      /// </summary>
      public const uint GeneralizedTime = 24;

      /// <summary>
      /// ASN.1 type GraphicString
      /// </summary>
      public const uint GraphicString = 25;

      /// <summary>
      /// ASN.1 type VisibleString
      /// </summary>
      public const uint VisibleString = 26;

      /// <summary>
      /// ASN.1 type GeneralString
      /// </summary>
      public const uint GeneralString = 27;

      /// <summary>
      /// ASN.1 type UniversalString
      /// </summary>
      public const uint UniversalString = 28;

      /// <summary>
      /// ASN.1 type CHARACTER STRING
      /// </summary>
      public const uint UnspecifiedString = 29;

      /// <summary>
      /// ASN.1 type BMPString
      /// </summary>
      public const uint BMPString = 30;

      /// <summary>
      /// Number of universal definitions. This is also the last number
      /// that - encoded - will fit into 1 byte.
      /// </summary>
      public const uint LastUniversal = 31;

      /// <summary>
      /// This flag is set if a type is not universal but application-defined
      /// (is encoded with class APPLICATION instead of UNIVERSAL).
      /// </summary>
      public const uint ApplicationFlag = 0x80000000;

      /// <summary>
      /// Tests if the passed BerType is application-defined or not.
      /// </summary>
      /// <param name="type">The BerType to test.</param>
      /// <returns>True if <paramref name="type"/> has the ApplicationFlag set.</returns>
      public static bool IsApplicationDefined(uint type)
      {
         return (type & ApplicationFlag) == ApplicationFlag;
      }
   }

   
   /// <summary>
   /// Defines constants used by decoding/encoding entities and
   /// some helper methods.
   /// </summary>
   public static class BerDefinitions
   {
      /// <summary>
      /// Constant used to represent an unkown length. This is
      /// used to signal to an encoder that the indefinite length
      /// form should be used to encode a TLV.
      /// </summary>
      public const int IndefiniteLength = -1;

      
      /// <summary>
      /// Gets the name of the type passed in <paramref name="type"/> in PascalCase
      /// (e.g. 'ObjectIdentifier' if type equals 6).
      /// If <paramref name="type"/> is an application-defined type, returns a name
      /// in the form 'A-1' (for type APPLICATION 1).
      /// </summary>
      /// <param name="type">Usually one of the BerType constants.</param>
      /// <returns></returns>
      public static string GetTypeName(uint type)
      {
         if(BerType.IsApplicationDefined(type))
            return String.Format("A-{0}", type & ~BerType.ApplicationFlag);

         var typeName = null as string;

         if(type < TypeNames.Length)
            typeName = TypeNames[type];

         return typeName ?? type.ToString();
      }

      /// <summary>
      /// Reverses the operation GetTypeName(), returning true if <paramref name="name"/>
      /// could be parsed into a BerType.
      /// </summary>
      public static bool GetTypeFromName(string name, bool ignoreCase, out uint type)
      {
         if(name.StartsWith("A-"))
         {
            var tokens = name.Split('-');

            if(tokens.Length > 1)
            {
               if(uint.TryParse(tokens[1], out type))
               {
                  type |= BerType.ApplicationFlag;
                  return true;
               }
            }
         }
         else
         {
            var comparison = ignoreCase
                             ? StringComparison.CurrentCultureIgnoreCase
                             : StringComparison.CurrentCulture;

            for(int index = 0; index < TypeNames.Length; index++)
            {
               if(String.Compare(TypeNames[index], name, comparison) == 0)
               {
                  type = (uint)index;
                  return true;
               }
            }
         }

         type = BerType.Invalid;
         return false;
      }

      /// <summary>
      /// Gets the name of the tag class identified by <paramref name="tagClass"/>:
      /// either 'C' (context-specific), 'A' (application), 'U' (universal) or
      /// 'P' (private).
      /// </summary>
      public static string GetShortClassName(BerClass tagClass)
      {
         return ShortClassNames[(byte)tagClass >> 6];
      }

      /// <summary>
      /// Reverses the operation GetShortClassName(), returning true
      /// if <paramref name="name"/> could be parsed into a BerClass value.
      /// </summary>
      public static bool GetClassFromName(string name, bool ignoreCase, out BerClass tagClass)
      {
         var comparison = ignoreCase
                          ? StringComparison.CurrentCultureIgnoreCase
                          : StringComparison.CurrentCulture;

         for(int index = 0; index < ShortClassNames.Length; index++)
         {
            if(String.Compare(ShortClassNames[index], name, comparison) == 0)
            {
               tagClass = (BerClass)(index << 6);
               return true;
            }
         }

         tagClass = BerClass.Universal;
         return false;
      }

      #region Implementation
      static readonly string[] TypeNames =
      {
         null,

         "Boolean",
         "Integer",
         "Bitstring",
         "OctetString",
         "Null",
         "ObjectIdentifier",
         "ObjectDescriptor",
         "External",
         "Real",
         "Enumerated",
         "EmbeddedPdv",
         "UTF8",
         "RelativeOid",

         null,
         null,

         "Sequence",
         "Set",
         "NumericString",
         "PrintableString",
         "TeletexString",
         "VideotexString",
         "IA5String",
         "UTCTime",
         "GeneralizedTime",
         "GraphicString",
         "VisibleString",
         "GeneralString",
         "UniversalString",
         "UnspecifiedString",
         "BMPString",
      };

      static readonly string[] ShortClassNames =
      {
         "U",
         "A",
         "C",
         "P",
      };
      #endregion
   }
}
