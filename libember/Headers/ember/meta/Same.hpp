/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_META_SAME_HPP
#define __LIBEMBER_META_SAME_HPP

#include "Boolean.hpp"

namespace libember { namespace meta
{

    /**
     * A binary meta-function that returns whether or not the two
     * supplied types are the same.
     */
    template<typename U, typename V>
    struct IsSame : FalseType
    {};

    template<typename U>
    struct IsSame<U, U> : TrueType
    {};
}
}

#endif  // __LIBEMBER_META_SAME_HPP

