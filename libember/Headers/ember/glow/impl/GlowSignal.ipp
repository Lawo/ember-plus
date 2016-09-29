/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWSIGNAL_IPP
#define __LIBEMBER_GLOW_GLOWSIGNAL_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    int GlowSignal::number() const
    {
        ber::Tag const tag = GlowTags::Signal::Number();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, 0);
            return value;
        }
        else
        {
            return -1;
        }
    }

    LIBEMBER_INLINE
    GlowSignal::GlowSignal(GlowType const& type, ber::Tag const& tag)
        : GlowContainer(type, tag)
    {}

    LIBEMBER_INLINE
    GlowSignal::GlowSignal(GlowType const& type, int number)
        : GlowContainer(type)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::Signal::Number(), number));
    }

    LIBEMBER_INLINE
    GlowSignal::GlowSignal(GlowType const& type, ber::Tag const& tag, int number)
        : GlowContainer(type, tag)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::Signal::Number(), number));
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSIGNAL_IPP
