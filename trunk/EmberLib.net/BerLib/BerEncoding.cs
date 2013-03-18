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
using System.Diagnostics;

namespace BerLib
{
   public static class BerEncoding
   {
      public static readonly byte[] IndefiniteLengthTerminator = new byte[] { 0, 0, 0, 0 };

      public static int GetTagLength(BerTag tag)
      {
         var number = tag.Number;

         if(number < 0x1F)
            return 1;

         return 1 + GetMultiByteLongLength(number);
      }

      public static int GetIntegerLength(int value)
      {
         var dword = (uint)value;
         uint mask = 0xFF800000;
         var length = 4;

         if(value < 0)
         {
            for(; (dword & mask) == mask && length > 1; mask >>= 8)
               length--;
         }
         else
         {
            uint topBitMask = 0x80000000;

            for(; (dword & mask) == 0 && length > 1; mask >>= 8)
            {
               topBitMask >>= 8;
               length--;
            }

            if((dword & topBitMask) != 0)
               length++; // include leading zero octet to signal positive value
         }

         return length;
      }

      public static int GetLongLength(long value)
      {
         return GetLongLength(value, true);
      }

      public static int GetMultiByteIntegerLength(uint value)
      {
         var size = 1;

         if((value & 0xF0000000) != 0) // most significant 4 bits
            size++;

         if((value & 0xFFE00000) != 0) // most significant 11 bits
            size++;

         if((value & 0xFFFFC000) != 0) // most significant 18 bits
            size++;

         if((value & 0xFFFFFF80) != 0) // most significant 25 bits
            size++;

         return size;
      }

      public static int GetMultiByteLongLength(ulong value)
      {
         var size = 1;

         if((value & 0x8000000000000000UL) != 0) // most significant 1 bits
            size++;

         if((value & 0xFF00000000000000UL) != 0) // most significant 8 bits
            size++;

         if((value & 0xFFFE000000000000UL) != 0) // most significant 15 bits
            size++;

         if((value & 0xFFFFFC0000000000UL) != 0) // most significant 22 bits
            size++;

         if((value & 0xFFFFFFF800000000UL) != 0) // most significant 29 bits
            size++;

         if((value & 0xFFFFFFFFF0000000UL) != 0) // most significant 36 bits
            size++;

         if((value & 0xFFFFFFFFFFE00000UL) != 0) // most significant 43 bits
            size++;

         if((value & 0xFFFFFFFFFFFFC000UL) != 0) // most significant 50 bits
            size++;

         if((value & 0xFFFFFFFFFFFFFF80UL) != 0) // most significant 57 bits
            size++;

         return size;
      }

      public static int GetHeaderLength(BerTag tag, int length)
      {
         int result = GetTagLength(tag);

         if(length <= 0x7F || length == BerDefinitions.IndefiniteLength)
            return result + 1;

         return result + 1 + GetIntegerLength(length);
      }

      public static int GetUtf8StringLength(string str)
      {
         if(str == null)
            return 0;

         return Encoding.UTF8.GetByteCount(str);
      }
      
      public static int GetAsciiStringLength(string str)
      {
         if(str == null)
            return 0;

         return AsciiEncoding.GetByteCount(str);
      }

      public static int GetObjectIdentifierLength(int[] value)
      {
         if(value.Length > 1)
         {
            var size = GetObjectSubidentifierLength(value[0] * 40 + value[1]);

            for(int index = 2; index < value.Length; index++)
               size += GetObjectSubidentifierLength(value[index]);

            return size;
         }

         return 0;
      }

      public static int GetRelativeOidLength(int[] value)
      {
         var length = 0;

         foreach(var id in value)
            length += GetMultiByteIntegerLength((uint)id);

         return length;
      }


      // ====================================================================
      //
      // Encode functions
      // all return the number of bytes in the
      // encoded result.
      //
      // ====================================================================

      public static int EncodeTag(IBerOutput output, BerTag tag)
      {
         var number = tag.Number;
         var size = 1;
         tag.Preamble &= 0xE0;

         if(number < 0x1F)
         {
            output.WriteByte((byte)(tag.Preamble | (number & 0x1F)));
         }
         else
         {
            output.WriteByte((byte)(tag.Preamble | 0x1F));

            size += EncodeMultiByteInteger(output, number);
         }

         return size;
      }

      public static int EncodeMultiByteInteger(IBerOutput output, uint value)
      {
         var size = 1;

         if((value & 0xF0000000) != 0) // most significant 4 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 28) & 0x0F)));
            size++;
         }

         if((value & 0xFFE00000) != 0) // most significant 11 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 21) & 0x7F)));
            size++;
         }

         if((value & 0xFFFFC000) != 0) // most significant 18 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 14) & 0x7F)));
            size++;
         }

         if((value & 0xFFFFFF80) != 0) // most significant 25 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 7) & 0x7F)));
            size++;
         }

         output.WriteByte((byte)(0x00 | ((value >> 0) & 0x7F)));

         return size;
      }

      public static int EncodeMultiByteLong(IBerOutput output, ulong value)
      {
         var size = 1;

         if((value & 0x8000000000000000UL) != 0) // most significant 1 bits
         {
            output.WriteByte((byte)(0x80 | 1));
            size++;
         }

         if((value & 0xFF00000000000000UL) != 0) // most significant 8 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 56) & 0x7F)));
            size++;
         }

         if((value & 0xFFFE000000000000UL) != 0) // most significant 15 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 49) & 0x7F)));
            size++;
         }

         if((value & 0xFFFFFC0000000000UL) != 0) // most significant 22 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 42) & 0x7F)));
            size++;
         }

         if((value & 0xFFFFFFF800000000UL) != 0) // most significant 29 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 35) & 0x7F)));
            size++;
         }

         if((value & 0xFFFFFFFFF0000000UL) != 0) // most significant 36 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 28) & 0x7F)));
            size++;
         }

         if((value & 0xFFFFFFFFFFE00000UL) != 0) // most significant 43 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 21) & 0x7F)));
            size++;
         }

         if((value & 0xFFFFFFFFFFFFC000UL) != 0) // most significant 50 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 14) & 0x7F)));
            size++;
         }

         if((value & 0xFFFFFFFFFFFFFF80UL) != 0) // most significant 57 bits
         {
            output.WriteByte((byte)(0x80 | ((value >> 7) & 0x7F)));
            size++;
         }

         output.WriteByte((byte)(0x00 | ((value >> 0) & 0x7F)));

         return size;
      }

      public static int EncodeLength(IBerOutput output, int value)
      {
         int size = 1;

         if(value == BerDefinitions.IndefiniteLength)
         {
            output.WriteByte(0x80);
         }
         else
         {
            if(value <= 0x7F)
            {
               output.WriteByte((byte)(value & 0x7F));
            }
            else
            {
               int integerLength = GetIntegerLength((int)value);
               output.WriteByte((byte)(0x80 | integerLength));

               size += EncodeInteger(output, (int)value, integerLength);
            }
         }

         return size;
      }

      // value encodings
      public static int EncodeBoolean(IBerOutput output, bool value)
      {
         output.WriteByte((byte)(value ? 0xFF : 0));
         return 1;
      }

      public static int EncodeInteger(IBerOutput output, int value, int length)
      {
         Debug.Assert(length > 0 && length <= 4);

         var dword = (uint)value;
         var bits = length * 8;

         while(bits > 0)
         {
            bits -= 8;

            output.WriteByte((byte)((dword >> bits) & 0xFF));
         }

         return length;
      }

      public static int EncodeLong(IBerOutput output, long value, int length)
      {
         Debug.Assert(length > 0 && length <= 8);

         ulong qword = (ulong)value;
         int bits = length * 8;

         while(bits > 0)
         {
            bits -= 8;

            output.WriteByte((byte)((qword >> bits) & 0xFF));
         }

         return length;
      }

      public static int EncodeReal(IBerOutput output, double value)
      {
         var size = 0;

         if(double.IsPositiveInfinity(value))
         {
            output.WriteByte(0x40); // 01000000 Value is PLUS-INFINITY
            size = 1;
         }
         else if(double.IsNegativeInfinity(value)) // negative infinity
         {
            output.WriteByte(0x41); // 01000001 Value is MINUS-INFINITY
            size = 1;
         }
         else
         {
            long longValue = DoubleToInt64Bits(value);

            if(longValue != 0)
            {
               long exponent = ((0x7FF0000000000000L & longValue) >> 52) - 1023;
               long mantissa =   0x000FFFFFFFFFFFFFL & longValue;
               mantissa |=       0x0010000000000000L; // set virtual delimeter

               // normalize mantissa (required by CER and DER)
               while((mantissa & 0xFF) == 0)
                  mantissa >>= 8;

               while((mantissa & 0x01) == 0)
                  mantissa >>= 1;

               int exponentLength = GetLongLength(exponent);
               Debug.Assert(exponentLength <= 3);

               byte preamble = 0x80;
               preamble |= (byte)(exponentLength - 1);

               if(((ulong)longValue & 0x8000000000000000UL) != 0)
                  preamble |= 0x40; // Sign

               output.WriteByte(preamble);

               size++;
               size += EncodeLong(output, exponent, exponentLength); // signed exponent
               size += EncodeLong(output, mantissa, GetLongLength(mantissa, false)); // unsigned mantissa
            }
         }

         return size;
      }

      public static int EncodeUtf8String(IBerOutput output, string value)
      {
         byte[] bytes = Encoding.UTF8.GetBytes(value);

         output.WriteBytes(bytes);

         return bytes.Length;
      }

      public static int EncodeAsciiString(IBerOutput output, string value)
      {
         byte[] bytes = AsciiEncoding.GetBytes(value);

         output.WriteBytes(bytes);

         return bytes.Length;
      }

      public static int EncodeByteArray(IBerOutput output, byte[] value)
      {
         output.WriteBytes(value);

         return value.Length;
      }

      public static int EncodeRelativeOid(IBerOutput output, int[] value)
      {
         var size = 0;

         foreach(var id in value)
            size += EncodeMultiByteInteger(output, (uint)id);

         return size;
      }

      public static int EncodeObjectIdentifier(IBerOutput output, int[] values)
      {
         var bytes = new List<byte>(4);
         var size = 0;

         if(values.Length > 1)
         {
            size = EncodeObjectSubidentifier(output, values[0] * 40 + values[1], bytes);

            for(int index = 2; index < values.Length; index++)
               size += EncodeObjectSubidentifier(output, values[index], bytes);
         }

         return size;
      }

      public static int EncodeGeneralizedTime(IBerOutput output, DateTime value) // encoded as GeneralizedTime (ASCII)
      {
         var utcTime = value.ToUniversalTime();

         string sBuffer = String.Format(
            "{0:0000}{1:00}{2:00}{3:00}{4:00}{5:00}.{6}Z",
            utcTime.Year,
            utcTime.Month,
            utcTime.Day,
            utcTime.Hour,
            utcTime.Minute,
            utcTime.Second,
            utcTime.Millisecond);

         return EncodeAsciiString(output, sBuffer);
      }


      // ====================================================================
      //
      // Decode functions
      //
      // ====================================================================

      public static BerTag DecodeTag(IBerInput input)
      {
         var tagByte = input.ReadByte();
         var tag = new BerTag((byte)(tagByte & 0xE0), (uint)(tagByte & 0x1F));

         if(tag.Number == 0x1F)
            tag.Number = DecodeMultiByteInteger(input);

         return tag;
      }

      public static int DecodeLength(IBerInput input)
      {
         int value;
         int byteCount;

         value = input.ReadByte();

         if((value & 0x80) != 0)
         {
            byteCount = value & 0x7F;

            if(byteCount == 0)
            {
               value = BerDefinitions.IndefiniteLength; // indefinite length form
            }
            else
            {
               value = 0;

               for( ; byteCount > 0; byteCount--)
                  value = (value << 8) | input.ReadByte();
            }
         }

         return value;
      }

      public static uint DecodeMultiByteInteger(IBerInput input, out int consumedByteCount)
      {
         byte currentByte;
         uint value = 0;
         var count = 0;

         do
         {
            currentByte = input.ReadByte();
            value = (value << 7) | (byte)(currentByte & ~0x80);
            count++;

         } while((currentByte & 0x80) != 0);

         consumedByteCount = count;
         return value;
      }

      public static uint DecodeMultiByteInteger(IBerInput input)
      {
         int count;

         return DecodeMultiByteInteger(input, out count);
      }

      public static ulong DecodeMultiByteLong(IBerInput input)
      {
         byte currentByte;
         ulong value = 0;

         do
         {
            currentByte = input.ReadByte();
            value = (value << 7) | (byte)(currentByte & ~0x80);

         } while((currentByte & 0x80) != 0);

         return value;
      }

      public static bool DecodeBoolean(IBerInput input)
      {
         return input.ReadByte() != 0;
      }

      public static int DecodeInteger(IBerInput input, int length)
      {
         int value = 0;
         int readByte;

         for(uint byteCount = 0; byteCount < length; byteCount++)
         {
            readByte = input.ReadByte();

            if(byteCount == 0 && (readByte & 0x80) != 0)
               readByte -= 0x100;

            value = (value << 8) | readByte;
         }

         return value;
      }

      public static long DecodeLong(IBerInput input, int length)
      {
         return DecodeLong(input, length, true);
      }

      public static double DecodeReal(IBerInput input, int length)
      {
         double value = 0;

         if(length != 0)
         {
            byte preamble = input.ReadByte();

            if(length == 1 && preamble == 0x40) // positive infinity
            {
               value = double.PositiveInfinity;
            }
            else if(length == 1 && preamble == 0x41) // negative infinity
            {
               value = double.NegativeInfinity;
            }
            else
            {
               long longValue;

               int exponentLength = 1 + (preamble & 3);
               int sign = preamble & 0x40;
               int ff = (preamble >> 2) & 3;

               // Unpack mantissa & decrement exponent for base 2
               long exponent = DecodeLong(input, exponentLength);
               long mantissa = DecodeLong(input, length - exponentLength - 1, false) << ff;

               // de-normalize mantissa (required by CER and DER)
               while((mantissa & 0x7FFFF00000000000L) == 0x0)
                  mantissa <<= 8;

               while((mantissa & 0x7FF0000000000000L) == 0x0)
                  mantissa <<= 1;

               mantissa &= 0x0FFFFFFFFFFFFFL;

               longValue = (exponent + 1023) << 52;
               longValue |= mantissa;

               if(sign != 0)
               {
                  ulong qword = (ulong)longValue | 0x8000000000000000UL;
                  longValue = (long)qword;
               }

               value = Int64BitsToDouble((long)longValue);
            }
         }

         return value;
      }

      public static string DecodeUtf8String(IBerInput input, int length)
      {
         var intLength = (int)length;
         var bytes = ReadString(input, ref intLength);

         return Encoding.UTF8.GetString(bytes, 0, intLength);
      }

      public static string DecodeAsciiString(IBerInput input, int length)
      {
         var intLength = (int)length;
         var bytes = ReadString(input, ref intLength);

         return AsciiEncoding.GetString(bytes, 0, intLength);
      }

      public static byte[] DecodeByteArray(IBerInput input, int length)
      {
         var bytes = new byte[length];

         for(int index = 0; index < length; index++)
            bytes[index] = input.ReadByte();

         return bytes;
      }

      public static int[] DecodeRelativeOid(IBerInput input, int length)
      {
         var subidentifiers = new List<int>();

         while(length > 0)
         {
            int count;
            subidentifiers.Add((int)DecodeMultiByteInteger(input, out count));

            length -= count;
         }

         return subidentifiers.ToArray();
      }

      public static int[] DecodeObjectIdentifier(IBerInput input, int length)
      {
         var values = new List<int>();
         int subidentifier = 0;
         uint bytesRead = 0;

         while(bytesRead++ < length)
         {
            byte uByte = input.ReadByte();

            subidentifier = ((subidentifier << 7) | (uByte & 0x7F));

            if((uByte & 0x80) == 0)
            {
               if(values.Count > 0)
               {
                  values.Add(subidentifier);
               }
               else
               {
                  values.Add(subidentifier / 40);
                  values.Add(subidentifier % 40);
               }

               subidentifier = 0;
            }
         }

         return values.ToArray();
      }


      public static DateTime DecodeGeneralizedTime(IBerInput input, int length)
      {
         var str = DecodeAsciiString(input, length);
         int year, month, day, hour, minute, second, millisecond;
         var isUtc = false;

         try
         {
            year = int.Parse(str.Substring(0, 4));
            month = int.Parse(str.Substring(4, 2));
            day = int.Parse(str.Substring(6, 2));
            hour = int.Parse(str.Substring(8, 2));
            minute = int.Parse(str.Substring(10, 2));
            second = int.Parse(str.Substring(12, 2));

            var suffix = str.Substring(15);
            if(suffix.EndsWith("Z")) // UTC
            {
               suffix = suffix.Substring(0, suffix.Length - 1);
               isUtc = true;
            }

            millisecond = int.Parse(suffix);

            return new DateTime(year, month, day, hour, minute, second, millisecond, isUtc ? DateTimeKind.Utc : DateTimeKind.Local);
         }
         catch(Exception ex)
         {
            Debug.WriteLine(ex);

            return DateTime.MinValue;
         }
      }

      #region Implementation
      static byte[] ReadString(IBerInput input, ref int length)
      {
         var bytes = new byte[length];

         if(length > 0)
         {
            for(int nIndex = 0; nIndex < length; nIndex++)
               bytes[nIndex] = input.ReadByte();

            // strip trailing zeroes
            while(length > 0 && bytes[length - 1] == 0)
               length--;
         }

         return bytes;
      }

      static int GetObjectSubidentifierLength(int value)
      {
         var length = 1;

         while((value = value >> 7) > 0)
            length++;

         return length;
      }

      static int EncodeObjectSubidentifier(IBerOutput output, int value, List<byte> bytes)
      {
         var size = 1;

         bytes.Clear();
         bytes.Add((byte)(value & 0x7F));

         while((value = value >> 7) > 0)
         {
            bytes.Add((byte)((value & 0x7F) | 0x80));
            size++;
         }

         bytes.Reverse();
         output.WriteBytes(bytes.ToArray());

         return size;
      }

      static Encoding AsciiEncoding
      {
         get { return Encoding.GetEncoding("us-ascii"); }
      }

      static double Int64BitsToDouble(long value)
      {
         return BitConverter.ToDouble(BitConverter.GetBytes(value), 0);
      }

      static long DoubleToInt64Bits(double value)
      {
         return BitConverter.ToInt64(BitConverter.GetBytes(value), 0);
      }

      static int GetLongLength(long value, bool isSigned)
      {
         var qword = (ulong)value;
         ulong mask = 0xFF80000000000000UL;
         var length = 8;

         if(value < 0)
         {
            for(; (qword & mask) == mask && length > 1; mask >>= 8)
               length--;
         }
         else
         {
            ulong topBitMask = 0x8000000000000000UL;

            for(; (qword & mask) == 0 && length > 1; mask >>= 8)
            {
               topBitMask >>= 8;
               length--;
            }

            if((qword & topBitMask) != 0 && isSigned)
               length++;
         }

         return length;
      }

      static long DecodeLong(IBerInput input, int length, bool isSigned)
      {
         long value = 0;
         long readByte;

         for(uint byteCount = 0; byteCount < length; byteCount++)
         {
            readByte = input.ReadByte();

            if(byteCount == 0
            && (readByte & 0x80) != 0
            && isSigned)
               readByte -= 0x100;

            value = (value << 8) | readByte;
         }

         return value;
      }
      #endregion
   }
}
