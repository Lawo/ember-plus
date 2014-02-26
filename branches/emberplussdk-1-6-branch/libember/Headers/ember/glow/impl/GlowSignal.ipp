/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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
