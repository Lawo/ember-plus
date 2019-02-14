/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

using System.Xml;
using BerLib;

namespace EmberLib
{
   /// <summary>
   /// This type defines methods which can be used to create EmberNode objects from
   /// TLVs with application-defined types.
   /// </summary>
   public abstract class EmberApplicationInterface
   {
      /// <summary>
      /// Override this method to create custom EmberNode-derived objects.
      /// This method is called when a decoder encounters a TLV with an
      /// application-defined type.
      /// </summary>
      /// <param name="type">The type of the decoded TLV, with BerType.ApplicationFlag set.</param>
      /// <param name="reader">A reader positioned on the TLV. Use the Get() methods of
      /// BerReaderBase to retrieve the value of the TLV.</param>
      /// <returns>Return a new instance of a type derived from EmberNode or null
      /// if no application-defined type is mapped to the passed <paramref name="type"/>.</returns>
      public abstract EmberNode CreateNodeFromReader(uint type, BerReaderBase reader);

      /// <summary>
      /// Override this method to create custom EmberNode-derived objects.
      /// This method is called when an XML decoder encounters a element with an
      /// application-defined type.
      /// </summary>
      /// <param name="type">The type of the decoded element, with BerType.ApplicationFlag set.</param>
      /// <param name="tag">The tag of the decoded element.</param>
      /// <param name="reader">A reader positioned on the element.</param>
      /// <returns>Return a new instance of a type derived from EmberNode or null
      /// if no application-defined type is mapped to the passed <paramref name="type"/>.</returns>
      public virtual EmberNode CreateNodeFromXml(uint type, BerTag tag, XmlReader reader)
      {
         return null;
      }

      /// <summary>
      /// Creates a container of an application-defined type with Set semantics
      /// </summary>
      /// <param name="type">Application-defined type. BerType.ApplicationFlag can be omitted.</param>
      public static EmberSet CreateApplicationDefinedSet(BerTag tag, uint type, EmberContainer parent)
      {
         var set = new EmberSet(tag, parent)
         {
            BerTypeNumber = type | BerType.ApplicationFlag
         };

         return set;
      }

      /// <summary>
      /// Creates a container of an application-defined type with Set semantics
      /// </summary>
      /// <param name="type">Application-defined type. BerType.ApplicationFlag can be omitted.</param>
      public static EmberSet CreateApplicationDefinedSet(BerTag tag, uint type)
      {
         return CreateApplicationDefinedSet(tag, type, null);
      }

      /// <summary>
      /// Creates a container of an application-defined type with Sequence semantics
      /// </summary>
      /// <param name="type">Application-defined type. BerType.ApplicationFlag can be omitted.</param>
      public static EmberSequence CreateApplicationDefinedSequence(BerTag tag, uint type, EmberContainer parent, bool isOrdered = false)
      {
         var sequence = new EmberSequence(tag, parent)
         {
            BerTypeNumber = type | BerType.ApplicationFlag,
            IsOrdered = isOrdered,
         };

         return sequence;
      }

      /// <summary>
      /// Creates a container of an application-defined type with Sequence semantics
      /// </summary>
      /// <param name="type">Application-defined type. BerType.ApplicationFlag can be omitted.</param>
      public static EmberSequence CreateApplicationDefinedSequence(BerTag tag, uint type)
      {
         return CreateApplicationDefinedSequence(tag, type, null);
      }

      /// <summary>
      /// Creates an EmberSet object with the passed BerType. Convenience method
      /// to be called from an overridden CreateNodeFromReader method.
      /// </summary>
      protected EmberNode CreateSet(uint type, BerReaderBase reader)
      {
         var tag = reader.Tag;
         var node = new EmberSet(tag, null);

         node.BerTypeNumber = type;
         return node;
      }

      /// <summary>
      /// Creates an EmberSet object with the passed BerType. Convenience method
      /// to be called from an overridden CreateNodeFromXml method.
      /// </summary>
      protected EmberNode CreateSet(uint type, BerTag tag, XmlReader reader)
      {
         var node = new EmberSet(tag, null, true)
         {
            BerTypeNumber = type
         };

         return node;
      }

      /// <summary>
      /// Creates an EmberSequence object with the passed BerType. Convenience method
      /// to be called from an overridden CreateNodeFromReader method.
      /// </summary>
      protected EmberNode CreateSequence(uint type, BerReaderBase reader)
      {
         var tag = reader.Tag;
         var node = new EmberSequence(tag, null);

         node.BerTypeNumber = type;
         return node;
      }

      /// <summary>
      /// Creates an EmberSequence object with the passed BerType. Convenience method
      /// to be called from an overridden CreateNodeFromXml method.
      /// </summary>
      protected EmberNode CreateSequence(uint type, BerTag tag, XmlReader reader)
      {
         var node = new EmberSequence(tag, null)
         {
            BerTypeNumber = type
         };

         return node;
      }
   }
}
