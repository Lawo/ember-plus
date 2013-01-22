#ifndef __LIBEMBER_META_ENABLEIF_HPP
#define __LIBEMBER_META_ENABLEIF_HPP

namespace libember { namespace meta
{
    /**
     * A binary metafunction that may be used to reduce the overload sets for
     * which a generic function is considered, by using SFINAE to check certain
     * conditions at compile time.
     */
    template<bool Condition, typename ResultType = void>
    struct EnableIf_C
    {};
    
    template <typename ResultType>
    struct EnableIf_C<true, ResultType>
    {
        typedef ResultType type;
    };

    /**
     * A binary metafunction that may be used to reduce the overload sets for
     * which a generic function is considered, by using SFINAE to check certain
     * conditions at compile time. This version may be used with eagerly or
     * lazily evaluated meta-functions.
     */
    template<typename Condition, typename ResultType = void>
    struct EnableIf
        : EnableIf_C<Condition::type::value, ResultType>
    {};
}
}

#endif  // __LIBEMBER_META_ENABLEIF_HPP

