/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

using BerLib;

namespace EmberLib.Glow
{
   /// <summary>
   /// Application-defined BER Types used by the glow DTD
   /// </summary>
   public static class GlowType
   {
      public const uint Parameter                = BerType.ApplicationFlag | 1;
      public const uint Command                  = BerType.ApplicationFlag | 2;
      public const uint Node                     = BerType.ApplicationFlag | 3;
      public const uint ElementCollection        = BerType.ApplicationFlag | 4;
      public const uint StreamEntry              = BerType.ApplicationFlag | 5;
      public const uint StreamCollection         = BerType.ApplicationFlag | 6;
      public const uint StringIntegerPair        = BerType.ApplicationFlag | 7;
      public const uint StringIntegerCollection  = BerType.ApplicationFlag | 8;
      public const uint QualifiedParameter       = BerType.ApplicationFlag | 9;
      public const uint QualifiedNode            = BerType.ApplicationFlag | 10;
      public const uint RootElementCollection    = BerType.ApplicationFlag | 11;
      public const uint StreamDescription        = BerType.ApplicationFlag | 12;
      public const uint Matrix                   = BerType.ApplicationFlag | 13;
      public const uint Target                   = BerType.ApplicationFlag | 14;
      public const uint Source                   = BerType.ApplicationFlag | 15;
      public const uint Connection               = BerType.ApplicationFlag | 16;
      public const uint QualifiedMatrix          = BerType.ApplicationFlag | 17;
      public const uint Label                    = BerType.ApplicationFlag | 18;
      public const uint Function                 = BerType.ApplicationFlag | 19;
      public const uint QualifiedFunction        = BerType.ApplicationFlag | 20;
      public const uint TupleItemDescription     = BerType.ApplicationFlag | 21;
      public const uint Invocation               = BerType.ApplicationFlag | 22;
      public const uint InvocationResult         = BerType.ApplicationFlag | 23;
      public const uint Template                 = BerType.ApplicationFlag | 24;
      public const uint QualifiedTemplate        = BerType.ApplicationFlag | 25;
   }


   /// <summary>
   /// Defines global parameters of the Glow implementation
   /// </summary>
   public static class GlowDtd
   {
      /// <summary>
      /// The version of the Glow DTD implemented by this library
      /// </summary>
      public const ushort Version = 0x0232;
   }


   /// <summary>
   /// Defines the possible values of the "number" field of GlowCommand.
   /// </summary>
   public static class GlowCommandType
   {
      public const int Subscribe    = 30;
      public const int Unsubscribe  = 31;
      public const int GetDirectory = 32;
      public const int Invoke       = 33;
   }


   /// <summary>
   /// Defines the possible values of the "dirFieldMask" field of GlowCommand
   /// </summary>
   public static class GlowFieldFlags
   {
      public const int Sparse            = -2;
      public const int All               = -1;
      public const int Identifier        = 1;
      public const int Description       = 2;
      public const int Value             = 4;
      public const int Connections       = 5;
   }


   /// <summary>
   /// Defines the possible values of the "type" field of GlowParameter
   /// </summary>
   public static class GlowParameterType
   {
      public const int None    = 0;
      public const int Integer = 1;
      public const int Real    = 2;
      public const int String  = 3;
      public const int Boolean = 4;
      public const int Trigger = 5;
      public const int Enum    = 6;
      public const int Octets  = 7;
   }


   /// <summary>
   /// Defines the possible values of the "access" field of GlowParameter
   /// </summary>
   public static class GlowAccess
   {
      public const int None      = 0;
      public const int Read      = 1;
      public const int Write     = 2;
      public const int ReadWrite = Read | Write;
   }


   /// <summary>
   /// Defines the possible values of the "format" field of GlowStreamDescription
   /// </summary>
   public static class GlowStreamFormat
   {
      public const int UnsignedInt8              =  0;
      public const int UnsignedInt16BigEndian    =  2;
      public const int UnsignedInt16LittleEndian =  3;
      public const int UnsignedInt32BigEndian    =  4;
      public const int UnsignedInt32LittleEndian =  5;
      public const int UnsignedInt64BigEndian    =  6;
      public const int UnsignedInt64LittleEndian =  7;
      public const int SignedInt8                =  8;
      public const int SignedInt16BigEndian      = 10;
      public const int SignedInt16LittleEndian   = 11;
      public const int SignedInt32BigEndian      = 12;
      public const int SignedInt32LittleEndian   = 13;
      public const int SignedInt64BigEndian      = 14;
      public const int SignedInt64LittleEndian   = 15;
      public const int IeeeFloat32BigEndian      = 20;
      public const int IeeeFloat32LittleEndian   = 21;
      public const int IeeeFloat64BigEndian      = 22;
      public const int IeeeFloat64LittleEndian   = 23;
   }


   /// <summary>
   /// Defines the possible values of the "addressingMode" field of GlowMatrix
   /// </summary>
   public static class GlowMatrixAddressingMode
   {
      public const int Linear = 0;
      public const int NonLinear = 1;
   }


   /// <summary>
   /// Defines the possible values of the "type" field of GlowMatrix
   /// </summary>
   public static class GlowMatrixType
   {
      public const int OneToN   = 0;
      public const int OneToOne = 1;
      public const int NToN     = 2;
   }


   /// <summary>
   /// Defines the possible values of the "operation" field of GlowConnection
   /// </summary>
   public static class GlowConnectionOperation
   {
      public const int Absolute   = 0;
      public const int Connect    = 1;
      public const int Disconnect = 2;
   }


   /// <summary>
   /// Defines the possible values of the "disposition" field of GlowConnection
   /// </summary>
   public static class GlowConnectionDisposition
   {
      public const int Tally    = 0;
      public const int Modified = 1;
      public const int Pending  = 2;
      public const int Locked   = 3;
   }


   /// <summary>
   /// tags used by the Glow DTD, collected in one file for easy adjustment
   /// </summary>
   public static class GlowTags
   {
      /// <summary>
      /// context-specific tags used by the "Parameter" type
      /// </summary>
      public static class Parameter
      {
         public static readonly BerTag Number = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Contents = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Children = new BerTag(BerClass.ContextSpecific, 2);
      }

      /// <summary>
      /// context-specific tags used by the "QualifiedParameter" type
      /// </summary>
      public static class QualifiedParameter
      {
         public static readonly BerTag Path = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Contents = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Children = new BerTag(BerClass.ContextSpecific, 2);
      }

      /// <summary>
      /// context-specific tags shared by the "Parameter" and "QualifiedParameter" types
      /// </summary>
      public static class ParameterContents
      {
         public static readonly BerTag Identifier = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Description = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Value = new BerTag(BerClass.ContextSpecific, 2);
         public static readonly BerTag Minimum = new BerTag(BerClass.ContextSpecific, 3);
         public static readonly BerTag Maximum = new BerTag(BerClass.ContextSpecific, 4);
         public static readonly BerTag Access = new BerTag(BerClass.ContextSpecific, 5);
         public static readonly BerTag Format = new BerTag(BerClass.ContextSpecific, 6);
         public static readonly BerTag Enumeration = new BerTag(BerClass.ContextSpecific, 7);
         public static readonly BerTag Factor = new BerTag(BerClass.ContextSpecific, 8);
         public static readonly BerTag IsOnline = new BerTag(BerClass.ContextSpecific, 9);
         public static readonly BerTag Formula = new BerTag(BerClass.ContextSpecific, 10);
         public static readonly BerTag Step = new BerTag(BerClass.ContextSpecific, 11);
         public static readonly BerTag Default = new BerTag(BerClass.ContextSpecific, 12);
         public static readonly BerTag Type = new BerTag(BerClass.ContextSpecific, 13);
         public static readonly BerTag StreamIdentifier = new BerTag(BerClass.ContextSpecific, 14);
         public static readonly BerTag EnumMap = new BerTag(BerClass.ContextSpecific, 15);
         public static readonly BerTag StreamDescriptor = new BerTag(BerClass.ContextSpecific, 16);
         public static readonly BerTag SchemaIdentifiers = new BerTag(BerClass.ContextSpecific, 17);
         public static readonly BerTag TemplateReference = new BerTag(BerClass.ContextSpecific, 18);
      }

      /// <summary>
      /// context-specific tags used by the "Node" type
      /// </summary>
      public static class Node
      {
         public static readonly BerTag Number = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Contents = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Children = new BerTag(BerClass.ContextSpecific, 2);
      }

      /// <summary>
      /// context-specific tags used by the "QualifiedNode" type
      /// </summary>
      public static class QualifiedNode
      {
         public static readonly BerTag Path = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Contents = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Children = new BerTag(BerClass.ContextSpecific, 2);
      }

      /// <summary>
      /// context-specific tags shared by the "Node" and "QualifiedNode" types
      /// </summary>
      public static class NodeContents
      {
         public static readonly BerTag Identifier = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Description = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag IsRoot = new BerTag(BerClass.ContextSpecific, 2);
         public static readonly BerTag IsOnline = new BerTag(BerClass.ContextSpecific, 3);
         public static readonly BerTag SchemaIdentifiers = new BerTag(BerClass.ContextSpecific, 4);
         public static readonly BerTag TemplateReference = new BerTag(BerClass.ContextSpecific, 5);
      }

        /// <summary>
        /// context-specific tags used by the "Command" type
        /// </summary>
      public static class Command
      {
         public static readonly BerTag Number = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag DirFieldMask = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Invocation = new BerTag(BerClass.ContextSpecific, 2);
      }

      /// <summary>
      /// context-specific tags used by the "StreamEntry" type
      /// </summary>
      public static class StreamEntry
      {
         public static readonly BerTag StreamIdentifier = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag StreamValue = new BerTag(BerClass.ContextSpecific, 1);
      }

      /// <summary>
      /// context-specific tags used by the "StreamCollection" type
      /// </summary>
      public static class StreamCollection
      {
         public static readonly BerTag StreamEntry = new BerTag(BerClass.ContextSpecific, 0);
      }

      /// <summary>
      /// context-specific tags used by the "StringIntegerPair" type
      /// </summary>
      public static class StringIntegerPair
      {
         public static readonly BerTag EntryString = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag EntryInteger = new BerTag(BerClass.ContextSpecific, 1);
      }

      /// <summary>
      /// context-specific tags used by the "StringIntegerCollection" type
      /// </summary>
      public static class StringIntegerCollection
      {
         public static readonly BerTag StringIntegerPair = new BerTag(BerClass.ContextSpecific, 0);
      }

      /// <summary>
      /// context-specific tags used by the "StreamDescription" type
      /// </summary>
      public static class StreamDescription
      {
         public static readonly BerTag Format = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Offset = new BerTag(BerClass.ContextSpecific, 1);
      }

      /// <summary>
      /// explicit tag of the "Root" type (which is an alias for "ElementCollection")
      /// </summary>
      public static readonly BerTag Root = new BerTag(BerClass.Application, 0);

      /// <summary>
      /// context-specific tags used by the "Matrix" type
      /// </summary>
      public static class Matrix
      {
         public static readonly BerTag Number = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Contents = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Children = new BerTag(BerClass.ContextSpecific, 2);
         public static readonly BerTag Targets = new BerTag(BerClass.ContextSpecific, 3);
         public static readonly BerTag Sources = new BerTag(BerClass.ContextSpecific, 4);
         public static readonly BerTag Connections = new BerTag(BerClass.ContextSpecific, 5);
      }

      /// <summary>
      /// context-specific tags used by the "QualifiedMatrix" type
      /// </summary>
      public static class QualifiedMatrix
      {
         public static readonly BerTag Path = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Contents = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Children = new BerTag(BerClass.ContextSpecific, 2);
         public static readonly BerTag Targets = new BerTag(BerClass.ContextSpecific, 3);
         public static readonly BerTag Sources = new BerTag(BerClass.ContextSpecific, 4);
         public static readonly BerTag Connections = new BerTag(BerClass.ContextSpecific, 5);
      }

      /// <summary>
      /// context-specific tags used by the "MatrixContents" type
      /// </summary>
      public static class MatrixContents
      {
         public static readonly BerTag Identifier = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Description = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Type = new BerTag(BerClass.ContextSpecific, 2);
         public static readonly BerTag AddressingMode = new BerTag(BerClass.ContextSpecific, 3);
         public static readonly BerTag TargetCount = new BerTag(BerClass.ContextSpecific, 4);
         public static readonly BerTag SourceCount = new BerTag(BerClass.ContextSpecific, 5);
         public static readonly BerTag MaximumTotalConnects = new BerTag(BerClass.ContextSpecific, 6);
         public static readonly BerTag MaximumConnectsPerTarget = new BerTag(BerClass.ContextSpecific, 7);
         public static readonly BerTag ParametersLocation = new BerTag(BerClass.ContextSpecific, 8);
         public static readonly BerTag GainParameterNumber = new BerTag(BerClass.ContextSpecific, 9);
         public static readonly BerTag Labels = new BerTag(BerClass.ContextSpecific, 10);
         public static readonly BerTag SchemaIdentifiers = new BerTag(BerClass.ContextSpecific, 11);
         public static readonly BerTag TemplateReference = new BerTag(BerClass.ContextSpecific, 12);
      }

      /// <summary>
      /// context-specific tags used by the "Signal" type
      /// </summary>
      public static class Signal
      {
         public static readonly BerTag Number = new BerTag(BerClass.ContextSpecific, 0);
      }

      /// <summary>
      /// context-specific tags used by the "Connection" type
      /// </summary>
      public static class Connection
      {
         public static readonly BerTag Target = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Sources = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Operation = new BerTag(BerClass.ContextSpecific, 2);
         public static readonly BerTag Disposition = new BerTag(BerClass.ContextSpecific, 3);
      }

      /// <summary>
      /// context-specific tags used by the "Label" type
      /// </summary>
      public static class Label
      {
         public static readonly BerTag BasePath = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Description = new BerTag(BerClass.ContextSpecific, 1);
      }

      /// <summary>
      /// tag for items contained in a glow collection
      /// </summary>
      public static readonly BerTag CollectionItem = new BerTag(BerClass.ContextSpecific, 0);

      /// <summary>
      /// context-specific tags used by the "Function" type
      /// </summary>
      public static class Function
      {
         public static readonly BerTag Number = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Contents = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Children = new BerTag(BerClass.ContextSpecific, 2);
      }

      /// <summary>
      /// context-specific tags used by the "QualifiedFunction" type
      /// </summary>
      public static class QualifiedFunction
      {
         public static readonly BerTag Path = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Contents = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Children = new BerTag(BerClass.ContextSpecific, 2);
      }

      /// <summary>
      /// context-specific tags used by the "FunctionContents" type
      /// </summary>
      public static class FunctionContents
      {
         public static readonly BerTag Identifier = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Description = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Arguments = new BerTag(BerClass.ContextSpecific, 2);
         public static readonly BerTag Result = new BerTag(BerClass.ContextSpecific, 3);
         public static readonly BerTag TemplateReference = new BerTag(BerClass.ContextSpecific, 4);
      }

      /// <summary>
      /// context-specific tags used by the "TupleItemDescription" type
      /// </summary>
      public static class TupleItemDescription
      {
         public static readonly BerTag Type = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Name = new BerTag(BerClass.ContextSpecific, 1);
      }

      /// <summary>
      /// context-specific tags used by the "Invocation" type
      /// </summary>
      public static class Invocation
      {
         public static readonly BerTag InvocationId = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Arguments = new BerTag(BerClass.ContextSpecific, 1);
      }

      /// <summary>
      /// context-specific tags used by the "InvocationResult" type
      /// </summary>
      public static class InvocationResult
      {
         public static readonly BerTag InvocationId = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Success = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Result = new BerTag(BerClass.ContextSpecific, 2);
      }

      /// <summary>
      /// context-specific tags used by the "Template" type
      /// </summary>
      public static class Template
      {
         public static readonly BerTag Number = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Element = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Description = new BerTag(BerClass.ContextSpecific, 2);
      }

      /// <summary>
      /// context-specific tags used by the "QualifiedTemplate" type
      /// </summary>
      public static class QualifiedTemplate
      {
         public static readonly BerTag Path = new BerTag(BerClass.ContextSpecific, 0);
         public static readonly BerTag Element = new BerTag(BerClass.ContextSpecific, 1);
         public static readonly BerTag Description = new BerTag(BerClass.ContextSpecific, 2);
      }
   }
}
