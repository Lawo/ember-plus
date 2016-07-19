/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_UTIL_TYPEPUN_HPP
#define __LIBEMBER_UTIL_TYPEPUN_HPP

namespace libember { namespace util
{
    /**
     * A helper function that performs type punning between on a value
     * without breaking the strict aliasing rules.
     * @param source the value to pun.
     * @return The punned value.
     * @note Even though this function takes care of aliasing type punning
     *      still invokes undefined behaviour and is thus unsafe and may
     *      break unexpectedly.
     */
    template<typename DestinationType, typename SourceType>
    inline DestinationType type_pun(SourceType source)
    {
        union
        {
            SourceType      source;
            DestinationType result;
        } pun;

        pun.source = source;
        return pun.result;
    }
}
}

#endif // __LIBEMBER_UTIL_TYPEPUN_HPP


