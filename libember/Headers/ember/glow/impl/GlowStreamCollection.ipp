/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_IPP
#define __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_IPP

#include "../../util/Inline.hpp"
#include "../GlowTags.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowStreamCollection* GlowStreamCollection::create()
    {
        return new GlowStreamCollection(GlowTags::Root());
    }

    LIBEMBER_INLINE
    GlowStreamCollection::GlowStreamCollection()
        : GlowElement(GlowType::StreamCollection, GlowTags::Root())
    {}

    LIBEMBER_INLINE
    GlowStreamCollection::GlowStreamCollection(ber::Tag const& tag)
        : GlowElement(GlowType::StreamCollection, tag)
    {}

    LIBEMBER_INLINE
    void GlowStreamCollection::insert(int streamIdentifier, int value)
    {
        GlowElement::insert(end(), new GlowStreamEntry(streamIdentifier, value));
    }

    LIBEMBER_INLINE
    void GlowStreamCollection::insert(int streamIdentifier, double value)
    {
        GlowElement::insert(end(), new GlowStreamEntry(streamIdentifier, value));
    }

    LIBEMBER_INLINE
    void GlowStreamCollection::insert(int streamIdentifier, std::string const& value)
    {
        GlowElement::insert(end(), new GlowStreamEntry(streamIdentifier, value));
    }

    LIBEMBER_INLINE
    void GlowStreamCollection::insert(GlowStreamEntry* entry)
    {
        GlowElement::insert(end(), entry);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTREAMCOLLECTION_IPP
