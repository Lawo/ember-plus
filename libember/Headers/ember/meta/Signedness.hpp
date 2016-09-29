/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_META_SIGNEDNESS_HPP
#define __LIBEMBER_META_SIGNEDNESS_HPP

#include "Boolean.hpp"

namespace libember { namespace meta
{
    /** A meta-function that returns whether an integral type is unsigned. */
    template<typename IntegralType>
    struct IsUnsigned : FalseType
    {};

    /** A meta-function that returns whether an integral type is signed. */
    template<typename IntegralType>
    struct IsSigned : FalseType
    {};

    /**
     * A meta-function that converts an integral type to it's unsigned
     * counterpart. If the integral type is already unsigned this meta-function
     * is the identity mapping.
     */
    template<typename IntegralType>
    struct MakeUnsigned;

    /**
     * A meta-function that converts an integral type to it's signed
     * counterpart. If the integral type is already signed this meta-function is
     * the identity mapping.
     */
    template<typename IntegralType>
    struct MakeSigned;



    template<> struct IsUnsigned<unsigned char     > : TrueType {};
    template<> struct IsUnsigned<unsigned short    > : TrueType {};
    template<> struct IsUnsigned<unsigned int      > : TrueType {};
    template<> struct IsUnsigned<unsigned long     > : TrueType {};
    template<> struct IsUnsigned<unsigned long long> : TrueType {};

    template<> struct IsSigned<char     > : TrueType {};
    template<> struct IsSigned<short    > : TrueType {};
    template<> struct IsSigned<int      > : TrueType {};
    template<> struct IsSigned<long     > : TrueType {};
    template<> struct IsSigned<long long> : TrueType {};

    #define _LIBEMBER_MAKE_UNSIGNED_SPEC_IMPL(specType, resultType)             \
        template<>                                                              \
        struct MakeUnsigned<specType>                                           \
        {                                                                       \
            typedef resultType type;                                            \
        };

    #define _LIBEMBER_MAKE_UNSIGNED_SPEC(baseType)                              \
        _LIBEMBER_MAKE_UNSIGNED_SPEC_IMPL(         baseType, unsigned baseType) \
        _LIBEMBER_MAKE_UNSIGNED_SPEC_IMPL(unsigned baseType, unsigned baseType)

    #define _LIBEMBER_MAKE_SIGNED_SPEC_IMPL(specType, resultType)               \
        template<>                                                              \
        struct MakeSigned<specType>                                             \
        {                                                                       \
            typedef resultType type;                                            \
        };

    #define _LIBEMBER_MAKE_SIGNED_SPEC(baseType)                                \
        _LIBEMBER_MAKE_SIGNED_SPEC_IMPL(unsigned baseType, baseType)            \
        _LIBEMBER_MAKE_SIGNED_SPEC_IMPL(         baseType, baseType)


    _LIBEMBER_MAKE_UNSIGNED_SPEC(char     )
    _LIBEMBER_MAKE_UNSIGNED_SPEC(short    )
    _LIBEMBER_MAKE_UNSIGNED_SPEC(int      )
    _LIBEMBER_MAKE_UNSIGNED_SPEC(long     )
    _LIBEMBER_MAKE_UNSIGNED_SPEC(long long)

    _LIBEMBER_MAKE_SIGNED_SPEC(char     )
    _LIBEMBER_MAKE_SIGNED_SPEC(short    )
    _LIBEMBER_MAKE_SIGNED_SPEC(int      )
    _LIBEMBER_MAKE_SIGNED_SPEC(long     )
    _LIBEMBER_MAKE_SIGNED_SPEC(long long)

    #undef _LIBEMBER_MAKE_SIGNED_SPEC
    #undef _LIBEMBER_MAKE_SIGNED_SPEC_IMPL
    #undef _LIBEMBER_MAKE_UNSIGNED_SPEC
    #undef _LIBEMBER_MAKE_UNSIGNED_SPEC_IMPL
}
}

#endif  // __LIBEMBER_META_SIGNEDNESS_HPP

