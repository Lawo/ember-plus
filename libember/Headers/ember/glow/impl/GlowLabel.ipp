/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWLABEL_IPP
#define __LIBEMBER_GLOW_GLOWLABEL_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowLabel::GlowLabel(ber::ObjectIdentifier const& basePath, std::string description)
        : GlowContainer(GlowType::Label)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::Label::BasePath(), basePath));
        insert(end(), new dom::VariantLeaf(GlowTags::Label::Description(), description));
    }

    LIBEMBER_INLINE
    GlowLabel::GlowLabel(ber::ObjectIdentifier const& basePath, std::string description, ber::Tag const& tag)
        : GlowContainer(GlowType::Label, tag)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::Label::BasePath(), basePath));
        insert(end(), new dom::VariantLeaf(GlowTags::Label::Description(), description));
    }

    LIBEMBER_INLINE
    GlowLabel::GlowLabel(ber::Tag const& tag)
        : GlowContainer(GlowType::Label, tag)
    {
    }

    LIBEMBER_INLINE
    ber::ObjectIdentifier GlowLabel::basePath() const
    {
        ber::Tag const tag = GlowTags::Label::BasePath();
        const_iterator const first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            ber::ObjectIdentifier const value = util::ValueConverter::valueOf(&*result, ber::ObjectIdentifier());
            return value;
        }
        else
        {
            return ber::ObjectIdentifier();
        }
    }

    LIBEMBER_INLINE
    std::string GlowLabel::description() const
    {
        ber::Tag const tag = GlowTags::Label::Description();
        const_iterator const first = begin();
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
}
}

#endif  // __LIBEMBER_GLOW_GLOWLABEL_IPP
