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

        return util::ValueConverter::toValue(find_node<dom::VariantLeaf>(begin(), end(), tag), -1);
    }

    LIBEMBER_INLINE
    Value GlowStreamEntry::value() const
    {
        ber::Tag const tag = GlowTags::StreamEntry::StreamValue();
        
        return Value::fromLeaf(find_node<dom::VariantLeaf>(begin(), end(), tag));
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWSTREAMENTRY_IPP
