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

#ifndef __LIBEMBER_GLOW_GLOWSTREAMDESCRIPTOR_IPP
#define __LIBEMBER_GLOW_GLOWSTREAMDESCRIPTOR_IPP

#include "../../util/Inline.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../GlowTags.hpp"
#include "../util/ValueConverter.hpp"

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
        int const value = util::ValueConverter::toValue(find_node<dom::VariantLeaf>(begin(), end(), tag), 1);

        return static_cast<StreamFormat::_Domain>(value);
    }

    LIBEMBER_INLINE
    int GlowStreamDescriptor::offset() const
    {
        ber::Tag const tag = GlowTags::StreamDescriptor::Offset();
        int const value = util::ValueConverter::toValue(find_node<dom::VariantLeaf>(begin(), end(), tag), 0);

        return value;
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTREAMDESCRIPTOR_IPP
