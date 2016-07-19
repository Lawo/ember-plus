/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_META_FUNCTIONTRAITS_HPP
#define __LIBEMBER_META_FUNCTIONTRAITS_HPP

#include "IntToType.hpp"

//SimianIgnore

namespace libember { namespace meta
{
    template<typename T>
    struct FunctionTraits {};
    
    template<typename R>
    struct FunctionTraits<R (*)()>
    {
        typedef IntToType<0>        arity;
        typedef R                   result_type;
    };

    template<typename R, typename A1>
    struct FunctionTraits<R (*)(A1)>
    {
        typedef IntToType<1>        arity;
        typedef R                   result_type;
        typedef A1                  arg1_type;
    };

    template<typename R, typename A1,typename A2>
    struct FunctionTraits<R (*)(A1, A2)>
    {
        typedef IntToType<2>        arity;
        typedef R                   result_type;
        typedef A1                  arg1_type;
        typedef A2                  arg2_type;
    };

    template<typename R, typename A1,typename A2, typename A3>
    struct FunctionTraits<R (*)(A1, A2, A3)>
    {
        typedef IntToType<3>        arity;
        typedef R                   result_type;
        typedef A1                  arg1_type;
        typedef A2                  arg2_type;
        typedef A3                  arg3_type;
    };

    template<typename R, typename A1,typename A2, typename A3, typename A4>
    struct FunctionTraits<R (*)(A1, A2, A3, A4)>
    {
        typedef IntToType<4>        arity;
        typedef R                   result_type;
        typedef A1                  arg1_type;
        typedef A2                  arg2_type;
        typedef A3                  arg3_type;
        typedef A4                  arg4_type;
    };

    template<typename R, typename A1,typename A2, typename A3, typename A4, typename A5>
    struct FunctionTraits<R (*)(A1, A2, A3, A4, A5)>
    {
        typedef IntToType<5>        arity;
        typedef R                   result_type;
        typedef A1                  arg1_type;
        typedef A2                  arg2_type;
        typedef A3                  arg3_type;
        typedef A4                  arg4_type;
        typedef A5                  arg5_type;
    };

    template<typename R, typename A1,typename A2, typename A3, typename A4, typename A5, typename A6>
    struct FunctionTraits<R (*)(A1, A2, A3, A4, A5, A6)>
    {
        typedef IntToType<6>        arity;
        typedef R                   result_type;
        typedef A1                  arg1_type;
        typedef A2                  arg2_type;
        typedef A3                  arg3_type;
        typedef A4                  arg4_type;
        typedef A5                  arg5_type;
        typedef A6                  arg6_type;
    };

    template<typename R, typename A1,typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    struct FunctionTraits<R (*)(A1, A2, A3, A4, A5, A6, A7)>
    {
        typedef IntToType<7>        arity;
        typedef R                   result_type;
        typedef A1                  arg1_type;
        typedef A2                  arg2_type;
        typedef A3                  arg3_type;
        typedef A4                  arg4_type;
        typedef A5                  arg5_type;
        typedef A6                  arg6_type;
        typedef A7                  arg7_type;
    };

    template<typename R, typename A1,typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    struct FunctionTraits<R (*)(A1, A2, A3, A4, A5, A6, A7, A8)>
    {
        typedef IntToType<8>        arity;
        typedef R                   result_type;
        typedef A1                  arg1_type;
        typedef A2                  arg2_type;
        typedef A3                  arg3_type;
        typedef A4                  arg4_type;
        typedef A5                  arg5_type;
        typedef A6                  arg6_type;
        typedef A7                  arg7_type;
        typedef A8                  arg8_type;
    };

}
}

//EndSimianIgnore

#endif  // __LIBEMBER_META_FUNCTIONTRAITS_HPP

