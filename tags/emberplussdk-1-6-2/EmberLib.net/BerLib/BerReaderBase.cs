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
using System.Diagnostics;

namespace BerLib
{
   /// <summary>
   /// Base class for all ber readers, providing methods
   /// to read the value of the current TLV.
   /// Derived classes are responsible to assign values to
   /// the properties Tag, Length, Type and IsContainer.
   /// </summary>
   public class BerReaderBase
   {
      /// <summary>
      /// Creates a new instance of BerReaderBase.
      /// </summary>
      protected BerReaderBase()
      {
      }

      /// <summary>
      /// Gets the tag of the current TLV as set by derived class.
      /// </summary>
      public BerTag Tag { get; protected set; }

      /// <summary>
      /// Gets the length of the current TLV as set by derived class.
      /// </summary>
      public int Length { get; protected set; }

      /// <summary>
      /// Gets the type of the current TLV as set by derived class.
      /// For an implicitly-tagged TLV, this is equal to Tag.Number.
      /// For an explicitly-tagged TLTLV, this is equal to the number
      /// of the inner tag.
      /// </summary>
      public uint Type { get; protected set; }

      /// <summary>
      /// Gets a value indicating whether the current TLV is the start
      /// of a container.
      /// Set by derived class.
      /// </summary>
      public bool IsContainer { get; protected set; }

      /// <summary>
      /// Gets or sets the encoded value of the current TLV as a byte
      /// array.
      /// Must be set by derived class when moving to the nex TLV.
      /// </summary>
      protected byte[] Value { get; set; }

      /// <summary>
      /// Gets or sets the outer length of the current TLV. For an
      /// implictly tagged TLV, this is equal to the inner length
      /// since a TLV only contains one length.
      /// For an explicitly tagged TLTLV, this is the length following
      /// the outer tag.
      /// </summary>
      protected int OuterLength { get; set; }

      /// <summary>
      /// Clears all properties containing information about the
      /// current TLV.
      /// </summary>
      protected virtual void DisposeCurrentTlv()
      {
         Value = null;

         Tag = BerTag.Zero;
         OuterLength = 0;
         Type = 0;
         IsContainer = false;

         Length = 0;
      }

      /// <summary>
      /// Throws a BerException describing the occurred error.
      /// </summary>
      /// <param name="errorCode">Integer code uniquely identifying the source of the error.</param>
      /// <param name="message">Associated error message.</param>
      protected virtual void ThrowError(int errorCode, string message)
      {
         throw new BerException(errorCode, message);
      }

      /// <summary>
      /// Decodes the value of the current TLV as boolean.
      /// Throws an exception in case of a format mismatch.
      /// </summary>
      /// <returns>The boolean value of the current TLV.</returns>
      public bool GetBoolean()
      {
         if(IsContainer || Length == 0 || Value == null)
            ThrowError(201, "Invalid Boolean encoding");

         Debug.Assert(Value != null || Length == 0);
         Debug.Assert(Type == BerType.Boolean || BerType.IsApplicationDefined(Type));

         var input = new BerMemoryInput(Value);

         return BerEncoding.DecodeBoolean(input);
      }

      /// <summary>
      /// Decodes the value of the current TLV a 32bit integer.
      /// Throws an exception in case of a format mismatch.
      /// </summary>
      /// <returns>The integer value of the current TLV.</returns>
      public int GetInteger()
      {
         if(IsContainer || Length == 0 || Value == null)
            ThrowError(202, "Invalid Integer encoding");

         Debug.Assert(Value != null || Length == 0);
         Debug.Assert(Type == BerType.Integer || BerType.IsApplicationDefined(Type));

         var input = new BerMemoryInput(Value);

         return BerEncoding.DecodeInteger(input, Length);
      }

      /// <summary>
      /// Decodes the value of the current TLV a 64bit integer.
      /// Throws an exception in case of a format mismatch.
      /// </summary>
      /// <returns>The integer value of the current TLV.</returns>
      public long GetLong()
      {
         if(IsContainer || Length == 0 || Value == null)
            ThrowError(203, "Invalid Integer encoding");

         Debug.Assert(Value != null || Length == 0);
         Debug.Assert(Type == BerType.Integer || BerType.IsApplicationDefined(Type));

         var input = new BerMemoryInput(Value);

         return BerEncoding.DecodeLong(input, Length);
      }

      /// <summary>
      /// Decodes the value of the current TLV as double.
      /// Throws an exception in case of a format mismatch.
      /// </summary>
      /// <returns>The double value of the current TLV.</returns>
      public double GetReal()
      {
         if(IsContainer)
            ThrowError(204, "Invalid Real encoding");

         Debug.Assert(Value != null || Length == 0);
         Debug.Assert(Type == BerType.Real || BerType.IsApplicationDefined(Type));

         var input = new BerMemoryInput(Value);

         return BerEncoding.DecodeReal(input, Length);
      }

      /// <summary>
      /// Decodes the value of the current TLV an UTF-8 string.
      /// Throws an exception in case of a format mismatch.
      /// </summary>
      /// <returns>The string value of the current TLV.</returns>
      public string GetString()
      {
         if(IsContainer)
            ThrowError(205, "Invalid String encoding");

         Debug.Assert(Value != null || Length == 0);
         Debug.Assert(Type == BerType.UTF8String || Type == BerType.Bitstring || BerType.IsApplicationDefined(Type));

         var input = new BerMemoryInput(Value);

         return BerEncoding.DecodeUtf8String(input, Length);
      }

      /// <summary>
      /// Decodes the value of the current TLV as an octet string (byte array).
      /// Throws an exception in case of a format mismatch.
      /// </summary>
      /// <returns>Byte array containing the OCTET STRING value of the current TLV.</returns>
      public byte[] GetOctetString()
      {
         if(IsContainer)
            ThrowError(206, "Invalid OctetString encoding");

         Debug.Assert(Value != null || Length == 0);
         Debug.Assert(Type == BerType.OctetString || BerType.IsApplicationDefined(Type));

         var input = new BerMemoryInput(Value);

         return BerEncoding.DecodeByteArray(input, Length);
      }

      /// <summary>
      /// Decodes the value of the current TLV as an OBJECT IDENTIFIER represented
      /// as an integer array, each integer containing one sub-identifier.
      /// Throws an exception in case of a format mismatch.
      /// </summary>
      /// <returns>Integer array containing the OBJECT IDENTIFIER value of the current TLV.</returns>
      public int[] GetObjectIdentifier()
      {
         if(IsContainer || Length == 0 || Value == null)
            ThrowError(209, "Invalid ObjectIdentifier encoding");

         Debug.Assert(Value != null || Length == 0);
         Debug.Assert(Type == BerType.ObjectIdentifier || BerType.IsApplicationDefined(Type));

         var input = new BerMemoryInput(Value);

         return BerEncoding.DecodeObjectIdentifier(input, Length);
      }

      /// <summary>
      /// Decodes the value of the current TLV as an RelativeOid.
      /// Throws an exception in case of a format mismatch.
      /// </summary>
      /// <returns>An array of integers containing the subidentifiers of the RelativeOid value of the current TLV.</returns>
      public int[] GetRelativeOid()
      {
         if(IsContainer)
            ThrowError(207, "Invalid RelativeOid encoding");

         Debug.Assert(Value != null || Length == 0);
         Debug.Assert(Type == BerType.RelativeOid || BerType.IsApplicationDefined(Type));

         var input = new BerMemoryInput(Value);

         return BerEncoding.DecodeRelativeOid(input, Length);
      }

      /// <summary>
      /// Decodes the value of the current TLV as an GeneralizedTime represented
      /// as a DateTime structure.
      /// Throws an exception in case of a format mismatch.
      /// </summary>
      /// <returns>DateTime value containing the GeneralizedTime value of the current TLV.</returns>
      public DateTime GetGeneralizedTime()
      {
         if(IsContainer || Length == 0 || Value == null)
            ThrowError(208, "Invalid GeneralizedTime encoding");

         Debug.Assert(Value != null || Length == 0);
         Debug.Assert(Type == BerType.GeneralizedTime || BerType.IsApplicationDefined(Type));

         var input = new BerMemoryInput(Value);

         return BerEncoding.DecodeGeneralizedTime(input, Length);
      }
   }
}
