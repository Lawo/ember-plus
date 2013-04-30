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
using System.Xml;
using BerLib;
using System.Globalization;

namespace EmberLib.Xml
{
   /// <summary>
   /// An IEmberVisitor implementation to convert an Ember DOM tree to XML.
   /// </summary>
   public class XmlExport : IEmberVisitor<XmlExportState, object>
   {
      internal static readonly IFormatProvider FormatProvider = CultureInfo.InvariantCulture;

      /// <summary>
      /// Converts <paramref name="node"/> to XML and writes the XML to <paramref name="writer"/>.
      /// </summary>
      public static void Export(EmberNode node, XmlWriter writer)
      {
         var export = new XmlExport();
         var state = new XmlExportState(writer);

         node.Accept(export, state);

         writer.WriteWhitespace(Environment.NewLine);
      }

      #region Implementation
      object WriteContainer(EmberContainer node, XmlExportState state)
      {
         var writer = state.Writer;

         writer.WriteStartElement(node.Tag.ToString());

         writer.WriteStartAttribute("type");
         writer.WriteString(BerDefinitions.GetTypeName(node.BerTypeNumber));
         writer.WriteEndAttribute();

         if(node.Count > 0)
         {
            foreach(var child in node)
               child.Accept(this, state);
         }

         writer.WriteEndElement();
         return null;
      }

      object WriteLeaf<TValue>(EmberLeaf<TValue> node, XmlExportState state, string valueStr)
      {
         var writer = state.Writer;

         writer.WriteStartElement(node.Tag.ToString());

         writer.WriteStartAttribute("type");
         writer.WriteString(BerDefinitions.GetTypeName(node.BerTypeNumber));
         writer.WriteEndAttribute();

         writer.WriteValue(valueStr);

         writer.WriteEndElement();
         return null;
      }
      #endregion

      #region IEmberVisitor<XmlExportVisitorState,object> Members
      object IEmberVisitor<XmlExportState, object>.Visit(EmberContainer node, XmlExportState state)
      {
         return WriteContainer(node, state);
      }

      object IEmberVisitor<XmlExportState, object>.Visit(EmberSet node, XmlExportState state)
      {
         return WriteContainer(node, state);
      }

      object IEmberVisitor<XmlExportState, object>.Visit(EmberSequence node, XmlExportState state)
      {
         return WriteContainer(node, state);
      }

      object IEmberVisitor<XmlExportState, object>.Visit(BooleanEmberLeaf node, XmlExportState state)
      {
         return WriteLeaf(node, state, node.Value.ToString());
      }

      object IEmberVisitor<XmlExportState, object>.Visit(IntegerEmberLeaf node, XmlExportState state)
      {
         return WriteLeaf(node, state, node.Value.ToString(FormatProvider));
      }

      object IEmberVisitor<XmlExportState, object>.Visit(LongEmberLeaf node, XmlExportState state)
      {
         return WriteLeaf(node, state, node.Value.ToString(FormatProvider));
      }

      object IEmberVisitor<XmlExportState, object>.Visit(RealEmberLeaf node, XmlExportState state)
      {
         return WriteLeaf(node, state, node.Value.ToString(FormatProvider));
      }

      object IEmberVisitor<XmlExportState, object>.Visit(StringEmberLeaf node, XmlExportState state)
      {
         return WriteLeaf(node, state, node.Value);
      }

      object IEmberVisitor<XmlExportState, object>.Visit(OctetStringEmberLeaf node, XmlExportState state)
      {
         var buffer = new StringBuilder();
         var value = node.Value;

         for(int index = 0; index < value.Length; index++)
         {
            buffer.Append(value[index].ToString("X2"));

            if((index & 0x1F) == 0x1F)
               buffer.AppendLine();
         }

         return WriteLeaf(node, state, buffer.ToString());
      }

      object IEmberVisitor<XmlExportState, object>.Visit(ObjectIdentifierEmberLeaf node, XmlExportState state)
      {
         var buffer = new StringBuilder();
         var value = node.Value;

         for(int index = 0; index < value.Length; index++)
         {
            if(index >= 1)
               buffer.Append(".");

            buffer.Append(value[index].ToString());
         }

         return WriteLeaf(node, state, buffer.ToString());
      }

      object IEmberVisitor<XmlExportState, object>.Visit(RelativeOidEmberLeaf node, XmlExportState state)
      {
         var buffer = new StringBuilder();
         var value = node.Value;

         for(int index = 0; index < value.Length; index++)
         {
            if(index >= 1)
               buffer.Append(".");

            buffer.Append(value[index].ToString());
         }

         return WriteLeaf(node, state, buffer.ToString());
      }
      #endregion
   }

   internal class XmlExportState
   {
      public XmlExportState(XmlWriter writer)
      {
         Writer = writer;
      }

      public XmlWriter Writer { get; private set; }
   }
}
