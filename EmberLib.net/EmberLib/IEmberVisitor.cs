/*
   EmberLib.net -- .NET implementation of the Ember+ Protocol

   Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

using System;
using System.Collections.Generic;
using System.Text;

namespace EmberLib
{
   /// <summary>
   /// Interface for visiting an EmberNode object tree.
   /// Use <see cref="EmberLib.EmberNode.Accept"/> to accept
   /// visitors implementing this interface.
   /// </summary>
   /// <typeparam name="TState">Generic visitor state.</typeparam>
   /// <typeparam name="TResult">Return type of the Visit() methods.</typeparam>
   public interface IEmberVisitor<TState, TResult>
   {
      TResult Visit(EmberContainer node, TState state);
      TResult Visit(EmberSet node, TState state);
      TResult Visit(EmberSequence node, TState state);
      TResult Visit(BooleanEmberLeaf node, TState state);
      TResult Visit(IntegerEmberLeaf node, TState state);
      TResult Visit(LongEmberLeaf node, TState state);
      TResult Visit(RealEmberLeaf node, TState state);
      TResult Visit(StringEmberLeaf node, TState state);
      TResult Visit(OctetStringEmberLeaf node, TState state);
      TResult Visit(ObjectIdentifierEmberLeaf node, TState state);
      TResult Visit(RelativeOidEmberLeaf node, TState state);
      TResult Visit(NullEmberLeaf node, TState state);
   }
}
