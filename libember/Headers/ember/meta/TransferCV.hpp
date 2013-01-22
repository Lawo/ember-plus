#ifndef __LIBEMBER_META_TRANSFERCV_HPP
#define __LIBEMBER_META_TRANSFERCV_HPP

namespace libember { namespace meta
{
    /**
     * An binary meta-function that transfers a possible const qualification
     * from the source type to the destination type.
     */
    template<typename SourceType, typename DestType>
    struct TransferConst
    {
        typedef DestType type;
    };

    /**
     * An binary meta-function that transfers a possible volatile qualification
     * from the source type to the destination type.
     */
    template<typename SourceType, typename DestType>
    struct TransferVolatile
    {
        typedef DestType type;
    };

    /**
     * An binary meta-function that transfers a possible const and/or volatile
     * qualification from the source type to the destination type.
     */
    template<typename SourceType, typename DestType>
    struct TransferCV
        : TransferConst<SourceType, typename TransferVolatile<SourceType, DestType>::type>
    {};
    


    /**************************************************************************/
    /* Mandatory inline implementation                                        */
    /**************************************************************************/


    template<typename SourceType, typename DestType>
    struct TransferConst<SourceType const, DestType>
    {
        typedef DestType const type;
    };

    template<typename SourceType, typename DestType>
    struct TransferVolatile<SourceType volatile, DestType>
    {
        typedef DestType volatile type;
    };

}
}

#endif  // __LIBEMBER_META_TRANSFERCV_HPP

