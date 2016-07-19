/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_META_MAXSIZE_HPP
#define __LIBEMBER_META_MAXSIZE_HPP

namespace libember { namespace meta
{
    /**
     * A meta-function that either returns the provided value
     * or the maximum value of the current type if 0 is set.
     */
    template<unsigned int Value>
    struct MaxSize
    {
        static const unsigned int value = Value;
    };

    template<>
    struct MaxSize<0>
    {
        static const unsigned int value = 0xFFFFFFFF;
    };
}
}

#endif // __LIBEMBER_META_MAXSIZE_HPP

