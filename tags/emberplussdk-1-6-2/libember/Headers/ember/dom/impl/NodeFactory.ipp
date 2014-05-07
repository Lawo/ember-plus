/*
    libember -- C++ 03 implementation of the Ember+ Protocol
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

#ifndef __LIBEMBER_DOM_IMPL_NODEFACTORY_IPP
#define __LIBEMBER_DOM_IMPL_NODEFACTORY_IPP

#include "../../util/Inline.hpp"
#include "../Set.hpp"
#include "../Sequence.hpp"

namespace libember { namespace dom 
{
    LIBEMBER_INLINE
    NodeFactory::~NodeFactory()
    {}

    LIBEMBER_INLINE
    Node* NodeFactory::createSet(ber::Tag const& tag) const
    {
        return new Set(tag);
    }

    LIBEMBER_INLINE
    Node* NodeFactory::createSequence(ber::Tag const& tag) const
    {
        return new Sequence(tag);
    }
}
}

#endif  // __LIBEMBER_DOM_IMPL_NODEFACTORY_IPP

