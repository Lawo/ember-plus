/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWDTD_IPP
#define __LIBEMBER_GLOW_GLOWDTD_IPP

#include "../../util/Inline.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    unsigned short GlowDtd::version()
    {
        return (LIBEMBER_GLOWDTD_VERSION_MAJOR << 8) | LIBEMBER_GLOWDTD_VERSION_MINOR;
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWDTD_IPP
