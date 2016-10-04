/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_DIRFIELDMASK_HPP
#define __LIBEMBER_GLOW_DIRFIELDMASK_HPP

//SimianIgnore

namespace libember { namespace glow
{
    /**
     * A scoped enumeration type containing the symbolic names
     * for the optional flags of a GetDirectory request.
     */
    class DirFieldMask
    {
        public:
            enum _Domain
            {
                Default = 0,
                Identifier = 1,
                Description = 2,
                Tree = 3,
                Value = 4,
                Connections = 5,
                All = 0xFFFFFFFF,
                Sparse = -2
            };

            typedef int value_type;

        public:
            /**
             * Initializes a new instance.
             * @param value The value to initializes the instance with.
             */
            DirFieldMask(_Domain value)
                : m_value(value)
            {}

            /**
             * Initializes a new instance.
             * @param value The value to initializes the instance with.
             */
            explicit DirFieldMask(value_type value)
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

#endif  // __LIBEMBER_GLOW_DIRFIELDMASK_HPP
