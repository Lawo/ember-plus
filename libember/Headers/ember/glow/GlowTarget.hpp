/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWTARGET_HPP
#define __LIBEMBER_GLOW_GLOWTARGET_HPP

#include "GlowSignal.hpp"

namespace libember { namespace glow
{
    /** Forward declarations **/
    class GlowNodeFactory;

    /**
     * Class representing a target description.
     */
    class LIBEMBER_API GlowTarget : public GlowSignal
    {
        friend class GlowNodeFactory;
        public:
            /**
             * Initializes a new instance of GlowTarget
             * @param number The signal number.
             */
            explicit GlowTarget(int number);

            /**
             * Initializes a new instance of GlowTarget
             * @param number The signal number.
             * @param tag The application tag to set
             */
            GlowTarget(int number, ber::Tag const& tag);

        private:
            /**
             * Initializes a new instance of GlowTarget
             * @param tag The application tag to set
             */
            explicit GlowTarget(ber::Tag const& tag);
    };

    /******************************************************
     * Inline Implementation                              *
     ******************************************************/

    inline GlowTarget::GlowTarget(int number, ber::Tag const& tag)
        : GlowSignal(GlowType::Target, tag, number)
    {}

    inline GlowTarget::GlowTarget(ber::Tag const& tag)
        : GlowSignal(GlowType::Target, tag)
    {}

    inline GlowTarget::GlowTarget(int number)
        : GlowSignal(GlowType::Target, number)
    {}
}
}


#endif  // __LIBEMBER_GLOW_GLOWTARGET_HPP
