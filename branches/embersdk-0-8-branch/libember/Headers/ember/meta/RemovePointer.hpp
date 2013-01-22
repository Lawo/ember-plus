#ifndef __LIBEMBER_META_REMOVEPOINTER_HPP
#define __LIBEMBER_META_REMOVEPOINTER_HPP

namespace libember { namespace meta
{
    /**
     * An unary meta-function that removes a possible pointer declaration from
     * the passed type.
     */
    template<typename T>
    struct RemovePointer
    {
        typedef T type;
    };


    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/


    template<typename T>
    struct RemovePointer<T*>
    {
        typedef T type;
    };
}
}

#endif  // __LIBEMBER_META_REMOVEPOINTER_HPP

