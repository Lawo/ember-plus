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
   }
}
