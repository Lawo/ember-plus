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
