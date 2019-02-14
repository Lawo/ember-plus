/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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

        void OnMessageReceived(MessageReceivedArgs oArgs, bool reset)
        {
            if (MessageReceived != null)
                MessageReceived(this, oArgs);

            if (reset)
            {
                _isInFrame = false;
                _stream.SetLength(0);
                _usesNonEscapingFraming = false;
                _outOfFrameByteCount = 0;
                _payloadLengthLength = -1;
                _payloadLength = 0;
                _isDataLinkEscaped = false;
                _crc = Crc.InitialValue;
            }
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
            if (_isInFrame == false)
            {
                if (b == Constants.Bof)
                {
                    if (_outOfFrameByteCount > 0 && framingErrorCallback != null)
                        framingErrorCallback(string.Format("S101: {0} out of frame data bytes!", _outOfFrameByteCount));

                    _isInFrame = true;
                    _stream.SetLength(0);
                    _usesNonEscapingFraming = false;
                    _outOfFrameByteCount = 0;
                    _payloadLengthLength = -1;
                    _payloadLength = 0;
                    _isDataLinkEscaped = false;
                    _crc = Crc.InitialValue;
                }
                else if (b == Constants.Invalid)
                {
                    if (_outOfFrameByteCount > 0 && framingErrorCallback != null)
                        framingErrorCallback(string.Format("S101: {0} out of frame data bytes!", _outOfFrameByteCount));

                    _isInFrame = true;
                    _stream.SetLength(0);
                    _usesNonEscapingFraming = true;
                    _outOfFrameByteCount = 0;
                    _payloadLengthLength = -1;
                    _payloadLength = 0;
                    _isDataLinkEscaped = false;
                    _crc = Crc.InitialValue;
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
        int _payloadLengthLength;
        bool _isInFrame;
        bool _usesNonEscapingFraming;

        void ReceiveByteWithoutEscaping(byte b, MessageCallback framingErrorCallback)
        {
            if (_payloadLengthLength == -1)
            {
                _payloadLengthLength = b;

                if (_payloadLengthLength == 0)
                {
                    OnMessageReceived(new MessageReceivedArgs(0, new byte[0], true), true);
                }

                return;
            }

            if (_payloadLengthLength > 0)
            {
                _payloadLength <<= 8;
                _payloadLength |= b;
                _payloadLengthLength--;
            }
            else
            {
                _stream.WriteByte(b);

                if (_stream.Length == _payloadLength)
                {
                    var memory = _stream.ToArray();

                    OnMessageReceived(new MessageReceivedArgs(memory.Length, memory, true), true);
                }

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
                        OnMessageReceived(new MessageReceivedArgs(memory.Length, memory, false), true);
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
            if (_stream != null)
            {
                try
                {
                    _stream.Dispose();
                }
                catch (ObjectDisposedException)
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
