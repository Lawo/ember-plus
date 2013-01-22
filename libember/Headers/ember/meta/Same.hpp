#ifndef __LIBEMBER_META_SAME_HPP
#define __LIBEMBER_META_SAME_HPP

#include "Boolean.hpp"

namespace libember { namespace meta
{

    /**
     * A binary meta-function that returns whether or not the two
     * supplied types are the same.
     */
    template<typename U, typename V>
    struct IsSame : FalseType
    {};

    template<typename U>
    struct IsSame<U, U> : TrueType
    {};
}
}

#endif  // __LIBEMBER_META_SAME_HPP

