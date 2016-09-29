/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_UTIL_ENUM_CAST_HPP
#define __LIBEMBER_UTIL_ENUM_CAST_HPP

namespace libember { namespace util
{
    template<typename EnumType>
    inline EnumType enum_cast(typename EnumType::value_type value)
    {
        return static_cast<EnumType>(value);
    }
}
}

#endif // __LIBEMBER_UTIL_ENUM_CAST_HPP


