#ifndef __LIBEMBER_GLOW_GLOWELEMENT_IPP
#define __LIBEMBER_GLOW_GLOWELEMENT_IPP

#include "../../util/Inline.hpp"

namespace libember { namespace glow {
    LIBEMBER_INLINE
    GlowElement::GlowElement(GlowType const& type)
        : GlowContainer(type)
    {}

    LIBEMBER_INLINE
    GlowElement::GlowElement(GlowType const& type, ber::Tag const& tag)
        : GlowContainer(type, tag)  
    {}
}
}

#endif  // __LIBEMBER_GLOW_GLOWELEMENT_IPP
