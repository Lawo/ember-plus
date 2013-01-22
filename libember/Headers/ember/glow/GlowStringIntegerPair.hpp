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

#ifndef __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_HPP
#define __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_HPP

#include "GlowContainer.hpp"

namespace libember { namespace glow
{
    /**
     * A GlowStringIntegerPair represents a single enumeration entry which contains a name and a value.
     */
    class LIBEMBER_API GlowStringIntegerPair : public GlowContainer
    {
        public:
            /**
             * Constructor, initializes an enumeration entry with the application tag specified.
             * @param tag Application tag.
             */
            explicit GlowStringIntegerPair(ber::Tag const& tag);

            /**
             * Constructor, initializes a enumeration entry with a provided name and value.
             * @param name Enumeration entry name.
             * @param value Enumeration entry value.
             */
            GlowStringIntegerPair(std::string const& name, int value);

            /**
             * Returns the name of the enumeration entry.
             * @return The enumeration entry name or an empty string if it doesn't exist.
             */
            std::string name() const;

            /**
             * Returns the value of the enumeration entry.
             * @return The enumeration entry value
             */
            int value() const;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowStringIntegerPair.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_HPP
