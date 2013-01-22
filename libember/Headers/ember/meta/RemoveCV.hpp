#ifndef __LIBEMBER_META_REMOVECV_HPP
#define __LIBEMBER_META_REMOVECV_HPP

namespace libember { namespace meta
{
    /**
     * An unary meta-function that removes a possible const qualification from
     * the passed type.
     */
    template<typename T>
    struct RemoveConst
    {
        typedef T type;
    };

    /**
     * An unary meta-function that removes a possible volatile qualification from
     * the passed type.
     */
    template<typename T>
    struct RemoveVolatile
    {
        typedef T type;
    };

    /**
     * An unary meta-function that removes possible const and/or volatile
     * qualifications from the passed type.
     */
    template<typename T>
    struct RemoveCV
        : RemoveConst<typename RemoveVolatile<T>::type>
    {};
    


    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/


    template<typename T>
    struct RemoveConst<T const>
    {
        typedef T type;
    };

    template<typename T>
    struct RemoveVolatile<T volatile>
    {
        typedef T type;
    };

}
}

#endif  // __LIBEMBER_META_REMOVECV_HPP

