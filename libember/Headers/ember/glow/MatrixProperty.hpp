/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
            SchemaIdentifiers = 11,
            TemplateReference = 12
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
