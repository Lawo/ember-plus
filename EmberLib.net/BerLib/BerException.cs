/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

using System;
using System.Text;
using System.Collections.Generic;

#if NO_SERIALIZATION
#else
using System.Runtime.Serialization;
using System.Security.Permissions;
#endif

namespace BerLib
{
#if NO_SERIALIZATION
#else
   [Serializable]
#endif
   public class BerException : Exception
   {
      public BerException(int errorCode, string message)
      : base(message)
      {
         ErrorCode = errorCode;
      }

      public int ErrorCode { get; private set; }

#if NO_SERIALIZATION
#else
      protected BerException(SerializationInfo info, StreamingContext context)
      : base(info, context)
      {
         ErrorCode = info.GetInt32("BerException.ErrorCode");
      }

      [SecurityPermission(SecurityAction.LinkDemand, Flags = SecurityPermissionFlag.SerializationFormatter)]
      public override void GetObjectData(SerializationInfo info, StreamingContext context)
      {
         base.GetObjectData(info, context);

         info.AddValue("BerException.ErrorCode", ErrorCode);
      }
#endif
   }
}
