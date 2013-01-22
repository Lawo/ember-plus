#ifndef __LIBEMBER_META_ISSAME_HPP
#define __LIBEMBER_META_ISSAME_HPP

#include "Boolean.hpp"

namespace libember { namespace meta
{
    /**
     * A binary meta-function that determines whether its two argument types
     * specify the same type.
     */
    template<typename U, typename V>
    struct IsSame
        : FalseType
    {};

    template<typename T>
    struct IsSame<T, T>
        : TrueType
    {};
}
}

#endif // __LIBEMBER_META_ISSAME_HPP

