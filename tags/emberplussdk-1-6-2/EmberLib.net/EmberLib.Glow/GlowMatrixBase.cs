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

namespace EmberLib.Glow
{
   /// <summary>
   /// Base class for the EmberPlus-Glow.Matrix and EmberPlus-Glow.QualifiedMatrix types.
   /// </summary>
   public abstract class GlowMatrixBase : GlowContentElement
   {
      /// <summary>
      /// Creates a new instance of GlowMatrix.
      /// </summary>
      /// <param name="tag">Either a specific tag or null when the node
      /// is to be inserted into a GlowElementCollection. The tag will be
      /// set to GlowTags.CollectionItem if the passed tag is null.</param>
      /// <param name="type">ber type of the represented glow type</param>
      protected GlowMatrixBase(BerTag? tag, uint type)
      : base(tag, type)
      {
      }

      /// <summary>
      /// Gets or sets the "targets" field of the matrix.
      /// Getter returns null if field not present.
      /// </summary>
      public EmberSequence Targets
      {
         get
         {
            var tag = TargetsTag;

            return this[tag] as EmberSequence;
         }
         set
         {
            var tag = TargetsTag;

            if(value.Tag != tag)
               throw new ArgumentException("Tag mismatch");

            Remove(tag);
            Insert(value);
         }
      }

      /// <summary>
      /// Gets an typed enumeration over the GlowTarget objects contained
      /// in the "targets" field.
      /// Returns null if field "targets" not present.
      /// </summary>
      public IEnumerable<GlowTarget> TypedTargets
      {
         get
         {
            var targets = Targets;

            return targets != null
                   ? InternalTools.EnumerateChildren<GlowTarget>(targets)
                   : null;
         }
      }

      /// <summary>
      /// Ensures that the "targets" field is present, creating a
      /// new EmberSequence and assigning it to the "targets" field
      /// if necessary.
      /// </summary>
      /// <returns>The value of the "targets" field, never null.</returns>
      public EmberSequence EnsureTargets()
      {
         var targets = Targets;

         if(targets == null)
         {
            targets = new EmberSequence(TargetsTag);
            Insert(targets);
         }

         return targets;
      }

      /// <summary>
      /// Gets or sets the "sources" field of the matrix.
      /// Getter returns null if field not present.
      /// </summary>
      public EmberSequence Sources
      {
         get
         {
            var tag = SourcesTag;

            return this[tag] as EmberSequence;
         }
         set
         {
            var tag = SourcesTag;

            if(value.Tag != tag)
               throw new ArgumentException("Tag mismatch");

            Remove(tag);
            Insert(value);
         }
      }

      /// <summary>
      /// Gets an typed enumeration over the GlowSource objects contained
      /// in the "sources" field.
      /// Returns null if field "sources" not present.
      /// </summary>
      public IEnumerable<GlowSource> TypedSources
      {
         get
         {
            var sources = Sources;

            return sources != null
                   ? InternalTools.EnumerateChildren<GlowSource>(sources)
                   : null;
         }
      }

      /// <summary>
      /// Ensures that the "sources" field is present, creating a
      /// new EmberSequence and assigning it to the "sources" field
      /// if necessary.
      /// </summary>
      /// <returns>The value of the "sources" field, never null.</returns>
      public EmberSequence EnsureSources()
      {
         var sources = Sources;

         if(sources == null)
         {
            sources = new EmberSequence(SourcesTag);
            Insert(sources);
         }

         return sources;
      }

      /// <summary>
      /// Gets or sets the "connections" field of the matrix.
      /// Getter returns null if field not present.
      /// </summary>
      public EmberSequence Connections
      {
         get
         {
            var tag = ConnectionsTag;

            return this[tag] as EmberSequence;
         }
         set
         {
            var tag = ConnectionsTag;

            if(value.Tag != tag)
               throw new ArgumentException("Tag mismatch");

            Remove(tag);
            Insert(value);
         }
      }

      /// <summary>
      /// Gets an typed enumeration over the GlowConnection objects contained
      /// in the "connections" field.
      /// Returns null if field "connections" not present.
      /// </summary>
      public IEnumerable<GlowConnection> TypedConnections
      {
         get
         {
            var connections = Connections;

            return connections != null
                   ? InternalTools.EnumerateChildren<GlowConnection>(connections)
                   : null;
         }
      }

      /// <summary>
      /// Ensures that the "connections" field is present, creating a
      /// new EmberSequence and assigning it to the "connections" field
      /// if necessary.
      /// </summary>
      /// <returns>The value of the "connections" field, never null.</returns>
      public EmberSequence EnsureConnections()
      {
         var connections = Connections;

         if(connections == null)
         {
            connections = new EmberSequence(ConnectionsTag);
            Insert(connections);
         }

         return connections;
      }

      /// <summary>
      /// Gets or sets the "identifier" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string Identifier
      {
         get { return GetContentRef<string>(GlowTags.MatrixContents.Identifier); }
         set
         {
            InternalTools.AssertIdentifierValid(value);

            var tag = GlowTags.MatrixContents.Identifier;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "description" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string Description
      {
         get { return GetContentRef<string>(GlowTags.MatrixContents.Description); }
         set
         {
            var tag = GlowTags.MatrixContents.Description;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Gets or sets the "type" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public int? MatrixType
      {
         get { return GetContentValue<int>(GlowTags.MatrixContents.Type); }
         set
         {
            var tag = GlowTags.MatrixContents.Type;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "addressingMode" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public int? AddressingMode
      {
         get { return GetContentValue<int>(GlowTags.MatrixContents.AddressingMode); }
         set
         {
            var tag = GlowTags.MatrixContents.AddressingMode;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "targetCount" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public int? TargetCount
      {
         get { return GetContentValue<int>(GlowTags.MatrixContents.TargetCount); }
         set
         {
            var tag = GlowTags.MatrixContents.TargetCount;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "sourceCount" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public int? SourceCount
      {
         get { return GetContentValue<int>(GlowTags.MatrixContents.SourceCount); }
         set
         {
            var tag = GlowTags.MatrixContents.SourceCount;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "maximumTotalConnects" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public int? MaximumTotalConnects
      {
         get { return GetContentValue<int>(GlowTags.MatrixContents.MaximumTotalConnects); }
         set
         {
            var tag = GlowTags.MatrixContents.MaximumTotalConnects;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "maximumConnectsPerTarget" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public int? MaximumConnectsPerTarget
      {
         get { return GetContentValue<int>(GlowTags.MatrixContents.MaximumConnectsPerTarget); }
         set
         {
            var tag = GlowTags.MatrixContents.MaximumConnectsPerTarget;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "parametersBasePath" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public GlowParametersLocation ParametersLocation
      {
         get
         {
            var tag = GlowTags.MatrixContents.ParametersLocation;
            var basePath = GetContentRef<int[]>(tag);

            if(basePath != null)
               return new GlowParametersLocation(basePath);

            var inline = GetContentValue<int>(tag);

            if(inline != null)
               return new GlowParametersLocation(inline.Value);

            return null;
         }
         set
         {
            var tag = GlowTags.MatrixContents.ParametersLocation;
            var contents = EnsureContentsAndRemove(tag);

            if(value.Kind == GlowParametersLocationKind.BasePath)
            {
               contents.Insert(new RelativeOidEmberLeaf(tag, value.BasePath));
            }
            else if(value.Kind == GlowParametersLocationKind.Inline)
            {
               contents.Insert(new IntegerEmberLeaf(tag, value.Inline));
            }
         }
      }

      /// <summary>
      /// Gets or sets the "gainParameterNumber" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public int? GainParameterNumber
      {
         get { return GetContentValue<int>(GlowTags.MatrixContents.GainParameterNumber); }
         set
         {
            var tag = GlowTags.MatrixContents.GainParameterNumber;

            EnsureContentsAndRemove(tag).Insert(new IntegerEmberLeaf(tag, value.Value));
         }
      }

      /// <summary>
      /// Gets or sets the "labels" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public EmberSequence Labels
      {
         get
         {
            var tag = GlowTags.MatrixContents.Labels;
            var contents = this[ContentsTag] as EmberSet;

            return contents != null
                   ? contents[tag] as EmberSequence
                   : null;
         }
         set
         {
            if(value.Tag != GlowTags.MatrixContents.Labels)
               throw new ArgumentException("Tag mismatch");

            EnsureContentsAndRemove(value.Tag).Insert(value);
         }
      }

      /// <summary>
      /// Gets an typed enumeration over the GlowLabel objects contained
      /// in the "contents.labels" field.
      /// Returns null if field "contents.labels" not present.
      /// </summary>
      public IEnumerable<GlowLabel> TypedLabels
      {
         get
         {
            var labels = Labels;

            return labels != null
                   ? InternalTools.EnumerateChildren<GlowLabel>(labels)
                   : null;
         }
      }

      /// <summary>
      /// Ensures that the "contents.labels" field is present, creating a
      /// new EmberSequence and assigning it to the "contents.labels" field
      /// if necessary.
      /// </summary>
      /// <returns>The value of the "contents.labels" field, never null.</returns>
      public EmberSequence EnsureLabels()
      {
         var labels = Labels;

         if(labels == null)
         {
            labels = new EmberSequence(GlowTags.MatrixContents.Labels);
            EnsureContents().Insert(labels);
         }

         return labels;
      }

      /// <summary>
      /// Gets or sets the "schemaIdentifiers" field of the "contents" SET.
      /// Getter returns null if field not present.
      /// </summary>
      public string SchemaIdentifiers
      {
         get { return GetContentRef<string>(GlowTags.MatrixContents.SchemaIdentifiers); }
         set
         {
            var tag = GlowTags.MatrixContents.SchemaIdentifiers;

            EnsureContentsAndRemove(tag).Insert(new StringEmberLeaf(tag, value));
         }
      }

      /// <summary>
      /// Tests if the matrix complies with the schema with the passed schema identifier.
      /// </summary>
      /// <param name="schemaIdentifier">The identifier of the schema to test for.</param>
      /// <returns>True if the matrix complies with the specified schema, otherwise false.</returns>
      public bool CompliesWithSchema(string schemaIdentifier)
      {
         var schemaIdentifiers = SchemaIdentifiers;

         return schemaIdentifiers != null
                ? InternalTools.CompliesWithSchema(schemaIdentifiers, schemaIdentifier)
                : false;
      }

      /// <summary>
      /// Override to return the tag for the "targets" field.
      /// </summary>
      internal abstract BerTag TargetsTag
      {
         get;
      }

      /// <summary>
      /// Override to return the tag for the "sources" field.
      /// </summary>
      internal abstract BerTag SourcesTag
      {
         get;
      }

      /// <summary>
      /// Override to return the tag for the "connections" field.
      /// </summary>
      internal abstract BerTag ConnectionsTag
      {
         get;
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
