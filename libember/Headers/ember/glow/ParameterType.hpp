/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            ParameterType(_Domain value)
                : m_value(value)
            {}

            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            explicit ParameterType(value_type value)
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

#endif  // __LIBEMBER_GLOW_PARAMETERTYPE_HPP
