/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/


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
      TResult Visit(GlowTemplate glow, TState state);
      TResult Visit(GlowQualifiedTemplate glow, TState state);
   }
}
