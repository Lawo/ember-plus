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

#ifndef __LIBEMBER_GLOW_GLOWSOURCE_HPP
#define __LIBEMBER_GLOW_GLOWSOURCE_HPP

#include "GlowSignal.hpp"

namespace libember { namespace glow
{
    /** Forward declarations **/
    class GlowNodeFactory;

    /**
     * Class representing a source description.
     */
    class LIBEMBER_API GlowSource : public GlowSignal
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Initializes a new instance of GlowSource
             * @param number The signal number.
             */
            explicit GlowSource(int number);

            /**
             * Initializes a new instance of GlowSource
             * @param number The signal number.
             * @param tag The application tag to set
             */
            GlowSource(int number, ber::Tag const& tag);

        private:
            /**
             * Initializes a new instance of GlowSource
             * @param tag The application tag to set
             */
            explicit GlowSource(ber::Tag const& tag);
    };

    /******************************************************
     * Inline Implementation                              *
     ******************************************************/

    inline GlowSource::GlowSource(int number, ber::Tag const& tag)
        : GlowSignal(GlowType::Source, tag, number)
    {}

    inline GlowSource::GlowSource(ber::Tag const& tag)
        : GlowSignal(GlowType::Source, tag)
    {}

    inline GlowSource::GlowSource(int number)
        : GlowSignal(GlowType::Source, number)
    {}
}
}


#endif  // __LIBEMBER_GLOW_GLOWSOURCE_HPP
