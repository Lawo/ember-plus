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

namespace EmberLib.Framing
{
   /// <summary>
   /// Reads partitioned ember data framed with the S101 protocol.
   /// You can successively call ReadByte() or ReadBytes(), passing
   /// the received S101 message bytes.
   /// Everytime a complete S101 message has been read, the PackageReceived
   /// event is raised.
   /// As soon as a complete ember tree has been received, the BaseReader
   /// (of type EmberLib.AsyncEmberReader) will raise the RootReady event.
   /// </summary>
   public class FramingReader : IDisposable
   {
      /// <summary>
      /// Constructs a new instance of FramingReader.
      /// </summary>
      /// <param name="baseReader">The EmberReader used to decode the ember data.</param>
      /// <param name="keepAliveCallback">A callback function that is subscribed to
      /// the KeepAliveRequestReceived event.</param>
      public FramingReader(AsyncEmberReader baseReader, EventHandler<KeepAliveRequestReceivedArgs> keepAliveCallback)
      {
         if(baseReader == null)
            throw new ArgumentNullException("baseReader");

         BaseReader = baseReader;

         if(keepAliveCallback != null)
            KeepAliveRequestReceived += keepAliveCallback;

         _rxFrame = new S101.RxFrame();
         _rxFrame.MessageReceived += RxFrame_MessageReceived;
      }

      /// <summary>
      /// Gets the EmberReader that is used to decode incoming ember data.
      /// As soon as a complete ember tree has been received, the BaseReader
      /// will raise the RootReady event.
      /// </summary>
      public AsyncEmberReader BaseReader { get; private set; }

      #region PackageReceived Event
      public class PackageReceivedArgs : EventArgs
      {
         public int SlotId { get; private set; }
         public byte MessageId { get; private set; }
         public byte Command { get; private set; }
         public Dtd? Dtd { get; private set; }
         public bool IsFirstPackage { get; private set; }
         public bool IsLastPackage { get; private set; }
         public bool IsEmptyPackage { get; private set; }
         public byte[] ApplicationBytes { get; private set; }

         public PackageReceivedArgs(int slotId, byte messageId, byte command, Dtd? dtd, bool isFirst, bool isLast, bool isEmpty, byte[] appBytes)
         {
            SlotId = slotId;
            MessageId = messageId;
            Command = command;
            Dtd = dtd;
            IsFirstPackage = isFirst;
            IsLastPackage = isLast;
            IsEmptyPackage = isEmpty;
            ApplicationBytes = appBytes;
         }
      }

      /// <summary>
      /// Raised everytime a package with valid framing has been received.
      /// This event is raised before the specialized events KeepAliveRequestReceived
      /// and KeepAliveResponse are raised and before any payload contained in
      /// the package is decoded by the BaseReader.
      /// This event is intended for advanced purposes - it is recommended to
      /// use the specialized events.
      /// </summary>
      public event EventHandler<PackageReceivedArgs> PackageReceived;

      protected virtual void OnPackageReceived(PackageReceivedArgs oArgs)
      {
         if(this.PackageReceived != null)
            PackageReceived(this, oArgs);
      }
      #endregion

      #region KeepAliveRequestReceived Event
      public class KeepAliveRequestReceivedArgs : EventArgs
      {
         public byte[] Response { get; private set; }
         public int ResponseLength { get; private set; }

         public KeepAliveRequestReceivedArgs(byte[] responseBytes, int responseLength)
         {
            Response = responseBytes;
            ResponseLength = responseLength;
         }
      }

      /// <summary>
      /// Raised when a Keep-Alive request has been decoded. The subscriber
      /// has to write the response passed in the KeepAliveRequestReceivedArgs
      /// to the remote host when handling this event.
      /// </summary>
      public event EventHandler<KeepAliveRequestReceivedArgs> KeepAliveRequestReceived;

      protected virtual void OnKeepAliveRequestReceived(KeepAliveRequestReceivedArgs e)
      {
         if(KeepAliveRequestReceived != null)
            KeepAliveRequestReceived(this, e);
      }
      #endregion

      #region KeepAliveResponseReceived Event
      /// <summary>
      /// Raised when a Keep-Alive response has been decoded. The subscriber
      /// may use this event when actively issueing Keep-Alive requests to
      /// the remote host.
      /// </summary>
      public event EventHandler KeepAliveResponseReceived;

      protected virtual void OnKeepAliveResponseReceived()
      {
         if(KeepAliveResponseReceived != null)
            KeepAliveResponseReceived(this, EventArgs.Empty);
      }
      #endregion

      #region FramingError Event
      public class FramingErrorArgs : EventArgs
      {
         public string Message { get; private set; }

         public FramingErrorArgs(string message)
         {
            Message = message;
         }
      }

      public event EventHandler<FramingErrorArgs> FramingError;

      protected virtual void OnFramingError(FramingErrorArgs e)
      {
         if(FramingError != null)
            FramingError(this, e);
      }
      #endregion

      /// <summary>
      /// Byte-wise reads an S101 message, unframes it and decodes the contained ember data.
      /// Everytime a complete S101 message has been read, the PackageReceived
      /// event is raised.
      /// As soon as a complete ember tree has been received, the BaseReader
      /// will raise the RootReady event.
      /// </summary>
      /// <param name="b">The byte to read.</param>
      public void ReadByte(byte b)
      {
         _rxFrame.ReceiveByte(b, message => OnFramingError(new FramingErrorArgs(message)));
      }

      /// <summary>
      /// Reads (part of) an S101 message, unframes it and decodes the contained ember data.
      /// Everytime a complete S101 message has been read, the PackageReceived
      /// event is raised.
      /// As soon as a complete ember tree has been received, the BaseReader
      /// will raise the RootReady event.
      /// </summary>
      /// <param name="bytes">Bytes containing either a part of or a complete received S101 message.</param>
      /// <param name="offset">Offset of the first byte in <paramref name="bytes"/> to decode.</param>
      /// <param name="length">Number of bytes in <paramref name="bytes"/> to decode.</param>
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
      /// Reads (part of) an S101 message, unframes it and decodes the contained ember data.
      /// Everytime a complete S101 message has been read, the PackageReceived
      /// event is raised.
      /// As soon as a complete ember tree has been received, the BaseReader
      /// will raise the RootReady event.
      /// </summary>
      /// <param name="bytes">Bytes containing either a part of or a complete received S101 message.</param>
      public void ReadBytes(IEnumerable<byte> bytes)
      {
         if(bytes == null)
            throw new ArgumentNullException("bytes");

         foreach(var b in bytes)
            ReadByte(b);
      }

      /// <summary>
      /// Dispose of allocated resources.
      /// </summary>
      /// <param name="disposing">If true, dispose of unmanaged resources
      /// otherwise only dispose of managed resources</param>
      protected virtual void Dispose(bool disposing)
      {
         if(_rxFrame != null)
            _rxFrame.Dispose();

         _rxFrame = null;
      }

      #region Implementation
      S101.RxFrame _rxFrame;

      void RxFrame_MessageReceived(object sender, S101.RxFrame.MessageReceivedArgs e)
      {
         if(e.Length >= 2)
         {
            var data = e.Data;
            var messageId = data[1];
            var command = data[2];

            if(messageId == ProtocolParameters.MessageId)
            {
               if(command == ProtocolParameters.Commands.Payload)
               {
                  var baseReader = BaseReader;
                  var appBytesCount = data[6];
                  var emberOffset = 7 + appBytesCount;

                  var flags = data[4];
                  var isFirst = (flags & ProtocolParameters.FirstPackageFlag) != 0;
                  var isLast = (flags & ProtocolParameters.LastPackageFlag) != 0;
                  var isEmpty = (flags & ProtocolParameters.EmptyPackageFlag) != 0;

                  var dtd = (Dtd)data[5];
                  var appBytes = null as byte[];

                  if(appBytesCount != 0)
                  {
                     appBytes = new byte[appBytesCount];
                     Array.Copy(data, 7, appBytes, 0, appBytesCount);
                  }

                  if(isFirst)
                  {
                     if(baseReader.IsEmpty == false)
                     {
                        var message = String.Format("Incomplete Ember tree detected! Current position: {0}", baseReader.GetPositionString());
                        OnFramingError(new FramingErrorArgs(message));
                        baseReader.Reset();
                     }
                  }

                  OnPackageReceived(new PackageReceivedArgs(data[0], messageId, command, dtd, isFirst, isLast, isEmpty, appBytes));
                  baseReader.ReadBytes(e.Data, emberOffset, e.Length - emberOffset);
               }
               else if(command == ProtocolParameters.Commands.KeepAliveRequest)
               {
                  OnPackageReceived(new PackageReceivedArgs(data[0], messageId, command, null, false, false, false, null));

                  int length;
                  var bytes = FramingBerOutput.WriteKeepAliveResponse(data[0], out length);
                  OnKeepAliveRequestReceived(new KeepAliveRequestReceivedArgs(bytes, length));
               }
               else if(command == ProtocolParameters.Commands.KeepAliveResponse)
               {
                  OnPackageReceived(new PackageReceivedArgs(data[0], messageId, command, null, false, false, false, null));
                  OnKeepAliveResponseReceived();
               }
            }
            else
            {
               OnPackageReceived(new PackageReceivedArgs(data[0], messageId, command, null, false, false, false, null));
            }
         }
      }
      #endregion

      #region IDisposable Members
      public void Dispose()
      {
         Dispose(true);
      }
      #endregion
   }
}
