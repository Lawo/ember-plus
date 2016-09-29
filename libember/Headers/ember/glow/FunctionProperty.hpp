/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_FUNCTIONPROPERTY_HPP
#define __LIBEMBER_GLOW_FUNCTIONPROPERTY_HPP

#include "../ber/Tag.hpp"

//SimianIgnore

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
                Result = 3,

                TemplateReference = 4
            };

        public:
            typedef unsigned int value_type;

            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            FunctionProperty(_Domain value)
                : m_value(value)
            {}

            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            explicit FunctionProperty(value_type value)
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

#endif  // __LIBEMBER_GLOW_FUNCTIONPROPERTY_HPP
