/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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

               BerTypeNumber = ChangeToSequence();
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
