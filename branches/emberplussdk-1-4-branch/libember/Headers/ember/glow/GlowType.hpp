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
                InvocationResult = 23
            };

            typedef unsigned int value_type;

        public:
            /**
             * Constructor 
             * @param type The glow type to initialize this instance with.
             */
            GlowType(_Domain type);

            /**
             * Returns the type identifier.
             * @return Returns the type identifier.
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

    inline GlowType::GlowType(_Domain type)
        : m_value(type)
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
