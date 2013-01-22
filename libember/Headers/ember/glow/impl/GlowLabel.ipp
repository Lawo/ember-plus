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
