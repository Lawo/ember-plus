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

#ifndef __LIBEMBER_GLOW_PARAMETERPROPERTY_HPP
#define __LIBEMBER_GLOW_PARAMETERPROPERTY_HPP

#include "../ber/Tag.hpp"

namespace libember { namespace glow
{ 
    /**
     * Structure listing the properties a glow parameter can have
     */
    struct ParameterProperty
    {
        /**
         * Enumeration containing the properties a parameter may have.
         */
        enum _Domain
        {
            Identifier = 0, 
            Description = 1,
            Value = 2,
            Minimum = 3,
            Maximum = 4,
            Access = 5,
            Format = 6,
            Enumeration = 7,
            Factor = 8,
            IsOnline = 9,
            Formula = 10,
            Step = 11,
            Default = 12,
            Type = 13,
            StreamIdentifier = 14,
            EnumMap = 15,
            StreamDescriptor = 16,
            SchemaIdentifiers = 17
        };

        typedef unsigned int value_type;

        /**
         * Initializes a new instance.
         * @param value The value to initialize this instance with.
         */
        ParameterProperty(_Domain value)
            : m_value(value)
        {}

        /**
         * Initializes a new instance.
         * @param value The value to initialize this instance with.
         */
        explicit ParameterProperty(value_type value)
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

#endif  // __LIBEMBER_GLOW_PARAMETERPROPERTY_HPP
