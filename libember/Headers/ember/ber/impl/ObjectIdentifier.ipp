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

#ifndef __LIBEMBER_BER_IMPL_OBJECTIDENTIFIER_IPP
#define __LIBEMBER_BER_IMPL_OBJECTIDENTIFIER_IPP

#include <algorithm>
#include "../../util/Inline.hpp"

namespace libember { namespace ber
{
    LIBEMBER_INLINE
    ObjectIdentifier::ObjectIdentifier(value_type value)
        : m_items(1, value)
    {}

    LIBEMBER_INLINE
    ObjectIdentifier::ObjectIdentifier()
    {}

    LIBEMBER_INLINE
    void ObjectIdentifier::swap(ObjectIdentifier& other)
    {
        using std::swap;
        swap(m_items, other.m_items);
    }

    LIBEMBER_INLINE
    ObjectIdentifier::value_type ObjectIdentifier::operator[](int index) const
    {
        return m_items[index];
    }

    LIBEMBER_INLINE
    bool operator==(ObjectIdentifier const& lhs, ObjectIdentifier const& rhs)
    {
        return ((lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }
}
}

#endif // __LIBEMBER_BER_IMPL_OBJECTIDENTIFIER_IPP

