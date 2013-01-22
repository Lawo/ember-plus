#ifndef __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_IPP
#define __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_IPP

#include "../../util/Inline.hpp"
#include "../GlowStreamEntry.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowStreamCollection::GlowStreamCollection(ber::Tag const& tag)
        : GlowElement(GlowType::StreamCollection, tag)
    {}

    LIBEMBER_INLINE
    void GlowStreamCollection::insert(int streamIdentifier, int value)
    {
        GlowElement::insert(end(), new GlowStreamEntry(streamIdentifier, value));
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_IPP
