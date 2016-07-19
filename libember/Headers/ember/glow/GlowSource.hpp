/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
