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
using System.Diagnostics.CodeAnalysis;
using BerLib;

namespace EmberLib.Framing
{
   /// <summary>
   /// Implementation of BerLib.IBerOutput, framing the written data with the S101 protocol and
   /// partitioning data into packages of MaximumPackageLength size.
   /// Raises the PackageReady event everytime a package is ready for transmit.
   /// </summary>
   public class FramingBerOutput : IBerOutput, IDisposable
   {
      /// <summary>
      /// Constructs a new instance of FramingBerOutput.
      /// </summary>
      /// <param name="maximumPackageLength">The maximum length of a package.
      /// Must be greater than ProtocolParameters.MinimumHeaderLength and less than or equal to
      /// ProtocolParameters.MaximumPackageLength.</param>
      /// <param name="slotId">The S101 slot id</param>
      /// <param name="dtd">The dtd of the ember data to frame</param>
      /// <param name="packageReadyHandler">A callback that is attached to the PackageReady event. May be null.</param>
      public FramingBerOutput(int maximumPackageLength, int slotId, Dtd dtd, EventHandler<PackageReadyArgs> packageReadyHandler)
      {
         if(maximumPackageLength <= ProtocolParameters.MinimumHeaderLength)
            throw new ArgumentException("maximumPackageLength must be greater than ProtocolParameters.MinimumHeaderLength!");
         if(maximumPackageLength > ProtocolParameters.MaximumPackageLength)
            throw new ArgumentException("maximumPackageLength must not be greater than ProtocolParameters.MaxPackageLength!");

         MaximumPackageLength = maximumPackageLength;
         SlotId = slotId;
         Dtd = dtd;

         if(packageReadyHandler != null)
            PackageReady += packageReadyHandler;

         _isFirstPackage = true;
      }

      // suppress "Implement IDisposable correctly"
      // because implementation is correct, warning is
      // raised because of additional method call to AssertFinished().
      [SuppressMessage("Microsoft.Design", "CA1063")]
      ~FramingBerOutput()
      {
         AssertFinished();

         Dispose(false);
      }

      /// <summary>
      /// Gets the maximum payload length per package.
      /// </summary>
      public int MaximumPackageLength { get; private set; }
      
      /// <summary>
      /// Gets the S101 slot id to address.
      /// </summary>
      public int SlotId { get; private set; }

      /// <summary>
      /// Gets the dtd identifier of the ember data to frame.
      /// </summary>
      public Dtd Dtd { get; private set; }

      #region PackageReady Event
      public class PackageReadyArgs : EventArgs
      {
         /// <summary>
         /// Gets the memory junk containing the framed package.
         /// </summary>
         public byte[] FramedPackage { get; private set; }
         
         /// <summary>
         /// Gets the number of valid bytes in the FramedPackage.
         /// </summary>
         public int FramedPackageLength { get; private set; }

         /// <summary>
         /// Gets the length of the ember payload.
         /// </summary>
         public int UnframedPackageLength { get; private set; }

         public PackageReadyArgs(byte[] package, int framedPackageLength, int unframedPackageLength)
         {
            FramedPackage = package;
            FramedPackageLength = framedPackageLength;
            UnframedPackageLength = unframedPackageLength;
         }
      }

      /// <summary>
      /// This event is raised everytime a package is ready for transmit.
      /// </summary>
      public event EventHandler<PackageReadyArgs> PackageReady;

      protected virtual void OnPackageReady(PackageReadyArgs oArgs)
      {
         if(this.PackageReady != null)
            PackageReady(this, oArgs);
      }
      #endregion

      /// <summary>
      /// Frames the last package and raises the PackageReady event.
      /// This must be called when all ember data has been written to
      /// the BerFramingOutput.
      /// </summary>
      public void Finish()
      {
         OnPackageReady(isLastPackage: true);
      }

      /// <summary>
      /// Writes multiple bytes of ember data to the output, raising the PackageReady event
      /// when necessary.
      /// </summary>
      /// <param name="bytes">The array of bytes to write. Must not be null.</param>
      /// <param name="offset">Offset of the first byte to write in <paramref name="bytes" />.</param>
      /// <param name="length">Number of bytes to write from <paramref name="bytes"/>.</param>
      public void WriteBytes(byte[] bytes, int offset, int length)
      {
         length += offset;

         if(bytes == null)
            throw new ArgumentNullException("bytes");

         if(offset < 0 || offset >= bytes.Length)
            throw new ArgumentOutOfRangeException("offset");

         if(length < 0 || length > bytes.Length)
            throw new ArgumentOutOfRangeException("length");

         for(int i = offset; i < length; i++)
            WriteByte(bytes[i]);
      }

      /// <summary>
      /// Frames a "Keep-Alive Request" message into a byte array.
      /// </summary>
      /// <param name="slotId">The S101 slot id to address.</param>
      /// <param name="length">Receives the length of the framed package.</param>
      /// <returns>A byte array containing the framed package.</returns>
      public static byte[] WriteKeepAliveRequest(int slotId, out int length)
      {
         var bytes = new byte[]
         {
            (byte)(slotId & 0xFF),
            ProtocolParameters.MessageId,
            ProtocolParameters.Commands.KeepAliveRequest,
            1,
         };

         return S101.TxFrame.WriteMessage(bytes, bytes.Length, out length);
      }

      /// <summary>
      /// Frames a "Keep-Alive Response" message into a byte array.
      /// </summary>
      /// <param name="slotId">The S101 slot id to address.</param>
      /// <param name="length">Receives the length of the framed package.</param>
      /// <returns>A byte array containing the framed package.</returns>
      public static byte[] WriteKeepAliveResponse(int slotId, out int length)
      {
         var bytes = new byte[]
         {
            (byte)(slotId & 0xFF),
            ProtocolParameters.MessageId,
            ProtocolParameters.Commands.KeepAliveResponse,
            1,
         };

         return S101.TxFrame.WriteMessage(bytes, bytes.Length, out length);
      }

      /// <summary>
      /// Can be overridden to return application-defined header bytes.
      /// </summary>
      /// <param name="length">Receives the number of valid bytes in the returned byte array.</param>
      /// <returns>An array containing the application-defined bytes to store in the frame header.</returns>
      protected virtual byte[] GetApplicationBytes(out byte length)
      {
         length = 0;
         return null;
      }

      /// <summary>
      /// Dispose of allocated resources.
      /// </summary>
      /// <param name="disposing">If true, dispose of unmanaged resources
      /// otherwise only dispose of managed resources</param>
      protected virtual void Dispose(bool disposing)
      {
         _buffer = null;
      }

      #region Implicit IBerOutput Members
      /// <summary>
      /// Writes a byte of ember data to the output.
      /// </summary>
      /// <param name="b">Byte to write</param>
      public void WriteByte(byte b)
      {
         if(_buffer == null)
            _buffer = new BerMemoryOutput();

         if(_buffer.Length >= MaximumPackageLength)
            OnPackageReady(isLastPackage: false);

         if(_dataLength == 0)
            WriteHeader();

         _buffer.WriteByte(b);
         _dataLength++;
      }

      /// <summary>
      /// Writes multiple bytes of ember data to the output, raising the PackageReady event
      /// when necessary.
      /// </summary>
      /// <param name="bytes">The bytes to write.</param>
      public void WriteBytes(IEnumerable<byte> bytes)
      {
         if(bytes == null)
            throw new ArgumentNullException("bytes");
         
         foreach(var b in bytes)
            WriteByte(b);
      }

      /// <summary>
      /// Overriden to do nothing. See Finish().
      /// </summary>
      public void Flush()
      {
         // do nothing: Finish() will flush the data
      }
      #endregion

      #region Implementation
      BerMemoryOutput _buffer;
      int _dataLength;
      bool _isFirstPackage;

      void WriteHeader()
      {
         // --- write header
         // slot:1 msg:0x10 version:1 flags:1 dtd:1 appBytesCountLo:1 appBytesCountHi:1 appBytes:appBytesCount payload
         // flags: 0x80 - first package
         //        0x40 - last package
         //        0x20 - empty package
         _buffer.WriteByte((byte)(SlotId & 0xFF));
         _buffer.WriteByte(ProtocolParameters.MessageId); // S101_Ember
         _buffer.WriteByte(ProtocolParameters.Commands.Payload);    // command
         _buffer.WriteByte(1);    // framingVersion
         _buffer.WriteByte(0);    // flags - filled in by OnPackageReady()
         _buffer.WriteByte((byte)Dtd);

         byte appBytesCount;
         var appBytes = GetApplicationBytes(out appBytesCount);

         _buffer.WriteByte(appBytesCount);    // appBytesCount

         for(int index = 0; index < appBytesCount; index++)
            _buffer.WriteByte(appBytes[index]);

         if(_buffer.Length > MaximumPackageLength)
            throw new InvalidOperationException("Header is longer than maximum package length!");
      }

      void OnPackageReady(bool isLastPackage)
      {
         var isEmptyPackage = _dataLength == 0;

         if(isEmptyPackage)
         {
            if(isLastPackage == false)
               throw new InvalidOperationException("only last package may be empty");

            WriteHeader();
         }

         var memory = _buffer.Memory;
         var packageFlags = (byte)0;

         if(_isFirstPackage)
            packageFlags |= ProtocolParameters.FirstPackageFlag;

         if(isLastPackage)
            packageFlags |= ProtocolParameters.LastPackageFlag;

         if(isEmptyPackage)
            packageFlags |= ProtocolParameters.EmptyPackageFlag;

         memory[4] = packageFlags;

         int framedLength;
         var package = S101.TxFrame.WriteMessage(memory, (int)_buffer.Length, out framedLength);

         OnPackageReady(new PackageReadyArgs(package, framedLength, _dataLength));

         _dataLength = 0;
         _buffer.Clear();

         _isFirstPackage = isLastPackage;
      }

      void AssertFinished()
      {
         if(_dataLength != 0)
            throw new Exception("Disposing of unfinished output! Call Finish() before disposing!");
      }
      #endregion

      #region Implicit IDisposable Members
      // suppress "Implement IDisposable correctly"
      // because implementation is correct, warning is
      // raised because of additional method call to AssertFinished().
      [SuppressMessage("Microsoft.Design", "CA1063")]
      public void Dispose()
      {
         AssertFinished();

         Dispose(true);

         GC.SuppressFinalize(this);
      }
      #endregion
   }
}
