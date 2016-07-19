/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_CONTAINER_IPP
#define __LIBEMBER_GLOW_CONTAINER_IPP

#include "../../util/Inline.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowContainer::GlowContainer(GlowType const& type, ber::Tag const& tag)
        : dom::Sequence(tag)    
        , m_universalTag(type.toTypeTag())
    {
    }
    
    LIBEMBER_INLINE
    GlowContainer::GlowContainer(GlowType const& type)
        : dom::Sequence(GlowTags::ElementDefault())
        , m_universalTag(type.toTypeTag())
    {}

    LIBEMBER_INLINE
    GlowContainer::iterator GlowContainer::insertImpl(iterator const&, Node* child)
    {
        iterator const last = end();
        if (child != 0)
        {
            ber::Tag const tag = child->applicationTag();
            for (iterator i = begin(); i != last; ++i)
            {
                ber::Tag const other = i->applicationTag();
                if (other > tag)
                {
                    return dom::detail::ListContainer::insertImpl(i, child);
                }
            }
        }
        return dom::detail::ListContainer::insertImpl(last, child);
    }

    LIBEMBER_INLINE
    ber::Tag GlowContainer::typeTagImpl() const
    {
        return m_universalTag;
    }
}
}

#endif  // __LIBEMBER_GLOW_CONTAINER_IPP
