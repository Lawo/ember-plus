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

namespace BerLib
{
   /// <summary>
   /// Toy to encode/decode ber data using printf/scanf like format strings.
   /// Do not use in production environments.
   /// </summary>
   internal static class BerFormatter
   {
      class TypeTagsContainer
      {
         internal readonly BerTag Sequence = new BerTag(BerType.Sequence, true);
         internal readonly BerTag Boolean = new BerTag(BerType.Boolean);
         internal readonly BerTag Integer = new BerTag(BerType.Integer);
         internal readonly BerTag Real = new BerTag(BerType.Real);
         internal readonly BerTag UTF8String = new BerTag(BerType.UTF8String);
         internal readonly BerTag GeneralizedTime = new BerTag(BerType.GeneralizedTime);
         internal readonly BerTag OctetString = new BerTag(BerType.OctetString);
         internal readonly BerTag RelativeOid = new BerTag(BerType.RelativeOid);
      }

      static readonly TypeTagsContainer TypeTags = new TypeTagsContainer();

      static void EncodeHeader(IBerOutput output, BerTag tag, int length)
      {
         BerEncoding.EncodeTag(output, tag);
         BerEncoding.EncodeLength(output, length);
      }

      static int DecodeHeader(IBerInput input, BerTag expectedTag)
      {
         var tag = BerEncoding.DecodeTag(input);

         if(tag != expectedTag)
            throw new BerException(4001, String.Format("Expected tag {0}, found tag {1}", tag, expectedTag));

         return BerEncoding.DecodeLength(input);
      }

      public static byte[] Encode(string format, params object[] args)
      {
         var output = new BerMemoryOutput();
         var paramIndex = 0;

         for(int charIndex = 0; charIndex < format.Length; charIndex++)
         {
            var ch = format[charIndex];

            switch(ch)
            {
               case '{':
                  EncodeHeader(output, TypeTags.Sequence, BerDefinitions.IndefiniteLength);
                  break;

               case '}':
                  output.WriteByte(0);
                  output.WriteByte(0);
                  break;

               case 'b':
               {
                  EncodeHeader(output, TypeTags.Boolean, 1);
                  BerEncoding.EncodeBoolean(output, (bool)args[paramIndex++]);
                  break;
               }

               case 'i':
               case 'd':
               {
                  var value = (int)args[paramIndex++];
                  var valueLength = BerEncoding.GetIntegerLength(value);

                  EncodeHeader(output, TypeTags.Integer, valueLength);
                  BerEncoding.EncodeInteger(output, value, (int)valueLength);
                  break;
               }

               case 'l':
               case 't':
               {
                  var value = (long)args[paramIndex++];
                  var valueLength = BerEncoding.GetLongLength(value);

                  EncodeHeader(output, TypeTags.Integer, valueLength);
                  BerEncoding.EncodeLong(output, value, (int)valueLength);
                  break;
               }

               case 's':
               {
                  var value = (string)args[paramIndex++];
                  var valueLength = BerEncoding.GetUtf8StringLength(value);

                  EncodeHeader(output, TypeTags.UTF8String, valueLength);
                  BerEncoding.EncodeUtf8String(output, value);
                  break;
               }

               case 'y':
               {
                  var value = (byte[])args[paramIndex++];
                  var valueLength = value.Length;

                  EncodeHeader(output, TypeTags.OctetString, valueLength);
                  BerEncoding.EncodeByteArray(output, value);
                  break;
               }

               case 'f':
               case 'r':
               {
                  var value = (double)args[paramIndex++];
                  var local = new BerMemoryOutput();
                  var valueLength = BerEncoding.EncodeReal(local, value);
                  EncodeHeader(output, TypeTags.Real, valueLength);
                  output.WriteBytes(local.Memory);
                  break;
               }

               case 'o':
               {
                  var value = (int[])args[paramIndex++];
                  var local = new BerMemoryOutput();
                  var valueLength = BerEncoding.EncodeRelativeOid(local, value);
                  EncodeHeader(output, TypeTags.RelativeOid, valueLength);
                  output.WriteBytes(local.Memory);
                  break;
               }

               case 'g':
               {
                  var value = (DateTime)args[paramIndex++];
                  var local = new BerMemoryOutput();
                  var valueLength = BerEncoding.EncodeGeneralizedTime(local, value);
                  EncodeHeader(output, TypeTags.GeneralizedTime, valueLength);
                  output.WriteBytes(local.Memory);
                  break;
               }
            }
         }

         return output.ToArray();
      }

      public static object[] Decode(string format, IList<byte> data)
      {
         var input = new BerMemoryInput(data);
         var objects = new List<object>();

         for(int charIndex = 0; charIndex < format.Length; charIndex++)
         {
            var ch = format[charIndex];
            
            switch(ch)
            {
               case '{':
                  DecodeHeader(input, TypeTags.Sequence);
                  break;

               case '}':
               {
                  if(input.ReadByte() != 0)
                     throw new BerException(4002, "Expected terminator");
                  if(input.ReadByte() != 0)
                     throw new BerException(4002, "Expected terminator");
                  break;
               }

               case 'b':
               {
                  DecodeHeader(input, TypeTags.Boolean);
                  objects.Add(BerEncoding.DecodeBoolean(input));
                  break;
               }

               case 'i':
               case 'd':
               {
                  var valueLength = DecodeHeader(input, TypeTags.Integer);
                  objects.Add(BerEncoding.DecodeInteger(input, valueLength));
                  break;
               }

               case 'l':
               case 't':
               {
                  var valueLength = DecodeHeader(input, TypeTags.Integer);
                  objects.Add(BerEncoding.DecodeLong(input, valueLength));
                  break;
               }

               case 's':
               {
                  var valueLength = DecodeHeader(input, TypeTags.UTF8String);
                  objects.Add(BerEncoding.DecodeUtf8String(input, valueLength));
                  break;
               }

               case 'y':
               {
                  var valueLength = DecodeHeader(input, TypeTags.OctetString);
                  objects.Add(BerEncoding.DecodeByteArray(input, valueLength));
                  break;
               }

               case 'f':
               case 'r':
               {
                  var valueLength = DecodeHeader(input, TypeTags.Real);
                  objects.Add(BerEncoding.DecodeReal(input, valueLength));
                  break;
               }

               case 'o':
               {
                  var valueLength = DecodeHeader(input, TypeTags.RelativeOid);
                  objects.Add(BerEncoding.DecodeRelativeOid(input, valueLength));
                  break;
               }

               case 'g':
               {
                  var valueLength = DecodeHeader(input, TypeTags.GeneralizedTime);
                  objects.Add(BerEncoding.DecodeGeneralizedTime(input, valueLength));
                  break;
               }
            }
         }

         return objects.ToArray();
      }
   }
}
