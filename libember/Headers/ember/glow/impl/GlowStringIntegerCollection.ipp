/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

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
