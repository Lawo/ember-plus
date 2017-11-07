/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWDTD_HPP
#define __LIBEMBER_GLOW_GLOWDTD_HPP

#include "../util/Api.hpp"

#define LIBEMBER_GLOWDTD_VERSION_MAJOR    2
#define LIBEMBER_GLOWDTD_VERSION_MINOR    50

namespace libember { namespace glow
{ 
    /**
     * Provides methods to retrieve the current dtd version
     */
    struct LIBEMBER_API GlowDtd
    {
         /**
          * Returns the current dtd version, where the high byte contains the major version
          * and the lower byte the minor version.
          * @return The current dtd version.
          */
         static unsigned short version();
    };
}
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/GlowDtd.ipp"
#endif

#endif //  __LIBEMBER_GLOW_GLOWDTD_HPP
