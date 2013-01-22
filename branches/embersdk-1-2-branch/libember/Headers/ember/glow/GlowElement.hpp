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

#ifndef __LIBEMBER_GLOW_GLOWELEMENT_HPP
#define __LIBEMBER_GLOW_GLOWELEMENT_HPP

#include "GlowContainer.hpp"

namespace libember { namespace glow
{
    /**
     * The Glow Element is the base class for Nodes, Parameters and Commands.
     */
    class LIBEMBER_API GlowElement : public GlowContainer
    {
        protected:
            /**
             * Initializes an new element with the provide type.
             * @note The application tag will be set to ContextSpecific-0.
             * @param type The glow type to set.
             */
            explicit GlowElement(GlowType const& type);

            /**
             * Constructor, initializes a new element with the provided type
             * and application tag.
             * @param type The glow type to set.
             * @param tag The application tag to initialize the container with.
             */
            GlowElement(GlowType const& type, ber::Tag const& tag);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowElement.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWELEMENT_HPP
