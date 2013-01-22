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

#ifndef __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_IPP
#define __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_IPP

#include "../../util/Inline.hpp"
#include "../GlowElement.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowElementCollection::GlowElementCollection()
        : GlowContainer(GlowType::ElementCollection)
    {}

    LIBEMBER_INLINE
    GlowElementCollection::GlowElementCollection(ber::Tag const& tag)
        : GlowContainer(GlowType::ElementCollection, tag)
    {}
}
}

#endif  // __LIBEMBER_GLOW_GLOWELEMENTCOLLECTION_IPP
