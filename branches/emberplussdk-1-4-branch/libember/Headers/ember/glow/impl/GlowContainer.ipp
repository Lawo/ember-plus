/*
    libember -- C++ 03 implementation of the Ember+ Protocol
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
    GlowContainer::iterator GlowContainer::insertImpl(iterator, Node* child)
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
