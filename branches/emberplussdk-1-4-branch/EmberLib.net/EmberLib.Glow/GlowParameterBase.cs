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
using BerLib;

namespace EmberLib.Glow
{
   /// <summary>
   /// Base class for the EmberPlus-Glow.Parameter and EmberPlus-Glow.QualifiedParameter types.
   /// </summary>
   public abstract class GlowParameterBase : GlowContentElement
   {
      /// <summary>
      /// Creates a new instance of GlowParameterBase.
      /// </summary>
      protected GlowParameterBase(BerTag? tag, uint type)
      : base(tag, type)
      {
      }

      /// <summary>
      /// Gets or sets the "identifier" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string Identifier
      {
         get { return GetContentRef<string>(GlowTags.ParameterContents.Identifier); }
         set
         {
            InternalTools.AssertIdentifierValid(value);

            var tag = GlowTags.ParameterContents.Identifier;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "description" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string Description
      {
         get { return GetContentRef<string>(GlowTags.ParameterContents.Description); }
         set
         {
            var tag = GlowTags.ParameterContents.Description;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "value" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public GlowValue Value
      {
         get { return GetValue(GlowTags.ParameterContents.Value); }
         set
         {
            InsertValue(GlowTags.ParameterContents.Value, value);
         }
      }

      /// <summary>
      /// Gets or sets the "minimum" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public GlowMinMax Minimum
      {
         get { return GetMinMax(GlowTags.ParameterContents.Minimum); }
         set
         {
            InsertMinMax(GlowTags.ParameterContents.Minimum, value);
         }
      }

      /// <summary>
      /// Gets or sets the "maximum" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public GlowMinMax Maximum
      {
         get { return GetMinMax(GlowTags.ParameterContents.Maximum); }
         set
         {
            InsertMinMax(GlowTags.ParameterContents.Maximum, value);
         }
      }

      /// <summary>
      /// Gets or sets the "access" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// If field not present, the parameter has access "read".
      /// </summary>
      public int? Access
      {
         get
         {
            var access = GetContentValue<int>(GlowTags.ParameterContents.Access);
            
            if(access == null)
            {
               // in glow version prior to 2.5, C-5 was a boolean value field "isWriteable".
               // in glow versions 2.5+ this translates to an access of value "ReadWrite".
               var isWriteable = GetContentValue<bool>(GlowTags.ParameterContents.Access);

               if(isWriteable == true)
                  return GlowAccess.ReadWrite;
            }

            return access;
         }
         set
         {
            var tag = GlowTags.ParameterContents.Access;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "format" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string Format
      {
         get { return GetContentRef<string>(GlowTags.ParameterContents.Format); }
         set
         {
            var tag = GlowTags.ParameterContents.Format;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "enumeration" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// The value of this property consists of newline-separated tokens.
      /// Each token defines the name of its zero-based index.
      /// E.g. "On\nOff" -> "On=0, Off=1".
      /// </summary>
      /// <remarks>
      /// Only use this property to define zero-based, linear enumerations
      /// (e.g. "off=0, on=1, unavailable=2").
      /// Use the EnumMap property (introduced in GlowDTD 2.2) for non-linear
      /// enumerations.
      /// To determine wether the parameter defines an enumeration,
      /// use the HasEnumeration property.
      /// To get enumeration name/value pairs, use the GetEnumeration
      /// method.
      /// </remarks>
      public string Enumeration
      {
         get { return GetContentRef<string>(GlowTags.ParameterContents.Enumeration); }
         set
         {
            var tag = GlowTags.ParameterContents.Enumeration;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "factor" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public int? Factor
      {
         get { return GetContentValue<int>(GlowTags.ParameterContents.Factor); }
         set
         {
            var tag = GlowTags.ParameterContents.Factor;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "isOnline" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public bool? IsOnline
      {
         get { return GetContentValue<bool>(GlowTags.ParameterContents.IsOnline); }
         set
         {
            var tag = GlowTags.ParameterContents.IsOnline;

            EnsureContentsAndRemove(tag).Insert(new BooleanEmberLeaf(tag, value == true));
         }
      }

      /// <summary>
      /// Gets or sets the "step" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public int? Step
      {
         get { return GetContentValue<int>(GlowTags.ParameterContents.Step); }
         set
         {
            var tag = GlowTags.ParameterContents.Step;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "default" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public GlowValue Default
      {
         get { return GetValue(GlowTags.ParameterContents.Default); }
         set
         {
            InsertValue(GlowTags.ParameterContents.Default, value);
         }
      }

      /// <summary>
      /// Gets or sets the "type" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      /// <remarks>
      /// Use the property EffectiveParameterType to determine the actual parameter type.
      /// </remarks>
      public int? Type
      {
         get
         {
            var type = GetContentValue<int>(GlowTags.ParameterContents.Type);

            if(type == null)
            {
               // in glow version prior to 2.5, C-13 was a boolean value field "isCommand".
               // in glow versions 2.5+ this translates to a parameter of type "trigger".
               var isCommand = GetContentValue<bool>(GlowTags.ParameterContents.Type);

               if(isCommand == true)
                  return GlowParameterType.Trigger;
            }

            return type;
         }
         set
         {
            var tag = GlowTags.ParameterContents.Type;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "streamIdentifier" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public int? StreamIdentifier
      {
         get { return GetContentValue<int>(GlowTags.ParameterContents.StreamIdentifier); }
         set
         {
            var tag = GlowTags.ParameterContents.StreamIdentifier;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "enumMap" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      /// <remarks>
      /// Use this property to define non-linear or non-zero based
      /// enumerations (e.g. "error=-1, stopped=0, running=255").
      /// To determine wether the parameter defines an enumeration,
      /// use the HasEnumeration property.
      /// To get enumeration name/value pairs, use the GetEnumeration
      /// method.
      /// </remarks>
      public GlowStringIntegerCollection EnumMap
      {
         get
         {
            var tag = GlowTags.ParameterContents.EnumMap;
            var contents = EnsureContents();

            return contents != null
                   ? contents[tag] as GlowStringIntegerCollection
                   : null;
         }
         set
         {
            if(value.Tag != GlowTags.ParameterContents.EnumMap)
               throw new ArgumentException("Tag mismatch");

            EnsureContentsAndRemove(value.Tag).Insert(value);
         }
      }

      /// <summary>
      /// Gets or sets the "streamDescriptor" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public GlowStreamDescription StreamDescriptor
      {
         get
         {
            var tag = GlowTags.ParameterContents.StreamDescriptor;
            var contents = EnsureContents();

            return contents != null
                   ? contents[tag] as GlowStreamDescription
                   : null;
         }
         set
         {
            if(value.Tag != GlowTags.ParameterContents.StreamDescriptor
            || value.BerTypeNumber != GlowType.StreamDescription)
               throw new ArgumentException("Tag or type mismatch");

            EnsureContentsAndRemove(value.Tag).Insert(value);
         }
      }

      /// <summary>
      /// Gets a value indicating wether the parameter defines an enumeration.
      /// It returns true if either the enumMap or the enumeration property
      /// is present.
      /// </summary>
      public bool HasEnumeration
      {
         get
         {
            return EnumMap != null || Enumeration != null;
         }
      }

      /// <summary>
      /// Gets a value indicating wether the parameter can be accessed or not.
      /// Depends on the "access" field of GlowParameter.
      /// </summary>
      public bool IsAccessible
      {
         get
         {
            return Access != GlowAccess.None;
         }
      }

      /// <summary>
      /// Gets a value indicating wether the parameter can be written or not.
      /// Depends on the "access" field of GlowParameter.
      /// </summary>
      public bool IsWriteable
      {
         get
         {
            var access = Access;

            return (access & GlowAccess.Write) == GlowAccess.Write;
         }
      }

      /// <summary>
      /// Gets the effective type of the parameter. Returns one of the
      /// constants defined in the GlowParameterType class.
      /// Use this property to determine the actual parameter type.
      /// </summary>
      public int? EffectiveParameterType
      {
         get
         {
            var type = Type;

            if(type == GlowParameterType.Trigger)
               return GlowParameterType.Trigger;

            if(HasEnumeration)
               return GlowParameterType.Enum;

            var value = Value;

            if(value != null)
               return value.Type;

            return type;
         }
      }

      /// <summary>
      /// Yield a collection of name/value pairs that represent
      /// an enumeration.
      /// It takes into account both the enumMap and the enumeration
      /// property, with the enumMap property taking precedence over
      /// the enumeration property.
      /// </summary>
      /// <returns>A lazy collection of name/value pairs which is
      /// empty if the parameter does not define an enumeration.</returns>
      public IEnumerable<KeyValuePair<string, int>> GetEnumeration()
      {
         var enumMap = EnumMap;

         if(enumMap != null)
         {
            foreach(var entry in enumMap.EnumEntries)
               yield return new KeyValuePair<string, int>(entry.EntryName, entry.EntryInteger);
         }
         else
         {
            var enumeration = Enumeration;

            if(enumeration != null)
            {
               var tokens = enumeration.Split('\n');

               for(int index = 0; index < tokens.Length; index++)
               {
                  var token = tokens[index];

                  if(token != "~")
                     yield return new KeyValuePair<string, int>(token, index);
               }
            }
         }
      }

      /// <summary>
      /// Gets the value of the "formula" field of the "contents" SET.
      /// </summary>
      /// <returns>A new formula source tuple.</returns>
      public GlowFormulaSource GetFormula()
      {
         var source = Formula;

         if(String.IsNullOrEmpty(source) == false)
         {
            var tokens = source.Split('\n');

            return new GlowFormulaSource(tokens[0], tokens.Length > 1 ? tokens[1] : null);
         }

         return null;
      }

      /// <summary>
      /// Sets the value of the "formula" field of the "contents" SET.
      /// </summary>
      /// <param name="formula">The formula source tuple to set.</param>
      public void SetFormula(GlowFormulaSource formula)
      {
         if(formula == null)
            throw new ArgumentNullException("formula");

         var strings = new[]
         {
            String.Empty,
            String.Empty,
         };

         if(formula.ProviderToConsumer != null)
            strings[0] = formula.ProviderToConsumer;

         if(formula.ConsumerToProvider != null)
            strings[1] = formula.ConsumerToProvider;

         Formula = String.Join("\n", strings);
      }

      /// <summary>
      /// Gets or sets the "formula" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      /// <remarks>
      /// Use the GetFormula() and SetFormula() methods for formula access.
      /// </remarks>
      protected internal string Formula
      {
         get { return GetContentRef<string>(GlowTags.ParameterContents.Formula); }
         set
         {
            var tag = GlowTags.ParameterContents.Formula;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Override this in application-defined node types to provide validation.
      /// Called when a node has been decoded.
      /// Overriden to validate Identifier.
      /// </summary>
      /// <param name="message">When returning false, set this to a string describing the kind of error.</param>
      /// <returns>False if validation failed, otherwise true.</returns>
      protected override bool ValidateAfterDecodeOverride(out string message)
      {
         var identifier = Identifier;

         if(identifier != null)
         {
            if(InternalTools.IsIdentifierValid(Identifier) == false)
            {
               HasValidationErrors = true;
               message = String.Format("Invalid Identifier \"{0}\"", identifier);
               return false;
            }
         }

         message = null;
         return true;
      }

      #region Implementation
      GlowValue GetValue(BerTag tag)
      {
         var contents = EnsureContents();

         if(contents != null)
            return InternalTools.GetValue(contents, tag);

         return null;
      }

      void InsertValue(BerTag tag, GlowValue value)
      {
         InternalTools.InsertValue(EnsureContentsAndRemove(tag), tag, value);
      }

      GlowMinMax GetMinMax(BerTag tag)
      {
         var contents = EnsureContents();

         if(contents != null)
            return InternalTools.GetMinMax(contents, tag);

         return null;
      }

      void InsertMinMax(BerTag tag, GlowMinMax value)
      {
         InternalTools.InsertMinMax(EnsureContentsAndRemove(tag), tag, value);
      }
      #endregion
   }


   /// <summary>
   /// Describes a formula source code (as used by the "formula" field of EmberPlus-Glow.Parameter).
   /// </summary>
   public class GlowFormulaSource
   {
      /// <summary>
      /// Creates a new instance of GlowFormulaSource.
      /// </summary>
      /// <param name="providerToConsumer">The source of the formula used by the consumer
      /// to convert values received from the provider.</param>
      /// <param name="consumerToProvider">The source of the formula used by the consumer
      /// to convert values to be sent to the provider.</param>
      public GlowFormulaSource(string providerToConsumer, string consumerToProvider)
      {
         ProviderToConsumer = providerToConsumer;
         ConsumerToProvider = consumerToProvider;
      }

      /// <summary>
      /// Gets he source of the formula used by the consumer
      /// to convert values received from the provider.
      /// </summary>
      public string ProviderToConsumer { get; private set; }

      /// <summary>
      /// Gets the source of the formula used by the consumer
      /// to convert values to be sent to the provider.
      /// </summary>
      public string ConsumerToProvider { get; private set; }
   }
}
