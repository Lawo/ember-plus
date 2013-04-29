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

#ifndef __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_IPP
#define __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_IPP

#include "../../util/Inline.hpp"
#include "../../dom/VariantLeaf.hpp"
#include "../GlowTags.hpp"
#include "../util/ValueConverter.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowTupleItemDescription::GlowTupleItemDescription(ber::Tag const& tag)
        : GlowContainer(GlowType::TupleItemDescription, tag)
    {}

    LIBEMBER_INLINE
    GlowTupleItemDescription::GlowTupleItemDescription(ber::Type const& type, std::string const& name)
        : GlowContainer(GlowType::TupleItemDescription)
    {
        insert(end(), new dom::VariantLeaf(GlowTags::TupleItemDescription::Type(), type.value()));
        insert(end(), new dom::VariantLeaf(GlowTags::TupleItemDescription::Name(), name));
    }

    LIBEMBER_INLINE
    ber::Type GlowTupleItemDescription::type() const
    {
        ber::Tag const tag = GlowTags::TupleItemDescription::Name(); 
        const_iterator first = begin();
        const_iterator const last = end();
        const_iterator const result = util::find_tag(first, last, tag);
        if (result != last)
        {
            int const value = util::ValueConverter::valueOf(&*result, int(0));
            return static_cast<ber::Type::_Domain>(value);
        }
        else
        {
            return ber::Type::None;
        }
    }

    LIBEMBER_INLINE
    std::string GlowTupleItemDescription::name() const
    {
        ber::Tag const tag = GlowTags::TupleItemDescription::Name(); 
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
}
}

#endif  // __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_IPP
