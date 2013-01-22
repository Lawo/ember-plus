#ifndef __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_IPP
#define __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_IPP

#include "../../util/Inline.hpp"
#include "../GlowElement.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowElementCollection* GlowElementCollection::createRoot()
    {
        return new GlowElementCollection(GlowTags::Root());
    }

    LIBEMBER_INLINE
    GlowElementCollection::GlowElementCollection()
        : GlowContainer(GlowType::ElementCollection)
    {}

    LIBEMBER_INLINE
    GlowElementCollection::GlowElementCollection(ber::Tag const& tag)
        : GlowContainer(GlowType::ElementCollection, tag)
    {}

    LIBEMBER_INLINE
    void GlowElementCollection::insertElement(GlowElement* element)
    {
        insert(end(), element);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_IPP
