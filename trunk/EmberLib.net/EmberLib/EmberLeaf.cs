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
using System.Diagnostics;
using BerLib;
using System.Globalization;

namespace EmberLib
{
   /// <summary>
   /// Base class for leaf nodes. Leaf nodes cannot contain children,
   /// only a primitive value.
   /// </summary>
   /// <typeparam name="T">The type of primitive value the leaf contains.
   /// <para>
   /// ASN.1/BER to CLR type mapping:
   /// INTEGER - int/long
   /// UTF8String - string
   /// BOOLEAN - bool
   /// REAL - double
   /// OCTET STRING - byte[]
   /// OBJECT IDENTIFIER - int[]
   /// RELATIVE OBJECT - ulong
   /// GeneralizedTime - DateTime
   /// </para></typeparam>
   public abstract class EmberLeaf<T> : EmberNode
   {
      #region Class DummyEnumerator
      protected sealed class DummyEnumerator : IEnumerator<EmberNode>
      {
         internal static readonly DummyEnumerator Instance = new DummyEnumerator();

         private DummyEnumerator()
         {
         }

         #region IEnumerator<Ember_Node> Members
         public EmberNode Current
         {
            get { return null; }
         }
         #endregion

         #region IDisposable Members
         public void Dispose()
         {
         }
         #endregion

         #region IEnumerator Members
         object System.Collections.IEnumerator.Current
         {
            get { return null; }
         }

         public bool MoveNext()
         {
            return false;
         }

         public void Reset()
         {
         }
         #endregion
      }
      #endregion

      /// <summary>
      /// Constructs a new instance of EmberLeaf with the specified tag.
      /// </summary>
      /// <param name="tag">The tag of the newly created EmberLeaf</param>
      public EmberLeaf(BerTag tag)
      : base(tag)
      {
      }

      /// <summary>
      /// Gets the contained primitive value of the leaf.
      /// </summary>
      public T Value { get; internal set; }

      /// <summary>
      /// Gets the number of child nodes this node contains.
      /// Overriden to return 0.
      /// </summary>
      public override sealed int Count
      {
         get { return 0; }
      }

      /// <summary>
      /// Gets a value indicating whether this container needs to be pre-encoded
      /// before being written out.
      /// </summary>
      internal override bool IsDirty
      {
         get { return _encoded == null; }
      }

      /// <summary>
      /// Writes the binary BER representaion of this node to output.
      /// </summary>
      /// <param name="output">The output to write to.</param>
      internal override void EncodeOverride(IBerOutput output)
      {
         if(_encoded == null)
            Update();

         output.WriteBytes(_encoded);
      }

      /// <summary>
      /// Removes the child node with the specified tag.
      /// Overriden to throws an exception because this node is not a container.
      /// </summary>
      /// <param name="tag">The tag of the child node to remove.</param>
      /// <returns>True if the child node was found and removed, otherwise false.</returns>
      public override sealed bool Remove(BerTag tag)
      {
         throw new BerException(3, Error_NotAContainer);
      }

      /// <summary>
      /// Gets the child node with the specified tag.
      /// Overriden to throws an exception because this node is not a container.
      /// </summary>
      /// <param name="tag">The tag of the child node to get.</param>
      /// <returns>The child node with the specified tag or null if no such
      /// node exists.</returns>
      internal override EmberNode GetChildNode(BerTag tag)
      {
         throw new BerException(3, Error_NotAContainer);
      }

      /// <summary>
      /// Overriden to return an empty enumerator.
      /// </summary>
      public override sealed IEnumerator<EmberNode> GetEnumerator()
      {
         return DummyEnumerator.Instance;
      }

      /// <summary>
      /// Inserts a node into this node's collection of children.
      /// Overriden to throws an exception because this node is not a container.
      /// </summary>
      /// <param name="node">The node to insert.</param>
      protected internal override sealed void InsertChildNode(EmberNode node)
      {
         throw new BerException(3, Error_NotAContainer);
      }

      protected const string Error_NotAContainer = "Not a container";

      /// <summary>
      /// Encoded leaf data
      /// </summary>
      protected byte[] Encoded
      {
         get { return _encoded; }
         set
         {
            _encoded = value;
         }
      }

      #region Implementation
      byte[] _encoded;
      #endregion
   }


   /// <summary>
   /// Leaf class to hold a primitive BOOLEAN value.
   /// </summary>
   public sealed class BooleanEmberLeaf : EmberLeaf<bool>
   {
      /// <summary>
      /// Creates a new instance containing the passed <paramref name="value"/>
      /// </summary>
      /// <param name="tag">The tag of the newly created node.</param>
      /// <param name="value">The primitive value to hold.</param>
      public BooleanEmberLeaf(BerTag tag, bool value)
      : base(tag)
      {
         Value = value;
         Type = BerType.Boolean;
      }

      public override TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      /// <summary>
      /// Pre-encodes the BER header and returns the byte length of the encoded node.
      /// Overriden to encode the entire TLTLV
      /// </summary>
      /// <returns>The length of the encoded node.</returns>
      internal override int Update()
      {
         var output = new BerMemoryOutput();

         BerEncoding.EncodeTag(output, Tag.ToContainer());
         BerEncoding.EncodeLength(output, 3);

         BerEncoding.EncodeTag(output, new BerTag(BerType.Boolean));
         BerEncoding.EncodeLength(output, 1);
         BerEncoding.EncodeBoolean(output, Value);

         Encoded = output.ToArray();
         EncodedLength = Encoded.Length;

         return EncodedLength;
      }
   }


   /// <summary>
   /// Leaf class to hold a primitive INTEGER value.
   /// When decoding from a reader, an instance of IntegerEmberLeaf is
   /// created if the value fits into a 32bit signed value.
   /// Otherwise a LongIntegerLeaf instance is created.
   /// </summary>
   public sealed class IntegerEmberLeaf : EmberLeaf<int>
   {
      /// <summary>
      /// Creates a new instance containing the passed <paramref name="value"/>
      /// </summary>
      /// <param name="tag">The tag of the newly created node.</param>
      /// <param name="value">The primitive value to hold.</param>
      public IntegerEmberLeaf(BerTag tag, int value)
      : base(tag)
      {
         Value = value;
         Type = BerType.Integer;
      }

      public override TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      /// <summary>
      /// Pre-encodes the BER header and returns the byte length of the encoded node.
      /// Overriden to encode the entire TLTLV
      /// </summary>
      /// <returns>The length of the encoded node.</returns>
      internal override int Update()
      {
         var output = new BerMemoryOutput();
         var valueLength = BerEncoding.GetIntegerLength(Value);

         BerEncoding.EncodeTag(output, Tag.ToContainer());
         BerEncoding.EncodeLength(output, valueLength + 2);

         BerEncoding.EncodeTag(output, new BerTag(BerType.Integer));
         BerEncoding.EncodeLength(output, valueLength);
         BerEncoding.EncodeInteger(output, Value, valueLength);

         Encoded = output.ToArray();
         EncodedLength = Encoded.Length;
         return EncodedLength;
      }
   }


   /// <summary>
   /// Leaf class to hold a primitive INTEGER value.
   /// When decoding from a reader, an instance of IntegerEmberLeaf is
   /// created if the value fits into a 32bit signed value.
   /// Otherwise a LongIntegerLeaf instance is created.
   /// </summary>
   public sealed class LongEmberLeaf : EmberLeaf<long>
   {
      /// <summary>
      /// Creates a new instance containing the passed <paramref name="value"/>
      /// </summary>
      /// <param name="tag">The tag of the newly created node.</param>
      /// <param name="value">The primitive value to hold.</param>
      public LongEmberLeaf(BerTag tag, long value)
      : base(tag)
      {
         Value = value;
         Type = BerType.Integer;
      }

      public override TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      /// <summary>
      /// Pre-encodes the BER header and returns the byte length of the encoded node.
      /// Overriden to encode the entire TLTLV
      /// </summary>
      /// <returns>The length of the encoded node.</returns>
      internal override int Update()
      {
         var output = new BerMemoryOutput();
         var valueLength = BerEncoding.GetLongLength(Value);

         BerEncoding.EncodeTag(output, Tag.ToContainer());
         BerEncoding.EncodeLength(output, valueLength + 2);

         BerEncoding.EncodeTag(output, new BerTag(BerType.Integer));
         BerEncoding.EncodeLength(output, valueLength);
         BerEncoding.EncodeLong(output, Value, valueLength);

         Encoded = output.ToArray();
         EncodedLength = Encoded.Length;
         return EncodedLength;
      }
   }


   /// <summary>
   /// Leaf class to hold a primitive REAL value.
   /// </summary>
   public sealed class RealEmberLeaf : EmberLeaf<double>
   {
      /// <summary>
      /// Creates a new instance containing the passed <paramref name="value"/>
      /// </summary>
      /// <param name="tag">The tag of the newly created node.</param>
      /// <param name="value">The primitive value to hold.</param>
      public RealEmberLeaf(BerTag tag, double value)
      : base(tag)
      {
         Value = value;
         Type = BerType.Real;
      }

      public override TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      /// <summary>
      /// Pre-encodes the BER header and returns the byte length of the encoded node.
      /// Overriden to encode the entire TLTLV
      /// </summary>
      /// <returns>The length of the encoded node.</returns>
      internal override int Update()
      {
         var output = new BerMemoryOutput();

         int tagLength, innerLength = 0, valueLength;

         tagLength = BerEncoding.EncodeTag(output, Tag.ToContainer());
         BerEncoding.EncodeLength(output, 0);

         innerLength += BerEncoding.EncodeTag(output, new BerTag(BerType.Real));
         innerLength += BerEncoding.EncodeLength(output, 0);
         innerLength += valueLength = BerEncoding.EncodeReal(output, Value);

         Encoded = output.ToArray();
         EncodedLength = Encoded.Length;

         Encoded[tagLength] = (byte)innerLength; // fix outer length
         Encoded[tagLength + 2] = (byte)valueLength; // fix inner length
         return EncodedLength;
      }
   }


   /// <summary>
   /// Leaf class to hold a primitive UTF8String value.
   /// </summary>
   public sealed class StringEmberLeaf : EmberLeaf<string>
   {
      /// <summary>
      /// Creates a new instance containing the passed <paramref name="value"/>
      /// </summary>
      /// <param name="tag">The tag of the newly created node.</param>
      /// <param name="value">The primitive value to hold.</param>
      public StringEmberLeaf(BerTag tag, string value)
      : base(tag)
      {
         Value = value ?? string.Empty;
         Type = BerType.UTF8String;
      }

      internal override void EncodeOverride(IBerOutput output)
      {
         Debug.Assert(Value != null);

         base.EncodeOverride(output);

         BerEncoding.EncodeUtf8String(output, Value);
      }

      public override TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      /// <summary>
      /// Pre-encodes the BER header and returns the byte length of the encoded node.
      /// </summary>
      /// <returns>The length of the encoded node.</returns>
      internal override int Update()
      {
         var output = new BerMemoryOutput();
         var stringLength = BerEncoding.GetUtf8StringLength(Value);

         var tagImplicit = new BerTag(BerType.UTF8String);
         var implicitLength = (stringLength + BerEncoding.GetHeaderLength(tagImplicit, stringLength));

         BerEncoding.EncodeTag(output, Tag.ToContainer());
         BerEncoding.EncodeLength(output, implicitLength);

         BerEncoding.EncodeTag(output, tagImplicit);
         BerEncoding.EncodeLength(output, stringLength);

         Encoded = output.ToArray();
         EncodedLength = stringLength + Encoded.Length;
         return EncodedLength;
      }
   }


   /// <summary>
   /// Leaf class to hold a primitive OCTET STRING value as a byte array.
   /// </summary>
   public sealed class OctetStringEmberLeaf : EmberLeaf<byte[]>
   {
      /// <summary>
      /// Creates a new instance containing the passed <paramref name="value"/>
      /// </summary>
      /// <param name="tag">The tag of the newly created node.</param>
      /// <param name="value">The primitive value to hold.</param>
      public OctetStringEmberLeaf(BerTag tag, byte[] value)
      : base(tag)
      {
         if(value == null)
            throw new ArgumentNullException("value");

         Value = value;
         Type = BerType.OctetString;
      }

      internal override void EncodeOverride(IBerOutput output)
      {
         Debug.Assert(Value != null);

         base.EncodeOverride(output);

         BerEncoding.EncodeByteArray(output, Value);
      }

      public override TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      /// <summary>
      /// Pre-encodes the BER header and returns the byte length of the encoded node.
      /// Overriden to encode the entire TLTLV
      /// </summary>
      /// <returns>The length of the encoded node.</returns>
      internal override int Update()
      {
         var output = new BerMemoryOutput();
         var arrayLength = Value.Length;

         var tagImplicit = new BerTag(BerType.OctetString);
         var implicitLength = (arrayLength + BerEncoding.GetHeaderLength(tagImplicit, arrayLength));

         BerEncoding.EncodeTag(output, Tag.ToContainer());
         BerEncoding.EncodeLength(output, implicitLength);

         BerEncoding.EncodeTag(output, tagImplicit);
         BerEncoding.EncodeLength(output, arrayLength);

         Encoded = output.ToArray();
         EncodedLength = arrayLength + Encoded.Length;
         return EncodedLength;
      }

      #region Implementation
      static byte[] EmptyBytes = new byte[0];
      #endregion
   }


   /// <summary>
   /// Leaf class to hold a primitive OBJECT IDENTIFIER value as an int array.
   /// </summary>
   public sealed class ObjectIdentifierEmberLeaf : EmberLeaf<int[]>
   {
      /// <summary>
      /// Creates a new instance containing the passed <paramref name="value"/>
      /// </summary>
      /// <param name="tag">The tag of the newly created node.</param>
      /// <param name="value">The primitive value to hold.</param>
      public ObjectIdentifierEmberLeaf(BerTag tag, int[] value)
      : base(tag)
      {
         Value = (value != null && value.Length > 1)
                 ? value
                 : new int[] { 0, 0 };

         Type = BerType.ObjectIdentifier;
      }

      internal override void EncodeOverride(IBerOutput output)
      {
         Debug.Assert(Value != null);

         base.EncodeOverride(output);

         BerEncoding.EncodeObjectIdentifier(output, Value);
      }

      public override TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      /// <summary>
      /// Pre-encodes the BER header and returns the byte length of the encoded node.
      /// </summary>
      /// <returns>The length of the encoded node.</returns>
      internal override int Update()
      {
         var output = new BerMemoryOutput();
         var tagImplicit = new BerTag(BerType.ObjectIdentifier);
         var valueLength = BerEncoding.GetObjectIdentifierLength(Value);
         var implicitLength = valueLength + BerEncoding.GetHeaderLength(tagImplicit, valueLength);

         BerEncoding.EncodeTag(output, Tag.ToContainer());
         BerEncoding.EncodeLength(output, implicitLength);

         BerEncoding.EncodeTag(output, tagImplicit);
         BerEncoding.EncodeLength(output, valueLength);

         Encoded = output.ToArray();

         EncodedLength = valueLength + Encoded.Length;
         return EncodedLength;
      }
   }


   /// <summary>
   /// Leaf class to hold a primitive RELATIVE OBJECT value as a ulong.
   /// </summary>
   public sealed class RelativeOidEmberLeaf : EmberLeaf<int[]>
   {
      /// <summary>
      /// Creates a new instance containing the passed <paramref name="value"/>
      /// </summary>
      /// <param name="tag">The tag of the newly created node.</param>
      /// <param name="value">The primitive value to hold.</param>
      public RelativeOidEmberLeaf(BerTag tag, int[] value)
      : base(tag)
      {
         Value = value;
         Type = BerType.RelativeOid;
      }

      internal override void EncodeOverride(IBerOutput output)
      {
         Debug.Assert(Value != null);

         base.EncodeOverride(output);

         BerEncoding.EncodeRelativeOid(output, Value);
      }

      public override TResult Accept<TState, TResult>(IEmberVisitor<TState, TResult> visitor, TState state)
      {
         return visitor.Visit(this, state);
      }

      /// <summary>
      /// Pre-encodes the BER header and returns the byte length of the encoded node.
      /// Overriden to encode the entire TLTLV
      /// </summary>
      /// <returns>The length of the encoded node.</returns>
      internal override int Update()
      {
         var output = new BerMemoryOutput();
         var tagImplicit = new BerTag(BerType.RelativeOid);
         var valueLength = BerEncoding.GetRelativeOidLength(Value);
         var implicitLength = valueLength + BerEncoding.GetHeaderLength(tagImplicit, valueLength);

         BerEncoding.EncodeTag(output, Tag.ToContainer());
         BerEncoding.EncodeLength(output, implicitLength);

         BerEncoding.EncodeTag(output, tagImplicit);
         BerEncoding.EncodeLength(output, valueLength);

         Encoded = output.ToArray();

         EncodedLength = valueLength + Encoded.Length;
         return EncodedLength;
      }
   }
}
