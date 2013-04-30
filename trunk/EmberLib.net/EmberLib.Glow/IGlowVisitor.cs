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

namespace EmberLib.Glow
{
   public interface IGlowVisitor<TState, TResult>
   {
      TResult Visit(GlowCommand glow, TState state);
      TResult Visit(GlowElementCollection glow, TState state);
      TResult Visit(GlowNode glow, TState state);
      TResult Visit(GlowParameter glow, TState state);
      TResult Visit(GlowStreamCollection glow, TState state);
      TResult Visit(GlowQualifiedParameter glow, TState state);
      TResult Visit(GlowQualifiedNode glow, TState state);
      TResult Visit(GlowRootElementCollection glow, TState state);
      TResult Visit(GlowSubContainer glow, TState state);
      TResult Visit(GlowMatrix glow, TState state);
      TResult Visit(GlowQualifiedMatrix glow, TState state);
      TResult Visit(GlowFunction glow, TState state);
      TResult Visit(GlowQualifiedFunction glow, TState state);
      TResult Visit(GlowInvocationResult glow, TState state);
   }
}
