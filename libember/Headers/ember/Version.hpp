/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2019 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_VERSION_HPP
#define __LIBEMBER_VERSION_HPP

#include "util/Api.hpp"
#include "util/Inline.hpp"

#define LIBEMBER_VERSION_MAJOR      1
#define LIBEMBER_VERSION_MINOR      8
#define LIBEMBER_VERSION_REVISION   0

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

