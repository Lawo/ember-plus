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

namespace BerLib
{
   /// <summary>
   /// Simple interface for byte-wise or block-wise writing.
   /// </summary>
   public interface IBerOutput
   {
      /// <summary>
      /// Writes a byte to underlying output.
      /// </summary>
      /// <param name="b">The byte to write.</param>
      void WriteByte(byte b);

      /// <summary>
      /// Writes multiple bytes to underlying output.
      /// </summary>
      /// <param name="bytes">The bytes to write.</param>
      void WriteBytes(IEnumerable<byte> bytes);

      /// <summary>
      /// Flushes any buffered bytes to underlying output.
      /// </summary>
      void Flush();
   }


   /// <summary>
   /// Abstract base class for output types that buffer data to be written
   /// in memory.
   /// </summary>
   public abstract class BerBufferedOutput : IBerOutput, IDisposable
   {
      /// <summary>
      /// Buffers a byte in the in-memory buffer. If the buffer
      /// is full, the buffer is flushed to the underlying output.
      /// </summary>
      /// <param name="b">The byte to write.</param>
      public void WriteByte(byte b)
      {
         Debug.Assert(_bufferLength < _buffer.Length);

         _buffer[_bufferLength] = b;
         _bufferLength++;

         if(_bufferLength == _buffer.Length)
            Flush();
      }

      /// <summary>
      /// Buffers multiple bytes in the in-memory buffer. Everytime the buffer
      /// is full, the buffer is flushed to the underlying output.
      /// </summary>
      /// <param name="bytes">The byte to write.</param>
      public void WriteBytes(IEnumerable<byte> bytes)
      {
         foreach(byte b in bytes)
            WriteByte(b);
      }

      /// <summary>
      /// Flushes any buffered bytes to underlying output.
      /// </summary>
      public void Flush()
      {
         FlushOverride(_buffer, _bufferLength);

         _bufferLength = 0;
      }

      /// <summary>
      /// When overriden in a derived class, flushes the passed data
      /// to the underlying output.
      /// </summary>
      /// <param name="buffer">The buffered bytes to flush.</param>
      /// <param name="bufferLength">The number of bytes in <paramref name="buffer"/></param>
      protected abstract void FlushOverride(byte[] buffer, int bufferLength);

      /// <summary>
      /// Dispose of allocated resources.
      /// </summary>
      /// <param name="disposing">If true, dispose of unmanaged resources
      /// otherwise only dispose of managed resources</param>
      protected virtual void Dispose(bool disposing)
      {
         if(_bufferLength > 0)
            Flush();
      }

      #region Implementation
      byte[] _buffer = new byte[1024];
      int _bufferLength;
      #endregion

      #region IDisposable Members
      public void Dispose()
      {
         Dispose(true);

         GC.SuppressFinalize(this);
      }
      #endregion
   }


   /// <summary>
   /// Provides buffered writing to a stream.
   /// </summary>
   public class BerStreamOutput : BerBufferedOutput
   {
      /// <summary>
      /// Constructs a new instance of BerStreamOutput.
      /// </summary>
      /// <param name="stream">The stream to write to. Will be closed
      /// when BerStreamOutput.Dispose() is called.</param>
      public BerStreamOutput(Stream stream)
      {
         if(stream == null)
            throw new ArgumentNullException();

         if(stream.CanWrite == false)
            throw new ArgumentException("stream is not writeable");

         _stream = stream;
      }

      /// <summary>
      /// Overriden to flush to the underlying stream.
      /// </summary>
      /// <param name="buffer">The buffered bytes to flush.</param>
      /// <param name="bufferLength">The number of bytes in <paramref name="buffer"/></param>
      protected override void FlushOverride(byte[] buffer, int bufferLength)
      {
         _stream.Write(buffer, 0, bufferLength);
      }

      /// <summary>
      /// Overriden to close the underlying stream.
      /// </summary>
      protected override void Dispose(bool disposing)
      {
         base.Dispose(disposing);

         _stream.Dispose();
      }

      #region Implementation
      Stream _stream;
      #endregion
   }


   /// <summary>
   /// Provides non-buffered writing to memory.
   /// </summary>
   public class BerMemoryOutput : IBerOutput
   {
      /// <summary>
      /// Gets the number of bytes written to the in-memory buffer.
      /// </summary>
      public int Length
      {
         get { return _memory.Count; }
      }

      /// <summary>
      /// Writes a byte to memory, growing the block of allocated
      /// memory if necessary.
      /// </summary>
      /// <param name="b">The byte to write.</param>
      public void WriteByte(byte b)
      {
         _memory.Add(b);
      }

      /// <summary>
      /// Writes multiple bytes to memory, growing the block of allocated
      /// memory if necessary.
      /// </summary>
      /// <param name="bytes">The bytes to write.</param>
      public void WriteBytes(IEnumerable<byte> bytes)
      {
         _memory.AddRange(bytes);
      }

      /// <summary>
      /// Does nothing, since memory output is not buffered.
      /// </summary>
      public void Flush()
      {
      }

      /// <summary>
      /// Resets length and position to zero.
      /// </summary>
      public void Clear()
      {
         _memory.Clear();
      }

      /// <summary>
      /// Gets all bytes written to the in-memory buffer.
      /// </summary>
      public IList<byte> Memory
      {
         get { return _memory; }
      }

      /// <summary>
      /// Returns a copy of the memory block used to write to,
      /// only containing the bytes written.
      /// </summary>
      public byte[] ToArray()
      {
         return _memory.ToArray();
      }

      #region Implementation
      List<byte> _memory = new List<byte>();
      #endregion
   }
}
