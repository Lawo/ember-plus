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

namespace EmberLib.Glow.PowerPack
{
   public sealed class GlowInlineVisitor : IGlowVisitor<object, bool>
   {
      public GlowInlineVisitor(Action<GlowNodeBase> onNode = null,
                               Action<GlowParameterBase> onParameter = null,
                               Action<GlowMatrixBase> onMatrix = null)
      {
         _onNode = onNode;
         _onParameter = onParameter;
         _onMatrix = onMatrix;
      }

      #region Implementation
      Action<GlowNodeBase> _onNode;
      Action<GlowParameterBase> _onParameter;
      Action<GlowMatrixBase> _onMatrix;
      #endregion

      #region IGlowVisitor<object,object> Members
      bool IGlowVisitor<object, bool>.Visit(GlowNode glow, object state)
      {
         if(_onNode != null)
         {
            _onNode(glow);
            return true;
         }

         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowQualifiedNode glow, object state)
      {
         if(_onNode != null)
         {
            _onNode(glow);
            return true;
         }

         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowParameter glow, object state)
      {
         if(_onParameter != null)
         {
            _onParameter(glow);
            return true;
         }

         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowQualifiedParameter glow, object state)
      {
         if(_onParameter != null)
         {
            _onParameter(glow);
            return true;
         }

         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowMatrix glow, object state)
      {
         if(_onMatrix != null)
         {
            _onMatrix(glow);
            return true;
         }

         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowQualifiedMatrix glow, object state)
      {
         if(_onMatrix != null)
         {
            _onMatrix(glow);
            return true;
         }

         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowCommand glow, object state)
      {
         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowElementCollection glow, object state)
      {
         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowStreamCollection glow, object state)
      {
         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowRootElementCollection glow, object state)
      {
         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowSubContainer glow, object state)
      {
         return false;
      }
      #endregion
   }
}
