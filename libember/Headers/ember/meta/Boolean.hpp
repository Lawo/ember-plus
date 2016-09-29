/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_META_BOOLEAN_HPP
#define __LIBEMBER_META_BOOLEAN_HPP

namespace libember { namespace meta
{
    template<bool Constant>
    struct Boolean
    {
        typedef Boolean     type;
        static bool const   value = Constant;
        
        operator bool() const
        {
            return value;
        }
    };

    template<bool Constant>
    bool const Boolean<Constant>::value;

    typedef Boolean<true>   TrueType;
    typedef Boolean<false>  FalseType;
}
}

#endif // __LIBEMBER_META_BOOLEAN_HPP

