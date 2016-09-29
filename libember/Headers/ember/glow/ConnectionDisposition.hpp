/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_CONNECTIONDISPOSITION_HPP
#define __LIBEMBER_GLOW_CONNECTIONDISPOSITION_HPP

//SimianIgnore

namespace libember { namespace glow
{
    /**
     * A scoped enumeration type containing the symbolic names
     * for the connection dispositions supported by Ember+.
     */
    class ConnectionDisposition
    {
        public:
            enum _Domain
            {
                Tally = 0,
                Modified = 1,
                Pending = 2,
                Locked = 3
            };

            typedef int value_type;

        public:
            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            ConnectionDisposition(_Domain value)
                : m_value(value)
            {}

            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            explicit ConnectionDisposition(value_type value)
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

#endif  // __LIBEMBER_GLOW_CONNECTIONDISPOSITION_HPP
