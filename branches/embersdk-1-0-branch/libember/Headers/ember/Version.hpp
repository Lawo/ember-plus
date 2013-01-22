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

#ifndef __LIBEMBER_VERSION_HPP
#define __LIBEMBER_VERSION_HPP

#include "util/Api.hpp"
#include "util/Inline.hpp"

#define LIBEMBER_VERSION_MAJOR      1
#define LIBEMBER_VERSION_MINOR      0
#define LIBEMBER_VERSION_REVISION   1

namespace libember
{
#ifndef LIBEMBER_HEADER_ONLY
    LIBEMBER_API
    LIBEMBER_EXTERN 
    char const* const versionString;
#endif
}

#ifdef LIBEMBER_HEADER_ONLY
#  include "impl/Version.ipp"
#endif

#endif  // __LIBEMBER_VERSION_HPP

