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
using System.Diagnostics.CodeAnalysis;
using System.Text;
using EmberLib.Glow.Formula;

namespace EmberLib.Glow.PowerPack
{
   /// <summary>
   /// Provides helpers for converting GlowValues according to the conversion rules
   /// defined by a parameter (formula, factor, stream descriptor).
   /// </summary>
   public static class ValueConversion
   {
      /// <summary>
      /// Provides value conversion for values coming from the provider and converted by
      /// the consumer.
      /// </summary>
      public static readonly ProviderToConsumerConversion ProviderToConsumer = new ProviderToConsumerConversion();

      /// <summary>
      /// Provides value conversion for values to be sent from the consumer to the provider.
      /// </summary>
      public static readonly ConsumerToProviderConversion ConsumerToProvider = new ConsumerToProviderConversion();

      #region Internals
      /// <summary>
      /// Compile a single formula and log errors with the log callback.
      /// </summary>
      internal static ICompilationResult CompileFormula(string formula, Action<string> log)
      {
         ICompilationResult result;

         lock(s_sync)
         {
            if(s_compilationResults.TryGetValue(formula, out result) == false)
            {
               if(formula.StartsWith("("))
               {
                  result = Compiler.Compile(formula, true);

                  if(result.Success == false)
                  {
                     if(log != null)
                        log(String.Format("Error compiling formula '{0}': {1}", formula, result.Error.Message));
                  }
               }
               else
               {
                  result = InvalidFormulaCompilationResult.Instance;

                  if(log != null)
                     log(String.Format("Formula '{0}': {1}", formula, result.Error.Message));
               }

               s_compilationResults.Add(formula, result);
            }
         }

         return result;
      }

      /// <summary>
      /// Converts native data as described by GlowStreamFormat to GlowValue or double.
      /// Only works on little-endian systems!
      /// </summary>
      internal static GlowValue ValueFromOctets(byte[] octets, int format, int offset)
      {
         if(BitConverter.IsLittleEndian == false)
            throw new InvalidOperationException();

         unchecked
         {
            switch(format)
            {
               case GlowStreamFormat.UnsignedInt8:
                  return new GlowValue((long)octets[offset]);

               case GlowStreamFormat.UnsignedInt16BigEndian:
                  return new GlowValue((long)SwapByteOrder16(BitConverter.ToUInt16(octets, offset)));

               case GlowStreamFormat.UnsignedInt16LittleEndian:
                  return new GlowValue((long)BitConverter.ToUInt16(octets, offset));

               case GlowStreamFormat.UnsignedInt32BigEndian:
                  return new GlowValue((long)SwapByteOrder32(BitConverter.ToUInt32(octets, offset)));

               case GlowStreamFormat.UnsignedInt32LittleEndian:
                  return new GlowValue((long)BitConverter.ToUInt32(octets, offset));

               case GlowStreamFormat.UnsignedInt64BigEndian:
                  return new GlowValue((long)SwapByteOrder64(BitConverter.ToUInt64(octets, offset)));

               case GlowStreamFormat.UnsignedInt64LittleEndian:
                  return new GlowValue((long)BitConverter.ToUInt64(octets, offset));

               case GlowStreamFormat.SignedInt8:
                  return new GlowValue((long)(sbyte)octets[offset]);

               case GlowStreamFormat.SignedInt16BigEndian:
                  return new GlowValue((long)(short)SwapByteOrder16(BitConverter.ToUInt16(octets, offset)));

               case GlowStreamFormat.SignedInt16LittleEndian:
                  return new GlowValue((long)BitConverter.ToInt16(octets, offset));

               case GlowStreamFormat.SignedInt32BigEndian:
                  return new GlowValue((long)(int)SwapByteOrder32(BitConverter.ToUInt32(octets, offset)));

               case GlowStreamFormat.SignedInt32LittleEndian:
                  return new GlowValue((long)BitConverter.ToInt32(octets, offset));

               case GlowStreamFormat.SignedInt64BigEndian:
                  return new GlowValue((long)SwapByteOrder64(BitConverter.ToUInt64(octets, offset)));

               case GlowStreamFormat.SignedInt64LittleEndian:
                  return new GlowValue((long)BitConverter.ToInt64(octets, offset));

               case GlowStreamFormat.IeeeFloat32BigEndian:
               {
                  var intValue = BitConverter.ToUInt32(octets, offset);
                  return new GlowValue((double)Int32BitsToFloat((int)SwapByteOrder32(intValue)));
               }

               case GlowStreamFormat.IeeeFloat32LittleEndian:
                  return new GlowValue((double)BitConverter.ToSingle(octets, offset));

               case GlowStreamFormat.IeeeFloat64BigEndian:
               {
                  var longValue = BitConverter.ToUInt64(octets, offset);
                  return new GlowValue(BitConverter.Int64BitsToDouble((long)SwapByteOrder64(longValue)));
               }

               case GlowStreamFormat.IeeeFloat64LittleEndian:
                  return new GlowValue(BitConverter.ToDouble(octets, offset));
            }
         }

         return null;
      }
      #endregion

      #region Implementation
      static object s_sync = new object();
      static Dictionary<string, ICompilationResult> s_compilationResults = new Dictionary<string, ICompilationResult>(StringComparer.InvariantCultureIgnoreCase);

      static ulong SwapByteOrder64(ulong value)
      {
         var swapped = (0x00000000000000FFUL) & (value >> 56)
                     | (0x000000000000FF00UL) & (value >> 40)
                     | (0x0000000000FF0000UL) & (value >> 24)
                     | (0x00000000FF000000UL) & (value >> 8)
                     | (0x000000FF00000000UL) & (value << 8)
                     | (0x0000FF0000000000UL) & (value << 24)
                     | (0x00FF000000000000UL) & (value << 40)
                     | (0xFF00000000000000UL) & (value << 56);

         return swapped;
      }

      static uint SwapByteOrder32(uint value)
      {
         var swapped = (0x000000FF) & (value >> 24)
                     | (0x0000FF00) & (value >> 8)
                     | (0x00FF0000) & (value << 8)
                     | (0xFF000000) & (value << 24);

         return swapped;
      }

      static ushort SwapByteOrder16(ushort value)
      {
         var swapped = (0x00FF) & (value >> 8)
                     | (0xFF00) & (value << 8);

         return (ushort)(swapped & 0xFFFF);
      }

      static float Int32BitsToFloat(int value)
      {
         return BitConverter.ToSingle(BitConverter.GetBytes(value), 0);
      }
      #endregion

      #region Nested Types
      class InvalidFormulaCompilationResult : ICompilationResult
      {
         public static readonly InvalidFormulaCompilationResult Instance = new InvalidFormulaCompilationResult();

         // suppress "static ctors should not raise exceptions" because
         // exception is only created here, not raised.
         [SuppressMessage("Microsoft.Design", "CA1065")]
         static InvalidFormulaCompilationResult()
         {
            s_ex = new Exception("Formula not enclosed in parenthesis!");
         }

         #region Implementation
         static Exception s_ex;
         #endregion

         #region ICompilationResult Members
         public Exception Error
         {
            get { return s_ex; }
         }

         public ICompiledFormula Formula
         {
            get { return null; }
         }

         public bool Success
         {
            get { return false; }
         }
         #endregion
      }
      #endregion
   }
}
