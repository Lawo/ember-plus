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

namespace EmberLib.Glow.PowerPack
{
   /// <summary>
   /// Provides value conversion for values coming from the provider and converted by
   /// the consumer.
   /// </summary>
   public sealed class ProviderToConsumerConversion
   {
      /// <summary>
      /// Ctor is internal because this is a singleton object.
      /// Only instance resides in static class ValueConversion.
      /// </summary>
      internal ProviderToConsumerConversion()
      {
      }

      /// <summary>
      /// Converts a value received from the provider.
      /// </summary>
      /// <param name="value">The value to convert</param>
      /// <param name="format">The stream descriptor format as defined by the "format" field of
      /// EmberPlus-Glow.StreamDescription. Only applies if value was received through octet
      /// string streaming. Pass null if not present.</param>
      /// <param name="offset">The stream descriptor offset as defined by the "offset" field of
      /// EmberPlus-Glow.StreamDescription. Only applies if value was received through octet
      /// string streaming. Pass null if not present.</param>
      /// <param name="formula">The formula string as returned by the ProviderToConsumer
      /// property of class EmberLib.Glow.GlowFormulaSource. Pass null if not present.</param>
      /// <param name="factor">A factor used to convert integer value into fixed-point
      /// real values as defined by the "factor" field of EmberPlus-Glow.ParameterContents.</param>
      /// <param name="log">A callback function invoked to log formula compilation errors.</param>
      /// <returns>The converted value.</returns>
      /// <remarks>This method caches formula compilation results in a global cache. This method is thread-safe.</remarks>
      public GlowValue Convert(GlowValue value, int? format, int? offset, string formula, int? factor, Action<string> log = null)
      {
         if(format != null
         && offset != null
         && value.Type == GlowParameterType.Octets)
            value = ValueConversion.ValueFromOctets(value.Octets, format.Value, offset.Value);

         if(String.IsNullOrEmpty(formula) == false)
         {
            if(value.Type == GlowParameterType.Integer
            || value.Type == GlowParameterType.Real)
            {
               var result = ValueConversion.CompileFormula(formula, log);

               if(result.Success)
               {
                  if(value.Type == GlowParameterType.Integer)
                     value = result.Formula.Eval(value.Integer);
                  else
                     value = result.Formula.Eval(value.Real);
               }
            }
         }
         else
         {
            if(factor != null
            && value.Type == GlowParameterType.Integer)
               value = new GlowValue(value.Integer / (double)factor.Value);
         }

         return value;
      }

      /// <summary>
      /// Converts a minimum or maximum value received from the provider.
      /// </summary>
      /// <param name="value">The value to convert</param>
      /// <param name="formula">The formula string as returned by the ProviderToConsumer
      /// property of class EmberLib.Glow.GlowFormulaSource. Pass null if not present.</param>
      /// <param name="factor">A factor used to convert integer value into fixed-point
      /// real values as defined by the "factor" field of EmberPlus-Glow.ParameterContents.</param>
      /// <param name="log">A callback function invoked to log formula compilation errors.</param>
      /// <returns>The converted value.</returns>
      /// <remarks>This method caches formula compilation results in a global cache. This method is thread-safe.</remarks>
      public GlowMinMax Convert(GlowMinMax value, string formula, int? factor, Action<string> log = null)
      {
         if(String.IsNullOrEmpty(formula) == false)
         {
            if(value.Type == GlowParameterType.Integer
            || value.Type == GlowParameterType.Real)
            {
               var result = ValueConversion.CompileFormula(formula, log);

               if(result.Success)
               {
                  if(value.Type == GlowParameterType.Integer)
                     value = result.Formula.Eval(value.Integer);
                  else
                     value = result.Formula.Eval(value.Real);
               }
            }
         }
         else
         {
            if(factor != null
            && value.Type == GlowParameterType.Integer)
               value = new GlowMinMax(value.Integer / (double)factor.Value);
         }

         return value;
      }
   }
}
