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

#ifndef __LIBEMBER_UTIL_TYPEPUN_HPP
#define __LIBEMBER_UTIL_TYPEPUN_HPP

namespace libember { namespace util
{
    /**
     * A helper function that performs type punning between on a value
     * without breaking the strict aliasing rules.
     * @param source the value to pun.
     * @return The punned value.
     * @note Even though this function takes care of aliasing type punning
     *      still invokes undefined behaviour and is thus unsafe and may
     *      break unexpectedly.
     */
    template<typename DestinationType, typename SourceType>
    inline DestinationType type_pun(SourceType source)
    {
        union
        {
            SourceType      source;
            DestinationType result;
        } pun;

        pun.source = source;
        return pun.result;
    }
}
}

#endif // __LIBEMBER_UTIL_TYPEPUN_HPP


