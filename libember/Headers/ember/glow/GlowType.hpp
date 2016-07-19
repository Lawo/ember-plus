/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWTYPE_HPP
#define __LIBEMBER_GLOW_GLOWTYPE_HPP

#include "../ber/Tag.hpp"
#include "../ber/Type.hpp"

namespace libember { namespace glow
{
    /**
     * Contains the application defined types used by glow
     */
    class GlowType
    {
        public:
            enum _Domain
            {
                Parameter = 1,
                Command = 2,
                Node = 3,
                ElementCollection = 4,
                StreamEntry = 5,
                StreamCollection = 6,
                StringIntegerPair = 7,
                StringIntegerCollection = 8,
                QualifiedParameter = 9,
                QualifiedNode = 10,
                RootElementCollection = 11,
                StreamDescriptor = 12,
                Matrix = 13,
                Target = 14,
                Source = 15,
                Connection = 16,
                QualifiedMatrix = 17,
                Label = 18,
                Function = 19,
                QualifiedFunction = 20,
                TupleItemDescription = 21,
                Invocation = 22,
                InvocationResult = 23,
                Template = 24,
                QualifiedTemplate = 25
            };

            typedef unsigned int value_type;

        public:
            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            GlowType(_Domain value);

            /**
             * Initializes a new instance.
             * @param value The value to initialize this instance with.
             */
            explicit GlowType(value_type value);

            /**
             * Returns the value.
             * @return The value.
             */
            value_type value() const;

            /**
             * Returns the glow type as ber tag which is used when the object is being encoded.
             * @return The type tag representing this glow type.
             */
            ber::Tag toTypeTag() const;

        private:
            value_type m_value;
    };

    /**************************************************************************/
    /* Inline implementation                                                  */
    /**************************************************************************/

    inline GlowType::GlowType(_Domain value)
        : m_value(value)
    {}

    inline GlowType::GlowType(value_type value)
        : m_value(value)
    {}

    inline GlowType::value_type GlowType::value() const
    {
        return m_value;
    }

    inline ber::Tag GlowType::toTypeTag() const
    {
        return ber::make_tag(ber::Class::Application, m_value);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWTYPE_HPP
