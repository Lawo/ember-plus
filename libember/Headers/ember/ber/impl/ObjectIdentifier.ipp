/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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
    void ObjectIdentifier::push_back(value_type value)
    {
        m_items.push_back(value);
    }

    LIBEMBER_INLINE
    void ObjectIdentifier::push_front(value_type value)
    {
        m_items.push_front(value);
    }

    LIBEMBER_INLINE
    bool operator==(ObjectIdentifier const& lhs, ObjectIdentifier const& rhs)
    {
        return ((lhs.size() == rhs.size()) && std::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }
}
}

#endif // __LIBEMBER_BER_IMPL_OBJECTIDENTIFIER_IPP

