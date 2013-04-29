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

#ifndef __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_HPP
#define __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_HPP

#include "GlowContainer.hpp"

namespace libember { namespace glow
{
    /**
     * A GlowTupleItemDescription contains a ber type information and a name.
     */
    class LIBEMBER_API GlowTupleItemDescription : public GlowContainer
    {
        public:
            /**
             * Constructor, initializes an enumeration entry with the application tag specified.
             * @param tag Application tag.
             */
            explicit GlowTupleItemDescription(ber::Tag const& tag);

            /**
             * Initializes a new GlowTupleItemDescripiton with a known type and name.
             * @param type The type to initialize this instance with.
             * @param name The name to initialize this instance with.
             */
            GlowTupleItemDescription(ber::Type const& type, std::string const& name);

            /**
             * Returns the type stored in this instance. If the property is not available,
             * ber::Type::None will be returned.
             * @return The type stored in this instance.
             */
            ber::Type type() const;

            /**
             * Returns the name of this item description. If no name is set, an empty
             * string will be returned.
             * @return The name of this item description.
             */
            std::string name() const;
    };
}
}

#endif  // __LIBEMBER_GLOW_GLOWTUPLEITEMDESCRIPTION_HPP
