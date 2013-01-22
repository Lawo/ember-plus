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

#ifndef __LIBEMBER_GLOW_GLOWSTRINGINTEGERCOLLECTION_HPP
#define __LIBEMBER_GLOW_GLOWSTRINGINTEGERCOLLECTION_HPP

#include "GlowElement.hpp"

namespace libember { namespace glow
{
    /**
     * The StringIntegerCollection contains a list of StringIntegerPairs.
     */
    class LIBEMBER_API GlowStringIntegerCollection : public GlowElement
    {
        public:
            /**
             * Initializes an empty stream collection with the provided application tag.
             * @param tag Application tag.
             */
            explicit GlowStringIntegerCollection(ber::Tag const& tag);

            /**
             * Inserts a new StreamEntry into the collection.
             * @param name The enumeration entry name
             * @param value The enumeration entry value. This is the value that has to be sent 
             *      to the remote device.
             */
            void insert(std::string const& name, int value);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/GlowStringIntegerCollection.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSTRINGINTEGERCOLLECTION_HPP
