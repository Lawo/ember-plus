/*
    libs101 -- C++ 03 implementation of the S101 encoding and decoding

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBS101_UTIL_ENUMCAST_HPP
#define __LIBS101_UTIL_ENUMCAST_HPP

namespace libs101 { namespace util
{
    template<typename EnumType>
    inline EnumType enum_cast(typename EnumType::value_type value)
    {
        return static_cast<EnumType>(value);
    }
}
}

#endif  // __LIBS101_UTIL_ENUMCAST_HPP

