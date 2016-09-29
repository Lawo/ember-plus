/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_META_INTTOTYPE_HPP
#define __LIBEMBER_META_INTTOTYPE_HPP

namespace libember { namespace meta
{
    template<int I>
    struct IntToType
    {
        enum
        {
                                    value = I
        };
        typedef int                 value_type;

        typedef IntToType<I>        type;
        typedef IntToType<I + 1>    next;
        typedef IntToType<I - 1>    previous;
    };
}
}

#endif  // __LIBEMBER_META_INTTOTYPE_HPP

