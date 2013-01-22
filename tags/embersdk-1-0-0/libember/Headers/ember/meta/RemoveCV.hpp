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

