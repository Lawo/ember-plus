/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
                               Action<GlowMatrixBase> onMatrix = null,
                               Action<GlowFunctionBase> onFunction = null)
      {
         _onNode = onNode;
         _onParameter = onParameter;
         _onMatrix = onMatrix;
         _onFunction = onFunction;
      }

      #region Implementation
      Action<GlowNodeBase> _onNode;
      Action<GlowParameterBase> _onParameter;
      Action<GlowMatrixBase> _onMatrix;
      Action<GlowFunctionBase> _onFunction;
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

      bool IGlowVisitor<object, bool>.Visit(GlowFunction glow, object state)
      {
         if(_onFunction != null)
         {
            _onFunction(glow);
            return true;
         }

         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowQualifiedFunction glow, object state)
      {
         if(_onFunction != null)
         {
            _onFunction(glow);
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

      bool IGlowVisitor<object, bool>.Visit(GlowInvocationResult glow, object state)
      {
         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowTemplate glow, object state)
      {
         return false;
      }

      bool IGlowVisitor<object, bool>.Visit(GlowQualifiedTemplate glow, object state)
      {
         return false;
      }
      #endregion
   }
}
