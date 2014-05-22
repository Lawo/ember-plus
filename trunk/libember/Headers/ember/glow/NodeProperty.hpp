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

#ifndef __LIBEMBER_GLOW_NODEPROPERTY_HPP
#define __LIBEMBER_GLOW_NODEPROPERTY_HPP

#include "../ber/Tag.hpp"

namespace libember { namespace glow
{ 
    /**
     * Structure listing the properties a glow node can have
     */
    struct NodeProperty
    {
        /**
         * Enumeration containing the properties a node may have.
         */
        enum _Domain
        {
            Identifier = 0, 
            Description = 1,
            IsRoot = 2,
            IsOnline = 3,
            SchemaIdentifiers = 4
        };

        typedef unsigned int value_type;

        /**
         * Initializes a new instance.
         * @param value The value to initialize this instance with.
         */
        NodeProperty(_Domain value)
            : m_value(value)
        {}

        /**
         * Initializes a new instance.
         * @param value The value to initialize this instance with.
         */
        explicit NodeProperty(value_type value)
            : m_value(value)
        {}

        /**
         * Returns the value.
         * @return The value.
         */
        value_type value() const
        {
            return m_value;
        }

    private:
        value_type m_value;
    };
}
}

#endif  // __LIBEMBER_GLOW_NODEPROPERTY_HPP
