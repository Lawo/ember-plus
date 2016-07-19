/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_NODEPROPERTY_HPP
#define __LIBEMBER_GLOW_NODEPROPERTY_HPP

#include "../ber/Tag.hpp"

//SimianIgnore

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
            SchemaIdentifiers = 4,
            TemplateReference = 5
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

//EndSimianIgnore

#endif  // __LIBEMBER_GLOW_NODEPROPERTY_HPP
