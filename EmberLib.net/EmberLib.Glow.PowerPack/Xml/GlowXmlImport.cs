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
using System.Globalization;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using BerLib;

namespace EmberLib.Glow.PowerPack.Xml
{
   /// <summary>
   /// Provides means to create a Glow tree from XML, which must be in
   /// the schema produced by GlowXmlExport.
   /// </summary>
   public sealed class GlowXmlImport
   {
      /// <summary>
      /// Imports XML in the data schema produced by GlowXmlExport.Export.
      /// </summary>
      /// <param name="reader">The XmlReader to read XML from.</param>
      /// <returns>A new instance of a class derived from GlowContainer
      /// or null if the XML root is unknown.</returns>
      public static GlowContainer Import(XmlReader reader)
      {
         var instance = new GlowXmlImport();
         var xml = XElement.Load(reader);

         return instance.Convert(xml);
      }

      #region Implementation
      private GlowXmlImport()
      {
      }

      GlowContainer Convert(XElement xml)
      {
         switch(xml.Name.LocalName)
         {
            case "Root":
               return ConvertRoot(xml);

            case "Command":
               return ConvertCommand(xml);

            case "Node":
               return ConvertNode(xml);

            case "Parameter":
               return ConvertParameter(xml);

            case "QualifiedParameter":
               return ConvertQualifiedParameter(xml);

            case "QualifiedNode":
               return ConvertQualifiedNode(xml);

            case "Matrix":
               return ConvertMatrix(xml);

            case "QualifiedMatrix":
               return ConvertQualifiedMatrix(xml);

            case "Function":
               return ConvertFunction(xml);

            case "QualifiedFunction":
               return ConvertQualifiedFunction(xml);
         }

         return null;
      }

      GlowContainer ConvertRoot(XElement xml)
      {
         switch(xml.Attribute("type").Value)
         {
            case "ElementCollection":
               var glow = GlowRootElementCollection.CreateRoot();
               FillElementCollection(glow, xml);
               return glow;

            case "StreamCollection":
               return ConvertStreamCollection(xml);

            case "InvocationResult":
               return ConvertInvocationResult(xml);
         }

         return null;
      }

      GlowStreamCollection ConvertStreamCollection(XElement xml)
      {
         var glow = GlowStreamCollection.CreateRoot();
         var glowStreamEntries = from xmlChild in xml.Elements("StreamEntry")
                                 select ConvertStreamEntry(xmlChild);

         foreach(var glowStreamEntry in glowStreamEntries)
            glow.Insert(glowStreamEntry);

         return glow;
      }

      GlowStreamEntry ConvertStreamEntry(XElement xml)
      {
         return new GlowStreamEntry
         {
            StreamIdentifier = XmlConvert.ToInt32(xml.Attribute("streamIdentifier").Value),
            StreamValue = ConvertValue(xml.Element("streamValue")),
         };
      }

      GlowValue ConvertValue(XElement xml)
      {
         var type = ConvertParameterType(xml.Attribute("type").Value);

         if(type != null)
         {
            GlowValue value;

            if(GlowValue.TryParse(xml.Value, type.Value, CultureInfo.InvariantCulture, out value))
               return value;
         }

         return null;
      }

      int? ConvertParameterType(string typeStr)
      {
         switch(typeStr)
         {
            case "BOOLEAN":      return GlowParameterType.Boolean;
            case "INTEGER":      return GlowParameterType.Integer;
            case "OCTET STRING": return GlowParameterType.Octets;
            case "REAL":         return GlowParameterType.Real;
            case "UTF8":         return GlowParameterType.String;
         }

         return null;
      }

      GlowMinMax ConvertMinMax(XElement xml)
      {
         var type = ConvertParameterType(xml.Attribute("type").Value);

         if(type == GlowParameterType.Integer
         || type == GlowParameterType.Real)
         {
            GlowMinMax value;

            if(GlowMinMax.TryParse(xml.Value, type.Value, CultureInfo.InvariantCulture, out value))
               return value;
         }

         return null;
      }

      int[] ConvertPath(string pathStr)
      {
         return (from token in pathStr.Split('.')
                 select XmlConvert.ToInt32(token))
                 .ToArray();
      }

      GlowStringIntegerCollection ConvertStringIntegerCollection(BerTag tag, XElement xml)
      {
         var glow = new GlowStringIntegerCollection(tag);
         var glowEntries = from xmlChild in xml.Elements("StringIntegerPair")
                           let name = xmlChild.Attribute("entryString").Value
                           let value = XmlConvert.ToInt32(xmlChild.Attribute("entryInteger").Value)
                           select new GlowStringIntegerPair(GlowTags.StringIntegerCollection.StringIntegerPair, name, value);

         foreach(var glowEntry in glowEntries)
            glow.Insert(glowEntry);

         return glow;
      }

      EmberSequence ConvertTupleDescription(BerTag tag, XElement xml)
      {
         var ember = new EmberSequence(tag);

         foreach(var itemsXml in xml.Elements("TupleItemDescription"))
         {
            var glowItem = new GlowTupleItemDescription(0, GlowTags.CollectionItem);

            var typeXml = itemsXml.Element("type");
            if(typeXml != null)
            {
               var type = ConvertParameterType(typeXml.Value);

               if(type != null)
                  glowItem.Type = type.Value;
            }

            itemsXml.Element("name").Do(value => glowItem.Name = value);

            ember.Insert(glowItem);
         }

         return ember;
      }

      GlowStreamDescription ConvertStreamDescription(BerTag tag, XElement xml)
      {
         return new GlowStreamDescription(
            tag,
            XmlConvert.ToInt32(xml.Element("format").Value),
            XmlConvert.ToInt32(xml.Element("offset").Value));
      }

      GlowCommand ConvertCommand(XElement xml)
      {
         var glow = new GlowCommand(XmlConvert.ToInt32(xml.Attribute("number").Value));
         xml.Element("dirFieldMask").Do(value => glow.DirFieldMask = XmlConvert.ToInt32(value));

         var invocationXml = xml.Element("invocation");
         if(invocationXml != null)
         {
            var glowInvocation = new GlowInvocation(GlowTags.Command.Invocation);
            invocationXml.Element("invocationId").Do(value => glowInvocation.InvocationId = XmlConvert.ToInt32(value));

            var argumentsXml = invocationXml.Element("arguments");
            if(argumentsXml != null)
            {
               glowInvocation.ArgumentValues = from valueXml in argumentsXml.Elements("Value")
                                               let glowValue = ConvertValue(valueXml)
                                               where glowValue != null
                                               select glowValue;
            }

            glow.Invocation = glowInvocation;
         }

         return glow;
      }

      GlowNode ConvertNode(XElement xml)
      {
         var glow = new GlowNode(XmlConvert.ToInt32(xml.Attribute("number").Value));
         FillNode(glow, xml);
         return glow;
      }

      GlowParameter ConvertParameter(XElement xml)
      {
         var glow = new GlowParameter(XmlConvert.ToInt32(xml.Attribute("number").Value));
         FillParameter(glow, xml);
         return glow;
      }

      GlowMatrix ConvertMatrix(XElement xml)
      {
         var glow = new GlowMatrix(XmlConvert.ToInt32(xml.Attribute("number").Value));
         FillMatrix(glow, xml);
         return glow;
      }

      GlowQualifiedNode ConvertQualifiedNode(XElement xml)
      {
         var glow = new GlowQualifiedNode(ConvertPath(xml.Attribute("path").Value));
         FillNode(glow, xml);
         return glow;
      }

      GlowQualifiedParameter ConvertQualifiedParameter(XElement xml)
      {
         var glow = new GlowQualifiedParameter(ConvertPath(xml.Attribute("path").Value));
         FillParameter(glow, xml);
         return glow;
      }

      GlowQualifiedMatrix ConvertQualifiedMatrix(XElement xml)
      {
         var glow = new GlowQualifiedMatrix(ConvertPath(xml.Attribute("path").Value));
         FillMatrix(glow, xml);
         return glow;
      }

      GlowContainer ConvertQualifiedFunction(XElement xml)
      {
         var glow = new GlowQualifiedFunction(ConvertPath(xml.Attribute("path").Value));
         FillFunction(glow, xml);
         return glow;
      }

      GlowContainer ConvertFunction(XElement xml)
      {
         var glow = new GlowFunction(XmlConvert.ToInt32(xml.Attribute("number").Value));
         FillFunction(glow, xml);
         return glow;
      }

      GlowContainer ConvertInvocationResult(XElement xml)
      {
         var glow = GlowInvocationResult.CreateRoot(0);

         glow.InvocationId = XmlConvert.ToInt32(xml.Attribute("invocationId").Value);
         xml.Element("success").Do(value => glow.Success = XmlConvert.ToBoolean(value));

         var resultXml = xml.Element("result");
         if(resultXml != null)
         {
            glow.ResultValues = from valueXml in resultXml.Elements("Value")
                                let glowValue = ConvertValue(valueXml)
                                where glowValue != null
                                select glowValue;
         }

         return glow;
      }

      void FillElementCollection(GlowElementCollectionBase glow, XElement xml)
      {
         var glowElements = from xmlChild in xml.Elements()
                            let glowElement = Convert(xmlChild) as GlowElement
                            where glowElement != null
                            select glowElement;

         foreach(var glowElement in glowElements)
            glow.Insert(glowElement);
      }

      void FillNode(GlowNodeBase glow, XElement xml)
      {
         var contentsXml = xml.Element("contents");

         if(contentsXml != null)
         {
            contentsXml.Element("identifier").Do(value => glow.Identifier = value);
            contentsXml.Element("description").Do(value => glow.Description = value);
            contentsXml.Element("isRoot").Do(value => glow.IsRoot = XmlConvert.ToBoolean(value));
            contentsXml.Element("isOnline").Do(value => glow.IsOnline = XmlConvert.ToBoolean(value));
         }

         var childrenXml = xml.Element("children");

         if(childrenXml != null)
            FillElementCollection(glow.EnsureChildren(), childrenXml);
      }

      void FillParameter(GlowParameterBase glow, XElement xml)
      {
         var contentsXml = xml.Element("contents");

         if(contentsXml != null)
         {
            contentsXml.Element("identifier").Do(value => glow.Identifier = value);
            contentsXml.Element("description").Do(value => glow.Description = value);

            var valueXml = contentsXml.Element("value");
            if(valueXml != null)
               glow.Value = ConvertValue(valueXml);

            var minimumXml = contentsXml.Element("minimum");
            if(minimumXml != null)
               glow.Minimum = ConvertMinMax(minimumXml);

            var maximumXml = contentsXml.Element("maximum");
            if(maximumXml != null)
               glow.Maximum = ConvertMinMax(maximumXml);

            contentsXml.Element("enumeration").Do(value => glow.Enumeration = value);
            contentsXml.Element("factor").Do(value => glow.Factor = XmlConvert.ToInt32(value));
            contentsXml.Element("format").Do(value => glow.Format = value);
            contentsXml.Element("step").Do(value => glow.Step = XmlConvert.ToInt32(value));
            contentsXml.Element("access").Do(value => glow.Access = XmlConvert.ToInt32(value));
            contentsXml.Element("type").Do(value => glow.Type = XmlConvert.ToInt32(value));
            contentsXml.Element("isOnline").Do(value => glow.IsOnline = XmlConvert.ToBoolean(value));
            contentsXml.Element("streamIdentifier").Do(value => glow.StreamIdentifier = XmlConvert.ToInt32(value));

            var enumMapXml = contentsXml.Element("enumMap");
            if(enumMapXml != null)
               glow.EnumMap = ConvertStringIntegerCollection(GlowTags.ParameterContents.EnumMap, enumMapXml);

            var streamDescriptorXml = contentsXml.Element("streamDescriptor");
            if(streamDescriptorXml != null)
               glow.StreamDescriptor = ConvertStreamDescription(GlowTags.ParameterContents.StreamDescriptor, streamDescriptorXml);
         }

         var childrenXml = xml.Element("children");

         if(childrenXml != null)
            FillElementCollection(glow.EnsureChildren(), childrenXml);
      }

      void FillMatrix(GlowMatrixBase glow, XElement xml)
      {
         var contentsXml = xml.Element("contents");

         if(contentsXml != null)
         {
            contentsXml.Element("identifier").Do(value => glow.Identifier = value);
            contentsXml.Element("description").Do(value => glow.Description = value);
            contentsXml.Element("type").Do(value => glow.MatrixType = XmlConvert.ToInt32(value));
            contentsXml.Element("addressingMode").Do(value => glow.AddressingMode = XmlConvert.ToInt32(value));
            contentsXml.Element("targetCount").Do(value => glow.TargetCount = XmlConvert.ToInt32(value));
            contentsXml.Element("sourceCount").Do(value => glow.SourceCount = XmlConvert.ToInt32(value));
            contentsXml.Element("maximumTotalConnects").Do(value => glow.MaximumTotalConnects = XmlConvert.ToInt32(value));
            contentsXml.Element("maximumConnectsPerTarget").Do(value => glow.MaximumConnectsPerTarget = XmlConvert.ToInt32(value));

            var parametersLocationXml = contentsXml.Element("parametersLocation");
            if(parametersLocationXml != null)
            {
               var attrib = parametersLocationXml.Attribute("type");

               if(attrib.Value == "RELATIVE-OID")
               {
                  glow.ParametersLocation = new GlowParametersLocation(ConvertPath(parametersLocationXml.Value));
               }
               else if(attrib.Value == "INTEGER")
               {
                  glow.ParametersLocation = new GlowParametersLocation(XmlConvert.ToInt32(parametersLocationXml.Value));
               }
            }

            contentsXml.Element("gainParameterNumber").Do(value => glow.GainParameterNumber = XmlConvert.ToInt32(value));

            var labelsXml = contentsXml.Element("labels");
            if(labelsXml != null)
            {
               var glowLabels = from xmlChild in labelsXml.Elements("Label")
                                select new GlowLabel
                                {
                                   BasePath = ConvertPath(xmlChild.Attribute("basePath").Value),
                                   Description = xmlChild.Attribute("description").Value,
                                };

               var glowLabelsCollection = new EmberSequence(GlowTags.MatrixContents.Labels);

               foreach(var glowLabel in glowLabels)
                  glowLabelsCollection.Insert(glowLabel);

               glow.Labels = glowLabelsCollection;
            }
         }

         var childrenXml = xml.Element("children");
         if(childrenXml != null)
            FillElementCollection(glow.EnsureChildren(), childrenXml);

         var targetsXml = xml.Element("targets");
         if(targetsXml != null)
         {
            var glowTargets = from xmlChild in targetsXml.Elements("Target")
                              select new GlowTarget(XmlConvert.ToInt32(xmlChild.Attribute("number").Value));
            var collection = glow.EnsureTargets();

            foreach(var glowTarget in glowTargets)
               collection.Insert(glowTarget);
         }

         var sourcesXml = xml.Element("sources");
         if(sourcesXml != null)
         {
            var glowSources = from xmlChild in sourcesXml.Elements("Source")
                              select new GlowSource(XmlConvert.ToInt32(xmlChild.Attribute("number").Value));
            var collection = glow.EnsureSources();

            foreach(var glowSource in glowSources)
               collection.Insert(glowSource);
         }

         var connectionsXml = xml.Element("connections");
         if(connectionsXml != null)
         {
            var collection = glow.EnsureConnections();

            foreach(var xmlChild in connectionsXml.Elements("Connection"))
            {
               var glowConnection = new GlowConnection(XmlConvert.ToInt32(xmlChild.Attribute("target").Value));
               xmlChild.Element("sources").Do(value => glowConnection.Sources = ConvertPath(value));
               xmlChild.Element("operation").Do(value => glowConnection.Operation = XmlConvert.ToInt32(value));
               xmlChild.Element("disposition").Do(value => glowConnection.Disposition = XmlConvert.ToInt32(value));
               collection.Insert(glowConnection);
            }
         }
      }

      void FillFunction(GlowFunctionBase glow, XElement xml)
      {
         var contentsXml = xml.Element("contents");

         if(contentsXml != null)
         {
            contentsXml.Element("identifier").Do(value => glow.Identifier = value);
            contentsXml.Element("description").Do(value => glow.Description = value);

            var argumentsXml = contentsXml.Element("arguments");
            if(argumentsXml != null)
               glow.Arguments = ConvertTupleDescription(GlowTags.FunctionContents.Arguments, argumentsXml);

            var resultXml = contentsXml.Element("result");
            if(resultXml != null)
               glow.Result = ConvertTupleDescription(GlowTags.FunctionContents.Result, resultXml);
         }

         var childrenXml = xml.Element("children");
         if(childrenXml != null)
            FillElementCollection(glow.EnsureChildren(), childrenXml);
      }
      #endregion
   }
}
