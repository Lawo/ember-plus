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

#ifndef __LIBEMBER_GLOW_PARAMETERTYPE_HPP
#define __LIBEMBER_GLOW_PARAMETERTYPE_HPP

//SimianIgnore

namespace libember { namespace glow
{
    /**
     * A scoped enumeration type containing the symbolic names
     * for the parameter types supported by Glow.
     */
    class ParameterType
    {
        public:
            enum _Domain
            {
                None = 0,
                Integer = 1,
                Real = 2,
                String = 3,
                Boolean = 4,
                Trigger = 5,
                Enum = 6,
                Octets = 7
            };

            typedef int value_type;

        public:
            /**
             * Initializes a parameter type. 
             * @param type Parameter type to initialize this instance with.
             */
            ParameterType(_Domain type)
                : m_value(type)
            {}

            /**
             * Returns the command value.
             * @return The command value.
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

#endif  // __LIBEMBER_GLOW_PARAMETERTYPE_HPP
