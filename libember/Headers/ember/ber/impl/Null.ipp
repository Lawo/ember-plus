/*
libember -- C++ 03 implementation of the Ember+ Protocol

Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_BER_IMPL_NULL_IPP
#define __LIBEMBER_BER_IMPL_NULL_IPP

#include "../../util/Inline.hpp"

namespace libember { namespace ber 
{
    LIBEMBER_INLINE
    Null::Null()
    {

    }

    LIBEMBER_INLINE
    Null::Null(Null const&)
    {
    }

    LIBEMBER_INLINE
    Null& Null::operator=(Null const&)
    {
        return *this;
    }
}
}

#endif//__LIBEMBER_BER_IMPL_NULL_IPP
