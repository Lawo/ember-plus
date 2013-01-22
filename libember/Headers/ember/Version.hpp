#ifndef __LIBEMBER_VERSION_HPP
#define __LIBEMBER_VERSION_HPP

#include "util/Api.hpp"
#include "util/Inline.hpp"

#define LIBEMBER_VERSION_MAJOR      0
#define LIBEMBER_VERSION_MINOR      6
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

