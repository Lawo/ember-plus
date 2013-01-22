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
using System.Xml;

namespace EmberLib.Glow
{
   /// <summary>
   /// The Ember application interface for the Glow DTD.
   /// Creates container classes defined in EmberLib.Glow for the types defined in GlowDtd.asn.
   /// </summary>
   public class GlowApplicationInterface : EmberApplicationInterface
   {
      public override EmberNode CreateNodeFromReader(uint type, BerLib.BerReaderBase reader)
      {
         var tag = reader.Tag;

         switch(type)
         {
            case GlowType.Parameter:
               return new GlowParameter(tag);

            case GlowType.Command:
               return new GlowCommand(tag);

            case GlowType.Node:
               return new GlowNode(tag);

            case GlowType.ElementCollection:
               return new GlowElementCollection(tag);

            case GlowType.StreamEntry:
               return new GlowStreamEntry(tag);

            case GlowType.StreamCollection:
               return new GlowStreamCollection(tag);

            case GlowType.StringIntegerPair:
               return new GlowStringIntegerPair(tag);

            case GlowType.StringIntegerCollection:
               return new GlowStringIntegerCollection(tag);

            case GlowType.QualifiedNode:
               return new GlowQualifiedNode(tag);

            case GlowType.QualifiedParameter:
               return new GlowQualifiedParameter(tag);

            case GlowType.RootElementCollection:
               return new GlowRootElementCollection(tag);

            case GlowType.StreamDescription:
               return new GlowStreamDescription(tag);

            case GlowType.Matrix:
               return new GlowMatrix(tag);

            case GlowType.Target:
               return new GlowTarget(tag);

            case GlowType.Source:
               return new GlowSource(tag);

            case GlowType.Connection:
               return new GlowConnection(tag);

            case GlowType.QualifiedMatrix:
               return new GlowQualifiedMatrix(tag);

            case GlowType.Label:
               return new GlowLabel(tag);
         }

         return null;
      }

      public override EmberNode CreateNodeFromXml(uint type, BerTag tag, XmlReader reader)
      {
         switch(type)
         {
            case GlowType.Parameter:
               return new GlowParameter(tag);

            case GlowType.Command:
               return new GlowCommand(tag);

            case GlowType.Node:
               return new GlowNode(tag);

            case GlowType.ElementCollection:
               return new GlowElementCollection(tag);

            case GlowType.StreamEntry:
               return new GlowStreamEntry(tag);

            case GlowType.StreamCollection:
               return new GlowStreamCollection(tag);

            case GlowType.StringIntegerPair:
               return new GlowStringIntegerPair(tag);

            case GlowType.StringIntegerCollection:
               return new GlowStringIntegerCollection(tag);

            case GlowType.QualifiedNode:
               return new GlowQualifiedNode(tag);

            case GlowType.QualifiedParameter:
               return new GlowQualifiedParameter(tag);

            case GlowType.RootElementCollection:
               return new GlowRootElementCollection(tag);

            case GlowType.StreamDescription:
               return new GlowStreamDescription(tag);

            case GlowType.Matrix:
               return new GlowMatrix(tag);

            case GlowType.Target:
               return new GlowTarget(tag);

            case GlowType.Source:
               return new GlowSource(tag);

            case GlowType.Connection:
               return new GlowConnection(tag);

            case GlowType.QualifiedMatrix:
               return new GlowQualifiedMatrix(tag);

            case GlowType.Label:
               return new GlowLabel(tag);
         }

         return base.CreateNodeFromXml(type, tag, reader);
      }
   }
}
