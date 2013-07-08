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

#ifndef __LIBEMBER_GLOW_FUNCTIONPROPERTY_HPP
#define __LIBEMBER_GLOW_FUNCTIONPROPERTY_HPP

#include "../ber/Tag.hpp"

namespace libember { namespace glow
{ 
    /**
     * Structure listing the properties a glow function or qualified function can have.
     */
    struct FunctionProperty
    {
        public:
            /**
            * A scoped enumeration type containing the symbolic names
            * for the properties an Ember+ function may have.
             */
            enum _Domain
            {
                /** The identifier string. */
                Identifier = 0,

                /** The description string. */
                Description = 1,

                /** The collection which declares the arguments. The arguments are stored inside a dom::Sequence. */
                Arguments = 2,

                /** The collection which contains the result type. */
                Result = 3
            };

        public:
            typedef unsigned int value_type;

            /**
             * Initializes a new FunctionProperty.
             * @param value The property type to initialize this instance with.
             */
            FunctionProperty(_Domain value)
                : m_value(value)
            {}

            /** 
             * Returns the value of the function property.
             * @return The value of the function property.
             */
            value_type value() const
            {
                return m_value;
            }

        private:
            /** Contains the property value passed to the constructor. */
            value_type m_value;
    };
}
}

#endif  // __LIBEMBER_GLOW_FUNCTIONPROPERTY_HPP
