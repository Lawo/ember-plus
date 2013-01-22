/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012  L-S-B Broadcast Technologies GmbH

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

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

