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

