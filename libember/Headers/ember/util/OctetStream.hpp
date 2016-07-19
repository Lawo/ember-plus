/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_UTIL_OCTETSTREAM_HPP
#define __LIBEMBER_UTIL_OCTETSTREAM_HPP

#include "StreamBuffer.hpp"

namespace libember { namespace util
{
    /**
     * Specialization of the StreamBuffer template for byte streams.
     */
    typedef StreamBuffer<unsigned char> OctetStream;
}
}

#endif  // __LIBEMBER_UTIL_OCTETSTREAM_HPP

