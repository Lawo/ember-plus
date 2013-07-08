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

namespace BerLib
{
   /// <summary>
   /// Represents a BER tag as used in a TLV (Tag,Length,Value) tuple.
   /// Implements IEquatable and IComparable, provides == and != operators
   /// to test two instances for equality.
   /// </summary>
   public struct BerTag : IEquatable<BerTag>, IComparable<BerTag>, IComparable
   {
      /// <summary>
      /// Constructs a new BerTag instance.
      /// </summary>
      /// <param name="preamble">The encoded tag preamble containing information
      /// like tag class and the container flag.</param>
      /// <param name="number">The tag number.</param>
      public BerTag(byte preamble, uint number)
      : this()
      {
         Preamble = (byte)(preamble & 0xE0);
         Number = number;
      }

      /// <summary>
      /// Constructs a new BerTag instance, not setting
      /// the container flag.
      /// </summary>
      /// <param name="tagClass">The tag class.</param>
      /// <param name="number">The tag number.</param>
      public BerTag(BerClass tagClass, uint number)
      : this()
      {
         Preamble = (byte)(tagClass & BerClass.Mask);
         Number = number;
      }

      /// <summary>
      /// Constructs a new BerTag instance.
      /// </summary>
      /// <param name="tagClass">The tag class.</param>
      /// <param name="number">The tag number.</param>
      /// <param name="isContainer">If true, the container flag is set.</param>
      public BerTag(BerClass tagClass, uint number, bool isContainer)
      : this(tagClass, number)
      {
         IsContainer = isContainer;
      }

      /// <summary>
      /// Constructs a new BerTag instance with UNIVERSAL
      /// tag class and a universal type (<see cref="BerLib.BerType"/>).
      /// The container flag is not set.
      /// </summary>
      /// <param name="universalType">The universal type used as tag number
      /// (<see cref="BerLib.BerType"/>).</param>
      public BerTag(uint universalType)
      : this(BerClass.Universal, universalType, false)
      {
         AssertIsUniversal(universalType);
      }

      /// <summary>
      /// Constructs a new BerTag instance with UNIVERSAL
      /// tag class and a universal type (<see cref="BerLib.BerType"/>).
      /// </summary>
      /// <param name="universalType">The universal type used as tag number
      /// (<see cref="BerLib.BerType"/>).</param>
      /// <param name="isContainer">If true, the container flag is set.</param>
      public BerTag(uint universalType, bool isContainer)
      : this(BerClass.Universal, universalType, isContainer)
      {
         AssertIsUniversal(universalType);
      }

      /// <summary>
      /// Uninitialized (invalid) BerTag instance: [UNIVERSAL 0].
      /// This is a non-existing universal type.
      /// </summary>
      public static readonly BerTag Zero = new BerTag();

      /// <summary>
      /// Gets the encoded tag preamble containing information
      /// like tag class and the container flag.
      /// </summary>
      public byte Preamble { get; internal set; }

      /// <summary>
      /// Gets the tag number.
      /// </summary>
      public uint Number { get; internal set; }

      /// <summary>
      /// Gets a value indicating whether the container flag is set or not.
      /// </summary>
      public bool IsContainer
      {
         get { return (Preamble & ContainerFlag) != 0; }
         private set
         {
            if(value)
               Preamble |= ContainerFlag;
            else
               Preamble &= unchecked((byte)~ContainerFlag);
         }
      }

      /// <summary>
      /// Gets the tag class.
      /// </summary>
      public BerClass Class
      {
         get { return (BerClass)(Preamble & (byte)BerClass.Mask); }
         private set
         {
            Preamble = (byte)((Preamble & (byte)~BerClass.Mask) | (byte)value);
         }
      }

      /// <summary>
      /// Gets a value indicating whether this tag is invalid.
      /// </summary>
      public bool IsZero
      {
         get { return Preamble == 0 && Number == 0; }
      }

      /// <summary>
      /// Gets the number of the tag as a bertype value, taking the tag class into account.
      /// If the tag class is APPLICATION, the BerType.ApplicationFlag is set.
      /// </summary>
      public uint NumberAsType
      {
         get
         {
            return Class == BerClass.Application
                   ? Number | BerType.ApplicationFlag
                   : Number;
         }
      }

      /// <summary>
      /// Creates a copy of this BerTag instance with the container flag set.
      /// </summary>
      /// <returns>A copy of this BerTag instance with the container flag set.</returns>
      public BerTag ToContainer()
      {
         return new BerTag((byte)(Preamble | ContainerFlag), Number);
      }

      /// <summary>
      /// Creates a copy of this BerTag instance with the container flag cleared.
      /// </summary>
      /// <returns>A copy of this BerTag instance with the container flag cleared.</returns>
      public BerTag ToPrimitive()
      {
         return new BerTag((byte)(Preamble & ~ContainerFlag), Number);
      }

      /// <summary>
      /// Tries to parse a string in the format "class-number" into a BerTag.
      /// "class" is either "C", "U", "A" or "P".
      /// </summary>
      /// <param name="value">The string to parse.</param>
      /// <param name="tag">Receives the parsed tag if successful.</param>
      /// <returns>True if parsing was successful, otherwise false.</returns>
      public static bool TryParse(string value, out BerTag tag)
      {
         var tokens = value.Split(SplitChars, StringSplitOptions.RemoveEmptyEntries);

         if(tokens.Length == 2)
         {
            BerClass tagClass;

            if(BerDefinitions.GetClassFromName(tokens[0], true, out tagClass))
            {
               uint number;

               if(uint.TryParse(tokens[1], out number))
               {
                  tag = new BerTag(tagClass, number);
                  return true;
               }
            }
         }

         tag = Zero;
         return false;
      }

      /// <summary>
      /// Overriden to test two BerTag instances for equality.
      /// </summary>
      /// <param name="obj">Another BerTag instance.</param>
      /// <returns>True if this BerTag instance and <paramref name="obj"/>
      /// identify the same tag.</returns>
      public override bool Equals(object obj)
      {
         if(obj == null)
            return false;

         if(obj.GetType() != GetType())
            return false;

         return Equals((BerTag)obj);
      }

      /// <summary>
      /// Overriden to provide a hash code for the BerTag instance.
      /// </summary>
      /// <returns>Hash code over this BerTag instance.</returns>
      public override int GetHashCode()
      {
         return (int)((Preamble << 24) ^ (int)(Number & 0xFFFFFFFFUL));
      }

      /// <summary>
      /// Builds a string representation of this BerTag instance.
      /// Useful for debugging.
      /// </summary>
      /// <returns>A string representation of this BerTag instance in
      /// the form "class-number".</returns>
      public override string ToString()
      {
         var value = BerDefinitions.GetShortClassName(Class) + SplitChars[0] + Number.ToString();

         if(IsContainer)
            value += "!C";

         return value;
      }

      /// <summary>
      /// May be used to test two BerTag instances for equality.
      /// </summary>
      /// <param name="tag1">First Operand.</param>
      /// <param name="tag2">Second Operand.</param>
      /// <returns>True if <paramref name="tag1"/> and <paramref name="tag2"/>
      /// are equal, otherwise false.</returns>
      public static bool operator ==(BerTag tag1, BerTag tag2)
      {
         return tag1.Equals(tag2);
      }

      /// <summary>
      /// May be used to test two BerTag instances for inequality.
      /// </summary>
      /// <param name="tag1">First Operand.</param>
      /// <param name="tag2">Second Operand.</param>
      /// <returns>True if <paramref name="tag1"/> and <paramref name="tag2"/>
      /// are not equal, otherwise false.</returns>
      public static bool operator !=(BerTag tag1, BerTag tag2)
      {
         return tag1.Equals(tag2) == false;
      }

      #region Implementation
      const byte ContainerFlag = 0x20;
      static readonly char[] SplitChars = { '-' };

      static void AssertIsUniversal(uint type)
      {
         if(type >= BerType.LastUniversal
         || BerType.IsApplicationDefined(type))
            throw new ArgumentException("type is not universal!");
      }
      #endregion

      #region IEquatable<BerTag> Members
      /// <summary>
      /// Tests two BerTag instances for equality.
      /// </summary>
      /// <param name="other">Another BerTag instance.</param>
      /// <returns>True if this BerTag instance and <paramref name="other"/>
      /// identify the same tag.</returns>
      public bool Equals(BerTag other)
      {
         return other.Preamble == Preamble && other.Number == Number;
      }
      #endregion

      #region IComparable<BerTag> Members
      /// <summary>
      /// Compares this BerTag instance to another BerTag instance.
      /// Compares the tag classes first, then the tag numbers.
      /// Order of classes (ascending): UNIVERSAL, APPLICATION,
      /// CONTEXT-SPECIFIC, PRIVATE.
      /// </summary>
      /// <param name="other">BerTag instance to compare.</param>
      /// <returns>A value greater than zero, if this BerTag instance
      /// is greater than <paramref name="other"/>.
      /// Zero if both BerTag instances are equal.
      /// A value less than zero if this BerTag instance is less
      /// than <paramref name="other"/>.</returns>
      public int CompareTo(BerTag other)
      {
         var result = (int)Preamble - (int)other.Preamble;

         return result != 0
                ? result
                : (int)(Number & 0xFFFFFFFFUL) - (int)(other.Number & 0xFFFFFFFFUL);
      }
      #endregion

      #region IComparable Members
      int IComparable.CompareTo(object obj)
      {
         if(obj is BerTag)
            return CompareTo((BerTag)obj);

         throw new ArgumentException("type mismatch!");
      }
      #endregion
   }
}
