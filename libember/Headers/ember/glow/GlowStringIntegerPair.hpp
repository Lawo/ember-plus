/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_HPP
#define __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_HPP

#include "GlowContainer.hpp"

namespace libember { namespace glow
{
    /**
     * A GlowStringIntegerPair represents a single enumeration entry which contains a name and a value.
     */
    class LIBEMBER_API GlowStringIntegerPair : public GlowContainer
    {
        public:
            /**
             * Constructor, initializes an enumeration entry with the application tag specified.
             * @param tag Application tag.
             */
            explicit GlowStringIntegerPair(ber::Tag const& tag);

            /**
             * Constructor, initializes a enumeration entry with a provided name and value.
             * @param name Enumeration entry name.
             * @param value Enumeration entry value.
             */
            GlowStringIntegerPair(std::string const& name, int value);

            /**
             * Returns the name of the enumeration entry.
             * @return The enumeration entry name or an empty string if it doesn't exist.
             */
            std::string name() const;

            /**
             * Returns the value of the enumeration entry.
             * @return The enumeration entry value
             */
            int value() const;
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowStringIntegerPair.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSTRINGINTEGERPAIR_HPP
