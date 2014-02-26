/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

