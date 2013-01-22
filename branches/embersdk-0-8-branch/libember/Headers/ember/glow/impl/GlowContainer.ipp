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
    ber::Tag GlowContainer::typeTagImpl() const
    {
        return m_universalTag;
    }
}
}

#endif  // __LIBEMBER_GLOW_CONTAINER_IPP
