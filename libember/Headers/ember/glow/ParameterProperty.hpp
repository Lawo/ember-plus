/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_PARAMETERPROPERTY_HPP
#define __LIBEMBER_GLOW_PARAMETERPROPERTY_HPP

#include "../ber/Tag.hpp"

//SimianIgnore

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
            SchemaIdentifiers = 17,
            TemplateReference = 18
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

//EndSimianIgnore

#endif  // __LIBEMBER_GLOW_PARAMETERPROPERTY_HPP
