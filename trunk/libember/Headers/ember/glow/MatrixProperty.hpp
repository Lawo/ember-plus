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

#ifndef __LIBEMBER_GLOW_MATRIXPROPERTY_HPP
#define __LIBEMBER_GLOW_MATRIXPROPERTY_HPP

#include "../ber/Tag.hpp"

//SimianIgnore

namespace libember { namespace glow
{ 
    /**
     * Structure listing the properties a glow matrix can have
     */
    struct MatrixProperty
    {
        /**
         * Enumeration containing the properties a matrix may have.
         */
        enum _Domain
        {
            Identifier = 0, 
            Description = 1,
            Type = 2,
            AddressingMode = 3,
            TargetCount = 4,
            SourceCount = 5,
            MaximumTotalConnects = 6,
            MaximumConnectsPerTarget = 7,
            ParametersLocation = 8,
            GainParameterNumber = 9,
            Labels = 10,
            SchemaIdentifiers = 11
        };

        typedef unsigned int value_type;

        /**
         * Initializes a new instance.
         * @param value The value to initialize this instance with.
         */
        MatrixProperty(_Domain value)
            : m_value(value)
        {}

        /**
         * Initializes a new instance.
         * @param value The value to initialize this instance with.
         */
        explicit MatrixProperty(value_type value)
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

//EndSimianIgnore

#endif  // __LIBEMBER_GLOW_MATRIXPROPERTY_HPP
