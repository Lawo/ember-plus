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
   /// A simple, synchronous Ember reader which can also
   /// act as an IBerInput itself, passing through read bytes from
   /// its own input.
   /// <see cref="EmberLib.EmberNode.Decode" /> uses this reader.
   /// </summary>
   public sealed class EmberReader : BerReaderBase, IBerInput
   {
      /// <summary>
      /// Creates a new EmberReader instance configured to read BER encoded data
      /// from <paramref name="input"/>.
      /// </summary>
      /// <param name="input">The input to read BER encoded data from.</param>
      public EmberReader(IBerInput input)
      {
         Debug.Assert(input != null);

         _input = input;
      }

      /// <summary>
      /// Creates a nested reader from another EmberReader instance.
      /// The passed <paramref name="parentReader"/> must be positioned
      /// on a container.
      /// The newly created nested reader will signal Eof when all bytes
      /// of this container have been read.
      /// </summary>
      /// <param name="parentReader">The base reader.</param>
      public EmberReader(EmberReader parentReader)
      {
         if(parentReader == null)
            throw new ArgumentNullException("parentReader");

         if(parentReader.IsContainer == false)
            throw new ArgumentException("parentReader is not positioned on a container");

         _input = parentReader._input;
         _parentReader = parentReader;

         _bytesAvailable = parentReader.Length;
      }

      /// <summary>
      /// Gets a value indicating wether all available bytes have been read.
      /// </summary>
      public bool Eof
      {
         get
         {
            return _bytesAvailable != BerDefinitions.IndefiniteLength
                && _bytesRead >= _bytesAvailable;
         }
      }

      /// <summary>
      /// Read the next TLTLV and publishes information like <see cref="BerLib.BerReaderBase.Tag" />,
      /// <see cref="BerLib.BerReaderBase.Length" />, <see cref="BerLib.BerReaderBase.Type" /> and
      /// <see cref="BerLib.BerReaderBase.IsContainer" />.
      /// Use one of the Get() functions of BerLib.BerReaderBase to retrieve the Value.
      /// </summary>
      /// <returns>True if a valid TLTLV has been read, otherwise false.</returns>
      public bool Read()
      {
         Debug.Assert(_input != null);
         var typeTag = BerTag.Zero;

         DisposeCurrentTlv();

         if(_parentReader != null // non top-level read?
         && this.Eof)
         {
            HandleEof();
            return false;
         }

         Tag = BerEncoding.DecodeTag(this);

         if(Tag.IsZero)
         {
            // terminator of indefinite length field: 4 zeros (outer length AND inner length must be indefinite
            if(ReadByte() == 0
            && ReadByte() == 0
            && ReadByte() == 0)
            {
               HandleEof();
               return false;
            }
            else
            {
               ThrowError(10, "Invalid zero tag: 0x00");
            }
         }

         OuterLength = BerEncoding.DecodeLength(this);

         if(OuterLength == 0)
            ThrowError(13, "Zero outer length encountered");

         if(Tag.IsContainer
         && Tag.Class != BerClass.Universal)
         {
            typeTag = BerEncoding.DecodeTag(this);
            Length = BerEncoding.DecodeLength(this);

            if((Length == BerDefinitions.IndefiniteLength) != (OuterLength == BerDefinitions.IndefiniteLength))
               ThrowError(12, "Outer and inner tag must use the same length form");
         }
         else
         {
            ThrowError(14, "Implicit tag or universal outer tag found");
         }

         IsContainer = typeTag.IsContainer;
         Tag = Tag.ToPrimitive(); // clear container flag
         Type = typeTag.NumberAsType;

         if(Length == BerDefinitions.IndefiniteLength
         && IsContainer == false)
            ThrowError(11, "Indefinite length form is only allowed on containers");

         if(IsContainer == false
         && Length > 0)
         {
            var value = new byte[Length];

            for(int index = 0; index < Length; index++)
               value[index] = ReadByte();

            Value = value;
         }

         return true;
      }

      /// <summary>
      /// Overriden to provide an information-rich error message when
      /// encountering a decoding error.
      /// </summary>
      protected override void ThrowError(int errorCode, string message)
      {
         var containerTags = new List<BerTag>();
         var currentReader = this;

         for( ; currentReader != null; currentReader = currentReader._parentReader)
            containerTags.Add(currentReader.Tag);

         var tagsBuffer = new StringBuilder();

         for(int index = containerTags.Count - 1; index >= 0; index--)
            tagsBuffer.AppendFormat(index > 0 ? "{0}/" : "{0}", containerTags[index]);

         base.ThrowError(errorCode, "@" + tagsBuffer.ToString() + ": " + message);
      }

      #region Implementation
      IBerInput _input;
      int _bytesRead;
      int _bytesAvailable;
      EmberReader _parentReader;
      bool _isEofHandled;

      byte ReadByte()
      {
         _bytesRead++;

         return _input.ReadByte();
      }

      void HandleEof()
      {
         if(_isEofHandled == false)
         {
            if(_parentReader != null)
               _parentReader._bytesRead += _bytesRead;

            _isEofHandled = true;
         }
      }
      #endregion

      #region IBerInput Members
      byte IBerInput.ReadByte()
      {
         return ReadByte();
      }
      #endregion
   }
}
