/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_IPP
#define __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_IPP

#include "../../util/Inline.hpp"
#include "../GlowElement.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowElementCollection::GlowElementCollection()
        : GlowContainer(GlowType::ElementCollection)
    {}

    LIBEMBER_INLINE
    GlowElementCollection::GlowElementCollection(ber::Tag const& tag)
        : GlowContainer(GlowType::ElementCollection, tag)
    {}
}
}

#endif  // __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_IPP
