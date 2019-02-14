/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

using System.Xml;
using BerLib;

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
            case GlowType.Parameter:               return new GlowParameter(tag);
            case GlowType.Command:                 return new GlowCommand(tag);
            case GlowType.Node:                    return new GlowNode(tag);
            case GlowType.ElementCollection:       return new GlowElementCollection(tag);
            case GlowType.StreamEntry:             return new GlowStreamEntry(tag);
            case GlowType.StreamCollection:        return new GlowStreamCollection(tag);
            case GlowType.StringIntegerPair:       return new GlowStringIntegerPair(tag);
            case GlowType.StringIntegerCollection: return new GlowStringIntegerCollection(tag);
            case GlowType.QualifiedNode:           return new GlowQualifiedNode(tag);
            case GlowType.QualifiedParameter:      return new GlowQualifiedParameter(tag);
            case GlowType.RootElementCollection:   return new GlowRootElementCollection(tag);
            case GlowType.StreamDescription:       return new GlowStreamDescription(tag);
            case GlowType.Matrix:                  return new GlowMatrix(tag);
            case GlowType.Target:                  return new GlowTarget(tag);
            case GlowType.Source:                  return new GlowSource(tag);
            case GlowType.Connection:              return new GlowConnection(tag);
            case GlowType.QualifiedMatrix:         return new GlowQualifiedMatrix(tag);
            case GlowType.Label:                   return new GlowLabel(tag);
            case GlowType.Function:                return new GlowFunction(tag);
            case GlowType.QualifiedFunction:       return new GlowQualifiedFunction(tag);
            case GlowType.TupleItemDescription:    return new GlowTupleItemDescription(tag);
            case GlowType.Invocation:              return new GlowInvocation(tag);
            case GlowType.InvocationResult:        return new GlowInvocationResult(tag);
            case GlowType.Template:                return new GlowTemplate(tag);
            case GlowType.QualifiedTemplate:       return new GlowQualifiedTemplate(tag);
         }

         return null;
      }

      public override EmberNode CreateNodeFromXml(uint type, BerTag tag, XmlReader reader)
      {
         switch(type)
         {
            case GlowType.Parameter:               return new GlowParameter(tag);
            case GlowType.Command:                 return new GlowCommand(tag);
            case GlowType.Node:                    return new GlowNode(tag);
            case GlowType.ElementCollection:       return new GlowElementCollection(tag);
            case GlowType.StreamEntry:             return new GlowStreamEntry(tag);
            case GlowType.StreamCollection:        return new GlowStreamCollection(tag);
            case GlowType.StringIntegerPair:       return new GlowStringIntegerPair(tag);
            case GlowType.StringIntegerCollection: return new GlowStringIntegerCollection(tag);
            case GlowType.QualifiedNode:           return new GlowQualifiedNode(tag);
            case GlowType.QualifiedParameter:      return new GlowQualifiedParameter(tag);
            case GlowType.RootElementCollection:   return new GlowRootElementCollection(tag);
            case GlowType.StreamDescription:       return new GlowStreamDescription(tag);
            case GlowType.Matrix:                  return new GlowMatrix(tag);
            case GlowType.Target:                  return new GlowTarget(tag);
            case GlowType.Source:                  return new GlowSource(tag);
            case GlowType.Connection:              return new GlowConnection(tag);
            case GlowType.QualifiedMatrix:         return new GlowQualifiedMatrix(tag);
            case GlowType.Label:                   return new GlowLabel(tag);
            case GlowType.Function:                return new GlowFunction(tag);
            case GlowType.QualifiedFunction:       return new GlowQualifiedFunction(tag);
            case GlowType.TupleItemDescription:    return new GlowTupleItemDescription(tag);
            case GlowType.Invocation:              return new GlowInvocation(tag);
            case GlowType.InvocationResult:        return new GlowInvocationResult(tag);
            case GlowType.Template:                return new GlowTemplate(tag);
            case GlowType.QualifiedTemplate:       return new GlowQualifiedTemplate(tag);
         }

         return base.CreateNodeFromXml(type, tag, reader);
      }
   }
}
