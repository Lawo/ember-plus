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
using BerLib;


namespace EmberLib
{
   /// <summary>
   /// very simple Ember writer that always uses the indefinite length form for containers
   /// </summary>
   public class EmberWriter
   {
      public EmberWriter(IBerOutput output)
      {
         if(output == null)
            throw new ArgumentNullException("output");

         _output = output;
      }

      public void WriteContainerBegin(BerTag tag, uint type)
      {
         WriteOuterHeader(tag, BerDefinitions.IndefiniteLength);
         BerEncoding.EncodeTag(_output, new BerTag(type, true));
         BerEncoding.EncodeLength(_output, BerDefinitions.IndefiniteLength);
      }

      public void WriteSequenceBegin(BerTag tag)
      {
         WriteContainerBegin(tag, BerType.Sequence);
      }

      public void WriteSetBegin(BerTag tag)
      {
         WriteContainerBegin(tag, BerType.Set);
      }

      public void WriteContainerEnd()
      {
         _output.WriteBytes(BerEncoding.IndefiniteLengthTerminator);
         _output.Flush();
      }

      [Obsolete]
      public void WriteFrameBegin()
      {
#pragma warning disable 618 // EmberFrame is obsolete
         WriteContainerBegin(Legacy.EmberFrame.FrameTag, BerType.Sequence);
#pragma warning restore 618
      }

      public void Write(BerTag tag, bool value)
      {
         WriteOuterHeader(tag, 3);
         BerEncoding.EncodeTag(_output, new BerTag(BerType.Boolean));
         BerEncoding.EncodeLength(_output, 1);
         BerEncoding.EncodeBoolean(_output, value);
      }

      public void Write(BerTag tag, int value)
      {
         var valueLength = BerEncoding.GetIntegerLength(value);

         WriteOuterHeader(tag, valueLength + 2);
         BerEncoding.EncodeTag(_output, new BerTag(BerType.Integer));
         BerEncoding.EncodeLength(_output, valueLength);
         BerEncoding.EncodeInteger(_output, value, (int)valueLength);
      }

      public void Write(BerTag tag, long value)
      {
         var valueLength = BerEncoding.GetLongLength(value);

         WriteOuterHeader(tag, valueLength + 2);
         BerEncoding.EncodeTag(_output, new BerTag(BerType.Integer));
         BerEncoding.EncodeLength(_output, valueLength);
         BerEncoding.EncodeLong(_output, value, (int)valueLength);
      }

      public void Write(BerTag tag, double value)
      {
         var valueOutput = new BerMemoryOutput();
         var valueLength = BerEncoding.EncodeReal(valueOutput, value);

         WriteOuterHeader(tag, valueLength + 2);
         BerEncoding.EncodeTag(_output, new BerTag(BerType.Real));
         BerEncoding.EncodeLength(_output, valueLength);
         _output.WriteBytes(valueOutput.Memory);
      }

      public void Write(BerTag tag, string value)
      {
         var valueOutput = new BerMemoryOutput();
         var valueLength = BerEncoding.EncodeUtf8String(valueOutput, value);
         var innerTag = new BerTag(BerType.UTF8String);

         WriteOuterHeader(tag, valueLength + BerEncoding.GetHeaderLength(innerTag, valueLength));
         BerEncoding.EncodeTag(_output, innerTag);
         BerEncoding.EncodeLength(_output, valueLength);
         _output.WriteBytes(valueOutput.Memory);
      }

      public void Write(BerTag tag, byte[] value)
      {
         var valueLength = value.Length;
         var innerTag = new BerTag(BerType.OctetString);

         WriteOuterHeader(tag, valueLength + BerEncoding.GetHeaderLength(innerTag, valueLength));
         BerEncoding.EncodeTag(_output, innerTag);
         BerEncoding.EncodeLength(_output, valueLength);
         _output.WriteBytes(value);
      }

      public void WriteRelativeOid(BerTag tag, int[] value)
      {
         var innerTag = new BerTag(BerType.RelativeOid);
         var valueLength = BerEncoding.GetRelativeOidLength(value);

         WriteOuterHeader(tag, valueLength + BerEncoding.GetHeaderLength(innerTag, valueLength));
         BerEncoding.EncodeTag(_output, innerTag);
         BerEncoding.EncodeLength(_output, valueLength);
         BerEncoding.EncodeRelativeOid(_output, value);
      }

      #region Implementation
      IBerOutput _output;

      void WriteOuterHeader(BerTag tag, int length)
      {
         BerEncoding.EncodeTag(_output, tag.ToContainer());
         BerEncoding.EncodeLength(_output, length);
      }
      #endregion
   }
}
