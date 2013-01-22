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
using System.Text;
using BerLib;

namespace EmberLib.Legacy
{
   [Obsolete("This class is a feature exclusively used by the Glow DTD version 1. Please consider using Glow DTD version 2 or higher.")]
   public class EmberDynamicContainer : EmberSet
   {
      public EmberDynamicContainer(BerTag tag, EmberContainer parent)
      : base(tag, parent, true)
      {
      }

      protected internal override void InsertChildNode(EmberNode node)
      {
         if(IsMapUsed)
         {
            var tag = node.Tag;
            var nodesMap = NodesMap;

            if(nodesMap.ContainsKey(tag))
            {
               IsMapUsed = false;
               nodesMap.Clear();

               Type = ChangeToSequence();
            }
            else
            {
               nodesMap[tag] = node;
            }
         }

         InsertChildNodeBase(node);
      }

      protected virtual uint ChangeToSequence()
      {
         return BerType.Sequence;
      }
   }
}
