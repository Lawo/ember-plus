/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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

