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

