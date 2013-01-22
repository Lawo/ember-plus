#ifndef __LIBEMBER_META_INTTOTYPE_HPP
#define __LIBEMBER_META_INTTOTYPE_HPP

namespace libember { namespace meta
{
    template<int I>
    struct IntToType
    {
        enum
        {
                                    value = I
        };
        typedef int                 value_type;

        typedef IntToType<I>        type;
        typedef IntToType<I + 1>    next;
        typedef IntToType<I - 1>    previous;
    };
}
}

#endif  // __LIBEMBER_META_INTTOTYPE_HPP

