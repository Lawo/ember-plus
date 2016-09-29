/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_IPP
#define __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_IPP

#include "../../util/Inline.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../GlowTags.hpp"
#include "../util/ValueConverter.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowStringIntegerPair::GlowStringIntegerPair(ber::Tag const& tag)
        : GlowContainer(GlowType::StringIntegerPair, tag)
    {}

    LIBEMBER_INLINE
    GlowStringIntegerPair::GlowStringIntegerPair(std::string const& name, int value)
        : GlowContainer(GlowType::StringIntegerPair)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::StringIntegerPair::Name(), name));
        insert(end(), new dom::VariantLeaf(GlowTags::StringIntegerPair::Value(), value));
    }

    LIBEMBER_INLINE
    std::string GlowStringIntegerPair::name() const
    {
        ber::Tag const tag = GlowTags::StringIntegerPair::Name(); 
        const_iterator first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            std::string const value = util::ValueConverter::valueOf(&*result, std::string());
            return value;
        }
        else
        {
            return std::string();
        }
    }

    LIBEMBER_INLINE
    int GlowStringIntegerPair::value() const
    {
        ber::Tag const tag = GlowTags::StringIntegerPair::Value();
        const_iterator first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, -1);
            return value;
        }
        else
        {
            return -1;
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_IPP
