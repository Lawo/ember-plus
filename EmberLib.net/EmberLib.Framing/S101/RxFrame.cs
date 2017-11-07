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
using System.IO;

namespace EmberLib.Framing.S101
{
   /// <summary>
   /// Used to receive and decode S101 messages. This class supports the traditional
   /// S101 framing which uses the escaping and the non-escaping variant.
   /// For consumer that want to determine the framing being used, see <see cref="MessageReceivedArgs.IsNonEscapingMessage"/>
   /// property can be used.
   /// </summary>
   public sealed class RxFrame : IDisposable
   {
      #region MessageReceived Event
      public class MessageReceivedArgs : EventArgs
      {
         public readonly int Length;
         public readonly byte[] Data;
         public readonly bool IsNonEscapingMessage;

         public MessageReceivedArgs(int length, byte[] data, bool isNonEscapingMessage)
         {
            Length = length;
            Data = data;
            IsNonEscapingMessage = isNonEscapingMessage;
         }
      }

      public event EventHandler<MessageReceivedArgs> MessageReceived;

      void OnMessageReceived(MessageReceivedArgs oArgs)
      {
         if(MessageReceived != null)
            MessageReceived(this, oArgs);
      }
      #endregion

      /// <summary>
      /// This should be called by the connection engine for every byte that has
      /// been received. ReceiveByte raises the MessageReceived event when a complete message
      /// has been received.
      /// </summary>
      /// <param name="b">received byte to process</param>
      /// <param name="framingErrorCallback">callback function that is invoked everytime a framing error is encountered</param>
      public void ReceiveByte(byte b, MessageCallback framingErrorCallback)
      {
         if(_isInFrame == false)
         {
            if(b == Constants.Bof)
            {
               if (_outOfFrameByteCount > 0 && framingErrorCallback != null)
                  framingErrorCallback(string.Format("S101: {0} out of frame data bytes!", _outOfFrameByteCount));

               _isInFrame = true;
               _stream.SetLength(0);
               _usesNonEscapingFraming = false;
               _outOfFrameByteCount = 0;
               _payloadLength = 0;
               _isDataLinkEscaped = false;
               _crc = Crc.InitialValue;
            }
            else if(b == Constants.Invalid)
            {
               if (_outOfFrameByteCount > 0 && framingErrorCallback != null)
                  framingErrorCallback(string.Format("S101: {0} out of frame data bytes!", _outOfFrameByteCount));

               _isInFrame = true;
               _stream.SetLength(0);
               _usesNonEscapingFraming = true;
               _outOfFrameByteCount = 0;
               _payloadLength = 0;
            }
            else
            {
               _outOfFrameByteCount++;
            }

            return;
         }

         if (_usesNonEscapingFraming)
         {
            ReceiveByteWithoutEscaping(b, framingErrorCallback);
         }
         else
         {
            ReceiveByteWithEscaping(b, framingErrorCallback);
         }
      }

      /// <summary>
      /// This should be called by the connection engine for every byte that has
      /// been received. ReceiveByte raises the MessageReceived event when a complete message
      /// has been received.
      /// </summary>
      /// <param name="b">received byte to process</param>
      public void ReceiveByte(byte b)
      {
         ReceiveByte(b, null);
      }

      #region Implementation
      MemoryStream _stream = new MemoryStream();
      bool _isDataLinkEscaped = false;
      ushort _crc;
      int _outOfFrameByteCount;
      int _payloadLength;
      bool _isInFrame;
      bool _usesNonEscapingFraming;

      void ReceiveByteWithoutEscaping(byte b, MessageCallback framingErrorCallback)
      {
         _stream.WriteByte(b);

         switch (_stream.Length)
         {
            case 1:
               _payloadLength = (b << 24);
               break;

            case 2:
               _payloadLength |= (b << 16);
               break;

            case 3:
               _payloadLength |= (b << 8);
               break;

            case 4:
               _payloadLength |= b;
               break;
         }

         if (_stream.Length >= 4 && _stream.Length - 4 == _payloadLength)
         {
            var memory = _stream.ToArray();

            OnMessageReceived(new MessageReceivedArgs(memory.Length, memory, true));

            _isInFrame = false;
            _stream.SetLength(0);
            _usesNonEscapingFraming = false;
            _payloadLength = 0;
         }
      }

      void ReceiveByteWithEscaping(byte b, MessageCallback framingErrorCallback)
      {
         if (b == Constants.Bof)
         {
            if (framingErrorCallback != null)
               framingErrorCallback("S101: BOF in frame!");

            _stream.SetLength(0);
            _isDataLinkEscaped = false;
            _crc = Crc.InitialValue;

            if (_outOfFrameByteCount > 0 && framingErrorCallback != null)
               framingErrorCallback(String.Format("S101: {0} out of frame data bytes!", _outOfFrameByteCount));

            _outOfFrameByteCount = 0;
            return;
         }

         if (b == Constants.Eof)
         {
            var length = (int)_stream.Length;
            _stream.Write(Constants.Eofs, 0, Constants.Eofs.Length);

            if (length >= 3)
            {
               if (_crc == 0xF0B8)
               {
                  _stream.SetLength(length - 2);
                  var memory = _stream.ToArray();
                  OnMessageReceived(new MessageReceivedArgs(memory.Length, memory, false));
               }
               else
               {
                  if (framingErrorCallback != null)
                     framingErrorCallback("S101: CRC error!");
               }
            }
            else if (length == 0)
            {
               if (framingErrorCallback != null)
                  framingErrorCallback("S101: EOF out of frame!");
            }

            _isInFrame = false;
            _stream.SetLength(0);
            return;
         }

         if (b == Constants.Ce)
         {
            _isDataLinkEscaped = true;
            return;
         }

         if (b >= Constants.Invalid)
         {
            if (framingErrorCallback != null)
               framingErrorCallback("S101: Invalid character received!");

            _isInFrame = false;
            _stream.SetLength(0);
            return;
         }

         if (_isDataLinkEscaped)
         {
            _isDataLinkEscaped = false;
            b ^= 0x20;
         }

         _stream.WriteByte(b);
         _crc = Crc.CrcCCITT16(_crc, b);
      }
      #endregion

      #region IDisposable Members
      public void Dispose()
      {
         if(_stream != null)
         {
            try
            {
               _stream.Dispose();
            }
            catch(ObjectDisposedException)
            {
            }

            _stream = null;
         }

         _crc = 0;
         _isDataLinkEscaped = false;
      }
      #endregion
   }

   /// <summary>
   /// Simple string-passing callback delegate.
   /// </summary>
   public delegate void MessageCallback(string message);
}
