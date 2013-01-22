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

#ifndef __LIBEMBER_GLOW_GLOWDTD_HPP
#define __LIBEMBER_GLOW_GLOWDTD_HPP

#include "../util/Api.hpp"

#define LIBEMBER_GLOWDTD_VERSION_MAJOR    2
#define LIBEMBER_GLOWDTD_VERSION_MINOR    10

namespace libember { namespace glow
{ 
    /**
     * Provides methods to retreive the current dtd version
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
