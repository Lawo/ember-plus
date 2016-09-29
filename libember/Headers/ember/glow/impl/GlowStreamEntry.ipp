/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWSTREAMENTRY_IPP
#define __LIBEMBER_GLOW_GLOWSTREAMENTRY_IPP

#include "../../util/Inline.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../GlowTags.hpp"
#include "../util/ValueConverter.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowStreamEntry::GlowStreamEntry(ber::Tag const& tag)
        : GlowContainer(GlowType::StreamEntry, tag)
    {}

    LIBEMBER_INLINE
    GlowStreamEntry::GlowStreamEntry(int streamIdentifier, int value)
        : GlowContainer(GlowType::StreamEntry)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::StreamEntry::StreamIdentifier(), streamIdentifier));
        insert(end(), new dom::VariantLeaf(GlowTags::StreamEntry::StreamValue(), value));
    }
    
    LIBEMBER_INLINE
    GlowStreamEntry::GlowStreamEntry(int streamIdentifier, double value)
        : GlowContainer(GlowType::StreamEntry)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::StreamEntry::StreamIdentifier(), streamIdentifier));
        insert(end(), new dom::VariantLeaf(GlowTags::StreamEntry::StreamValue(), value));
    }

    LIBEMBER_INLINE
    GlowStreamEntry::GlowStreamEntry(int streamIdentifier, std::string const& value)
        : GlowContainer(GlowType::StreamEntry)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::StreamEntry::StreamIdentifier(), streamIdentifier));
        insert(end(), new dom::VariantLeaf(GlowTags::StreamEntry::StreamValue(), value));
    }

    LIBEMBER_INLINE
    int GlowStreamEntry::streamIdentifier() const
    {
        ber::Tag const tag = GlowTags::StreamEntry::StreamIdentifier(); 
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            return util::ValueConverter::valueOf(&*result, -1);
        }
        else
        {
            return -1;
        }
    }

    LIBEMBER_INLINE
    Value GlowStreamEntry::value() const
    {
        ber::Tag const tag = GlowTags::StreamEntry::StreamValue(); 
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            ber::Value const value = util::ValueConverter::valueOf(&*result);
            return Value(value);
        }
        else
        {
            return Value(-1L);
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTREAMENTRY_IPP
