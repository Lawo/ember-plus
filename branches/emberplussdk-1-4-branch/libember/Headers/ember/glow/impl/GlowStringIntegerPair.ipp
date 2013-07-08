/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
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
