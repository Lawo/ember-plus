/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWSIGNAL_HPP
#define __LIBEMBER_GLOW_GLOWSIGNAL_HPP

#include "GlowContainer.hpp"

namespace libember { namespace glow
{
    /**
     * GlowSignal is the base class for GlowTarget and GlowSource
     */
    class LIBEMBER_API GlowSignal : public GlowContainer
    {
        public:
            /**
             * Returns the signal number.
             * @return The signal number.
             */
            int number() const;

        protected:
            /**
             * Initializes a new instance of GlowSignal
             * @param type The type of the instance. Either target or source.
             * @param tag The application tag to set
             */
            GlowSignal(GlowType const& type, ber::Tag const& tag);

            /**
             * Initializes a new instance of GlowSignal
             * @param type The type of the instance. Either target or source.
             * @param number The signal number.
             */
            GlowSignal(GlowType const& type, int number);

            /**
             * Initializes a new instance of GlowSignal
             * @param type The type of the instance. Either target or source.
             * @param tag The application tag to set
             * @param number The signal number.
             */
            GlowSignal(GlowType const& type, ber::Tag const& tag, int number);
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowSignal.ipp"
#endif

#endif  // __LIBEMBER_GLOW_GLOWSIGNAL_HPP
