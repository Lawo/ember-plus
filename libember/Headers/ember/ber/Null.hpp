/*
libember -- C++ 03 implementation of the Ember+ Protocol

Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_NULL_HPP
#define __LIBEMBER_BER_NULL_HPP

#include <vector>
#include "../util/Api.hpp"

namespace libember { namespace ber
{
    /**
     * Represents the null value, which is a universal tag with type number 5 (Null).
     */
    class LIBEMBER_API Null
    {
    public:
        Null();

        Null(Null const&);

        Null& operator=(Null const&);
    };

    inline bool operator==(Null const&, Null const&)
    {
        return true;
    }

    inline bool operator!=(Null const&, Null const&)
    {
        return false;
    }
}
}


#ifdef LIBEMBER_HEADER_ONLY
#   include "impl/Null.ipp"
#endif

#endif//__LIBEMBER_BER_NULL_HPP

