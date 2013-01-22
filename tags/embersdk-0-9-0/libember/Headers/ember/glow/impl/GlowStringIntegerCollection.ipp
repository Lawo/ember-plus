#ifndef __LIBEMBER_GLOW_GLOWSTRINGINTEGERCOLLECTION_IPP
#define __LIBEMBER_GLOW_GLOWSTRINGINTEGERCOLLECTION_IPP

#include "../../util/Inline.hpp"
#include "../GlowStringIntegerPair.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowStringIntegerCollection::GlowStringIntegerCollection(ber::Tag const& tag)
        : GlowElement(GlowType::StringIntegerCollection, tag)
    {}

    LIBEMBER_INLINE
    void GlowStringIntegerCollection::insert(std::string const& name, int value)
    {
        GlowElement::insert(end(), new GlowStringIntegerPair(name, value));
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTRINGINTEGERCOLLECTION_IPP
