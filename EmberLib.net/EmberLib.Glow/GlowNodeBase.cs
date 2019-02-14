/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

using System;
using BerLib;

namespace EmberLib.Glow
{
   /// <summary>
   /// Base class for the EmberPlus-Glow.Node and EmberPlus-Glow.QualifiedNode types.
   /// </summary>
   public abstract class GlowNodeBase : GlowContentElement
   {
      /// <summary>
      /// Creates a new instance of GlowNodeBase.
      /// </summary>
      protected GlowNodeBase(BerTag? tag, uint type)
      : base(tag, type)
      {
      }

      /// <summary>
      /// Gets or sets the "identifier" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string Identifier
      {
         get { return GetContentRef<string>(GlowTags.NodeContents.Identifier); }
         set
         {
            InternalTools.AssertIdentifierValid(value);

            var tag = GlowTags.NodeContents.Identifier;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "description" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string Description
      {
         get { return GetContentRef<string>(GlowTags.NodeContents.Description); }
         set
         {
            var tag = GlowTags.NodeContents.Description;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "isRoot" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public bool? IsRoot
      {
         get { return GetContentValue<bool>(GlowTags.NodeContents.IsRoot); }
         set
         {
            var tag = GlowTags.NodeContents.IsRoot;

            EnsureContentsAndRemove(tag).Insert(new BooleanEmberLeaf(tag, value == true));
         }
      }

      /// <summary>
      /// Gets or sets the "isOnline" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public bool? IsOnline
      {
         get { return GetContentValue<bool>(GlowTags.NodeContents.IsOnline); }
         set
         {
            var tag = GlowTags.NodeContents.IsOnline;

            EnsureContentsAndRemove(tag).Insert(new BooleanEmberLeaf(tag, value == true));
         }
      }

      /// <summary>
      /// Gets or sets the "schemaIdentifiers" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string SchemaIdentifiers
      {
         get { return GetContentRef<string>(GlowTags.NodeContents.SchemaIdentifiers); }
         set
         {
            var tag = GlowTags.NodeContents.SchemaIdentifiers;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
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
            var tag = GlowTags.NodeContents.TemplateReference;
            return GetContentRef<int[]>(tag);
         }
         set
         {
            var tag = GlowTags.NodeContents.TemplateReference;

            EnsureContentsAndRemove(tag).Insert(new RelativeOidEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Tests if the node complies with the schema with the passed schema identifier.
      /// </summary>
      /// <param name="schemaIdentifier">The identifier of the schema to test for.</param>
      /// <returns>True if the node complies with the specified schema, otherwise false.</returns>
      public bool CompliesWithSchema(string schemaIdentifier)
      {
         var schemaIdentifiers = SchemaIdentifiers;

         return schemaIdentifiers != null
                ? InternalTools.CompliesWithSchema(schemaIdentifiers, schemaIdentifier)
                : false;
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
   }
}
