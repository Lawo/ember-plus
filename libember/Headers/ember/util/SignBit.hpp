/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2024 Lawo AG (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_UTIL_SIGNBIT_HPP
#define __LIBEMBER_UTIL_SIGNBIT_HPP

#include "TypePun.hpp"

namespace libember { namespace util
{
    inline bool signbit(float value)
    {
        return (type_pun<unsigned int>(value) & 0x80000000U) != 0U;
    }

    inline bool signbit(double value)
    {
        return (type_pun<unsigned long long>(value) & 0x8000000000000000LLU) != 0LLU;
    }
}
}

#endif // __LIBEMBER_UTIL_SIGNBIT_HPP
