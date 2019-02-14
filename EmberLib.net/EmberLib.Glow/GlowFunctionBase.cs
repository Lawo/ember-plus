/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

using System;
using System.Collections.Generic;
using BerLib;

namespace EmberLib.Glow
{
   /// <summary>
   /// Base class for the EmberPlus-Glow.Function and EmberPlus-Glow.QualifiedFunction types.
   /// </summary>
   public abstract class GlowFunctionBase : GlowContentElement
   {
      /// <summary>
      /// Creates a new instance of GlowFunctionBase.
      /// </summary>
      protected GlowFunctionBase(BerTag? tag, uint type)
         : base(tag, type)
      {
      }

      /// <summary>
      /// Gets or sets the "identifier" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string Identifier
      {
         get { return GetContentRef<string>(GlowTags.FunctionContents.Identifier); }
         set
         {
            InternalTools.AssertIdentifierValid(value);

            var tag = GlowTags.FunctionContents.Identifier;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "description" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string Description
      {
         get { return GetContentRef<string>(GlowTags.FunctionContents.Description); }
         set
         {
            var tag = GlowTags.FunctionContents.Description;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "arguments" field.
      /// Getter returns null if field not present.
      /// </summary>
      public EmberSequence Arguments
      {
         get
         {
            var tag = GlowTags.FunctionContents.Arguments;
            var contents = this[ContentsTag] as EmberSet;

            return contents != null
                   ? contents[tag] as EmberSequence
                   : null;
         }
         set
         {
            if (value.Tag != GlowTags.FunctionContents.Arguments)
               throw new ArgumentException("Tag mismatch");

            EnsureContentsAndRemove(value.Tag).Insert(value);
         }
      }

      /// <summary>
      /// Gets an typed enumeration over the TupleItemDescriptions contained in
      /// in the "arguments" field.
      /// Returns null if field "arguments" not present.
      /// </summary>
      public IEnumerable<GlowTupleItemDescription> TypedArguments
      {
         get
         {
            var arguments = Arguments;

            return arguments != null
                   ? InternalTools.EnumerateChildren<GlowTupleItemDescription>(arguments)
                   : null;
         }
      }

      /// <summary>
      /// Ensures that the "arguments" field is present, creating a
      /// new EmberSequence and assigning it to the "arguments" field
      /// if necessary.
      /// </summary>
      /// <returns>The value of the "arguments" field, never null.</returns>
      public EmberSequence EnsureArguments()
      {
         var arguments = Arguments;

         if (arguments == null)
         {
            arguments = new EmberSequence(GlowTags.FunctionContents.Arguments);
            Arguments = arguments;
         }

         return arguments;
      }

      /// <summary>
      /// Gets or sets the "result" field.
      /// Getter returns null if field not present.
      /// </summary>
      public EmberSequence Result
      {
         get
         {
            var tag = GlowTags.FunctionContents.Result;
            var contents = this[ContentsTag] as EmberSet;

            return contents != null
                   ? contents[tag] as EmberSequence
                   : null;
         }
         set
         {
            if (value.Tag != GlowTags.FunctionContents.Result)
               throw new ArgumentException("Tag mismatch");

            EnsureContentsAndRemove(value.Tag).Insert(value);
         }
      }

      /// <summary>
      /// Gets an typed enumeration over the TupleItemDescriptions contained in
      /// in the "result" field.
      /// Returns null if field "result" not present.
      /// </summary>
      public IEnumerable<GlowTupleItemDescription> TypedResult
      {
         get
         {
            var result = Result;

            return result != null
                   ? InternalTools.EnumerateChildren<GlowTupleItemDescription>(result)
                   : null;
         }
      }

      /// <summary>
      /// Gets or sets the OID of the template containing the description
      /// of this element. If not present, <c>null</c> is being returned.
      /// </summary>
      public int[] TemplateReference
      {
         get
         {
            var tag = GlowTags.FunctionContents.TemplateReference;
            return GetContentRef<int[]>(tag);
         }
         set
         {
            var tag = GlowTags.FunctionContents.TemplateReference;

            EnsureContentsAndRemove(tag).Insert(new RelativeOidEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Ensures that the "result" field is present, creating a
      /// new EmberSequence and assigning it to the "result" field
      /// if necessary.
      /// </summary>
      /// <returns>The value of the "result" field, never null.</returns>
      public EmberSequence EnsureResult()
      {
         var result = Result;

         if (result == null)
         {
            result = new EmberSequence(GlowTags.FunctionContents.Result);
            Result = result;
         }

         return result;
      }
   }
}
