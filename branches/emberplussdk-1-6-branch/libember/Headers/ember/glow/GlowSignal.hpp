/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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
