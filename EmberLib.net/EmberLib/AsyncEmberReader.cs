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
using BerLib;

namespace EmberLib
{
   /// <summary>
   /// Base class for asynchronous reader implementations, which can be fed
   /// BER encoded data through one of the ReadBytes() methods and which
   /// calls a virtual method everytime a TLV has been decoded.
   /// </summary>
   public abstract class AsyncEmberReader : BerReaderBase
   {
      /// <summary>
      /// Creates a new instance of AyncEmberReader.
      /// </summary>
      public AsyncEmberReader()
      {
         ResetState(DecodeState.Tag);
      }

      /// <summary>
      /// Gets a value indicating whether the reader is ready to receive
      /// a new Ember tree or not.
      /// Returns false if the last received Ember data did not close the Ember tree.
      /// </summary>
      public bool IsEmpty
      {
         get
         {
            if(_currentContainer == null
            && _containerStack.Count == 0
            && OuterLength == 0
            && Tag == BerTag.Zero
            && _decodeState == DecodeState.Tag)
               return true;

            return false;
         }
      }

      /// <summary>
      /// Feeds a single input byte into the reader. If this byte completes
      /// a TLV, the virtual method OnItemReady() is called.
      /// If the byte begins a new container, the virtual method OnNewContainer()
      /// is called.
      /// </summary>
      /// <param name="b">The input byte to process.</param>
      public virtual void ReadByte(byte b)
      {
         _buffer.Add(b);

         if(_currentContainer != null)
            _currentContainer.BytesRead++;

         bool isEofOk = false;

         switch(_decodeState)
         {
            case DecodeState.Tag:
               isEofOk = ReadByte_Tag(b);
               break;

            case DecodeState.Length:
               isEofOk = ReadByte_Length(b);
               break;

            case DecodeState.Value:
               isEofOk = ReadByte_Value(b);
               break;

            case DecodeState.Terminator:
               isEofOk = ReadByte_Terminator(b);
               break;
         }

         while(_currentContainer != null
            && _currentContainer.IsEof)
         {
            if(isEofOk == false)
               ThrowError(106, "Unexpected end of container");

            EndContainer();
         }
      }

      /// <summary>
      /// Feeds multiple bytes of input data into the reader. Everytime a complete
      /// TLV has been decoded, the virtual method OnItemReady() is called.
      /// Everytime a new container has been opened, the virtual method OnNewContainer()
      /// is called.
      /// </summary>
      /// <param name="bytes">The array to feed bytes from.</param>
      /// <param name="offset">The index of the first byte in <paramref name="bytes"/> to feed.</param>
      /// <param name="length">The number of bytes in <paramref name="bytes"/> to feed.</param>
      public void ReadBytes(byte[] bytes, int offset, int length)
      {
         if (length == 0) return;
         length += offset;

         if(bytes == null)
            throw new ArgumentNullException("bytes");

         if(offset < 0 || offset >= bytes.Length)
            throw new ArgumentOutOfRangeException("offset");

         if(length < 0 || length > bytes.Length)
            throw new ArgumentOutOfRangeException("length");

         for(int i = offset; i < length; i++)
            ReadByte(bytes[i]);
      }

      /// <summary>
      /// Feeds multiple bytes of input data into the reader. Everytime a complete
      /// TLV has been decoded, the virtual method OnItemReady() is called.
      /// Everytime a new container has been opened, the virtual method OnNewContainer()
      /// is called.
      /// </summary>
      /// <param name="bytes">The collection of bytes to feed.</param>
      public void ReadBytes(IEnumerable<byte> bytes)
      {
         if(bytes == null)
            throw new ArgumentNullException("bytes");

         foreach(var b in bytes)
            ReadByte(b);
      }

      /// <summary>
      /// Resets the reader state.
      /// </summary>
      public virtual void Reset()
      {
         _containerStack.Clear();
         _currentContainer = null;

         DisposeCurrentTlv();

         ResetState(DecodeState.Tag);
      }

      /// <summary>
      /// Returns a string describing the current reader position.
      /// </summary>
      /// <returns>A string containing the current Ember tree position.</returns>
      public string GetPositionString()
      {
         var buffer = new StringBuilder();

         foreach(var container in _containerStack)
            buffer.AppendFormat("{0}:{1}/", container.Tag, BerDefinitions.GetTypeName(container.Type));

         return String.Format("{0}{1}:{2}", buffer.ToString(), Tag, BerDefinitions.GetTypeName(Type));
      }

      /// <summary>
      /// Called when the opening tag and length of a new container have been
      /// read.
      /// The opened container can be examined through the Tag, Length and Type properties.
      /// </summary>
      protected abstract void OnNewContainer();

      /// <summary>
      /// Called when a TLV (either container or primitive) has been decoded.
      /// The read TLV can be examined through the Tag, Length, Type and IsContainer properties.
      /// </summary>
      protected abstract void OnItemReady();

      /// <summary>
      /// Overriden to provide a significant error message.
      /// </summary>
      protected override void ThrowError(int errorCode, string message)
      {
         message = String.Format("@{0}: {1}", GetPositionString(), message);

         base.ThrowError(errorCode, message);
      }

      #region Implementation
      DecodeState _decodeState;
      int _bytesRead;
      int _bytesExpected;
      List<byte> _buffer = new List<byte>();
      AsyncContainer _currentContainer;
      ContainerStack _containerStack = new ContainerStack();

      bool ReadByte_Tag(byte b)
      {
         if(b == 0 && _bytesRead == 0)
         {
            ResetState(DecodeState.Terminator);
            return false;
         }

         if(_bytesRead > 12)
            ThrowError(103, "Number of tag octets out of bounds");

         if(_bytesRead == 0 && (b & 0x1F) != 0x1F
         || _bytesRead > 0 && (b & 0x80) == 0)
         {
            var input = new BerMemoryInput(_buffer);

            if(Tag.IsZero)
            {
               // no outer tag read yet -> read outer tag
               var tag = BerEncoding.DecodeTag(input);

               if(tag.Class == BerClass.Universal)
                  ThrowError(107, "Universal outer tag encountered");

               if(tag.IsContainer == false)
                  ThrowError(108, "Primitive outer tag encountered");

               Tag = tag.ToPrimitive(); // clear container flag
            }
            else
            {
               // outer tag already read -> read inner tag (type tag)
               var typeTag = BerEncoding.DecodeTag(input);
               var type = typeTag.NumberAsType;

               if(BerType.IsApplicationDefined(type) == false)
               {
                  if(typeTag.Class != BerClass.Universal)
                     ThrowError(110, "Non-universal inner tag encountered");

                  if(type == 0 || type >= BerType.LastUniversal)
                     ThrowError(109, "Invalid BER type encountered");
               }

               IsContainer = typeTag.IsContainer;
               Type = type;
            }

            ResetState(DecodeState.Length);
            return false;
         }

         _bytesRead++;
         return false;
      }

      bool ReadByte_Length(byte b)
      {
         if(_bytesExpected == 0)
         {
            if((b & 0x80) != 0)
               _bytesExpected = (b & 0x7F) + 1;
            else
               _bytesExpected = 1;

            if(_bytesExpected > 5)
               ThrowError(104, "Number of length octets out of bounds");
         }

         _bytesRead++;

         if(_bytesRead == _bytesExpected)
         {
            var input = new BerMemoryInput(_buffer);

            if(Type == 0)
            {
               OuterLength = BerEncoding.DecodeLength(input);

               if(OuterLength == 0)
                  ThrowError(102, "Zero outer length encountered");

               ResetState(DecodeState.Tag);
            }
            else
            {
               Length = BerEncoding.DecodeLength(input);
               var isEofOK = Length == 0;

               if(IsContainer)
               {
                  ResetState(DecodeState.Tag);

                  OnNewContainer();
                  PushContainer();

                  DisposeCurrentTlv();
                  return isEofOK;
               }

               if(Length == 0)
               {
                  OnValueReady();
               }
               else
               {
                  ResetState(DecodeState.Value);
                  _buffer.Capacity = (int)Length;
               }

               return isEofOK;
            }
         }

         return false;
      }

      bool ReadByte_Value(byte b)
      {
         if(_bytesRead == 0)
            _bytesExpected = Length;

         _bytesRead++;

         if(_bytesRead == _bytesExpected)
         {
            Debug.Assert(_bytesRead == Length);
            Debug.Assert(_buffer.Count == _bytesRead);

            OnValueReady();
            return true;
         }

         return false;
      }

      bool ReadByte_Terminator(byte b)
      {
         if(_currentContainer == null
         || _currentContainer.Length != BerDefinitions.IndefiniteLength)
            ThrowError(105, "Unexpected terminator");

         if(b == 0)
         {
            _bytesRead++;

            if(_bytesRead == 3)
            {
               // end of indefinite length container
               if(_currentContainer != null)
                  _currentContainer.Length = _currentContainer.BytesRead;

               return true;
            }
         }
         else
         {
            ThrowError(101, "Non-zero byte in terminator");
         }

         return false;
      }

      void ResetState(DecodeState state)
      {
         _buffer.Clear();

         _decodeState = state;
         _bytesRead = 0;
         _bytesExpected = 0;
      }

      void OnValueReady()
      {
         Debug.Assert(Value == null);

         if(Length > 0)
         {
            Value = new byte[_bytesRead];
            _buffer.CopyTo(0, Value, 0, (int)_bytesRead);
         }

         ResetState(DecodeState.Tag);

         OnItemReady();

         DisposeCurrentTlv();
      }
         
      void PushContainer()
      {
         _currentContainer = new AsyncContainer(Tag, Type, Length);

         _containerStack.Push(_currentContainer);
      }

      bool EndContainer()
      {
         ResetState(DecodeState.Tag);

         if(_containerStack.Count != 0)
         {
            var container = _containerStack.Pop();

            Debug.Assert(container == _currentContainer);

            Tag = container.Tag;
            Type = container.Type;
            Length = container.Length;
            IsContainer = true;

            OnItemReady();

            if(_containerStack.Count != 0)
            {
               _currentContainer = _containerStack.Peek();
               _currentContainer.BytesRead += Length;
            }
            else
            {
               _currentContainer = null;
            }

            DisposeCurrentTlv();
            return true;
         }

         return false;
      }
      #endregion

      #region Nested Types
      class AsyncContainer
      {
         public readonly BerTag Tag;
         public readonly uint Type;
         public int Length;
         public int BytesRead;

         public bool IsEof
         {
            get
            {
               return BytesRead >= Length
                   && Length != BerDefinitions.IndefiniteLength;
            }
         }

         public AsyncContainer(BerTag tag, uint type, int length)
         {
            Tag = tag;
            Type = type;
            Length = length;
            BytesRead = 0;
         }
      }

      enum DecodeState
      {
         Tag,
         Length,
         Value,
         Terminator,
      }

      class ContainerStack : LinkedList<AsyncContainer>
      {
         public void Push(AsyncContainer value)
         {
            base.AddLast(value);
         }

         public AsyncContainer Pop()
         {
            var value = base.Last.Value;
            base.RemoveLast();
            return value;
         }

         public AsyncContainer Peek()
         {
            return base.Last.Value;
         }
      }
      #endregion
   }
}
