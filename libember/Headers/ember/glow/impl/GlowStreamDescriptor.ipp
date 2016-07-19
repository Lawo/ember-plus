/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWSTREAMDESCRIPTOR_IPP
#define __LIBEMBER_GLOW_GLOWSTREAMDESCRIPTOR_IPP

#include "../../util/Inline.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../GlowTags.hpp"
#include "../util/ValueConverter.hpp"
#include "../StreamFormat.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowStreamDescriptor::GlowStreamDescriptor(ber::Tag const& tag)
        : GlowContainer(GlowType::StreamDescriptor, tag)
    {}

    LIBEMBER_INLINE
    GlowStreamDescriptor::GlowStreamDescriptor(ber::Tag const& tag, StreamFormat format, int offset)
        : GlowContainer(GlowType::StreamDescriptor, tag)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::StreamDescriptor::Format(), format.value()));
        insert(end(), new dom::VariantLeaf(GlowTags::StreamDescriptor::Offset(), offset));
    }

    LIBEMBER_INLINE
    StreamFormat GlowStreamDescriptor::format() const
    {
        ber::Tag const tag = GlowTags::StreamDescriptor::Format();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, 0);
            return static_cast<StreamFormat::_Domain>(value);
        }
        else
        {
            return StreamFormat::UnsignedInt8;
        }
    }

    LIBEMBER_INLINE
    int GlowStreamDescriptor::offset() const
    {
        ber::Tag const tag = GlowTags::StreamDescriptor::Offset();
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
            return 0;
        }
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTREAMDESCRIPTOR_IPP
