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

#ifndef __LIBEMBER_BER_IMPL_TAG_IPP
#define __LIBEMBER_BER_IMPL_TAG_IPP

#include <iomanip>
#include "../../util/Inline.hpp"

namespace libember { namespace ber
{
    namespace detail
    {
        Tag::Preamble const TagContainer = 0x20;
    }

    LIBEMBER_INLINE
    Tag::Tag()
        : m_preamble(0), m_number(0)
    {}

    LIBEMBER_INLINE
    Tag::Tag(Class berClass, Number tagNumber)
        : m_preamble(berClass.value() & Class::Mask), m_number(tagNumber)
    {}

    LIBEMBER_INLINE
    Tag::Tag(Preamble preamble, Number number)
        : m_preamble(preamble & 0xE0U), m_number(number)
    {}
        
    LIBEMBER_INLINE
    Tag::Preamble Tag::preamble() const
    {
        return m_preamble;
    }

    LIBEMBER_INLINE
    Tag::Number Tag::number() const
    {
        return m_number;
    }

    LIBEMBER_INLINE
    bool Tag::isContainer() const
    {
        return ((m_preamble & detail::TagContainer) != 0);
    }

    LIBEMBER_INLINE
    void Tag::setContainer(bool value)
    {
        if (value)
            m_preamble |=  detail::TagContainer;
        else
            m_preamble &= ~ detail::TagContainer;
    }

    LIBEMBER_INLINE
    Tag Tag::toContainer() const
    {
        return Tag(static_cast<Preamble>(m_preamble |  detail::TagContainer), m_number);
    }

    LIBEMBER_INLINE
    Class Tag::getClass() const
    {
        return static_cast<Class::_Domain>(m_preamble & Class::Mask);
    }

    LIBEMBER_INLINE
    void Tag::setClass(Class berClass)
    {
        m_preamble &= ~Class::Mask;
        m_preamble |= berClass.value() & Class::Mask;
    }

    LIBEMBER_API
    LIBEMBER_INLINE
    Tag make_tag(Tag::Preamble preamble, Tag::Number number)
    {
        return Tag(preamble, number);
    }

    LIBEMBER_API
    LIBEMBER_INLINE
    bool operator==(Tag const& lhs, Tag const& rhs)
    {
        return (lhs.preamble() == rhs.preamble()) && (lhs.number() == rhs.number());
    }

    LIBEMBER_API
    LIBEMBER_INLINE
    bool operator!=(Tag const& lhs, Tag const& rhs)
    {
        return (lhs.preamble() != rhs.preamble()) || (lhs.number() != rhs.number());
    }

    LIBEMBER_API
    LIBEMBER_INLINE
    bool operator<(Tag const& lhs, Tag const& rhs)
    {
        return (lhs.preamble() < rhs.preamble()) || 
            ((lhs.preamble() == rhs.preamble()) && (lhs.number() < rhs.number()));
    }

    LIBEMBER_API
    LIBEMBER_INLINE
    bool operator<=(Tag const& lhs, Tag const& rhs)
    {
        return (lhs.preamble() < rhs.preamble()) || 
            ((lhs.preamble() == rhs.preamble()) && (lhs.number() <= rhs.number()));
    }

    LIBEMBER_API
    LIBEMBER_INLINE
    bool operator>(Tag const& lhs, Tag const& rhs)
    {
        return (rhs.preamble() < lhs.preamble()) || 
            ((rhs.preamble() == lhs.preamble()) && (rhs.number() < lhs.number()));
    }

    LIBEMBER_API
    LIBEMBER_INLINE
    bool operator>=(Tag const& lhs, Tag const& rhs)
    {
        return (rhs.preamble() < lhs.preamble()) || 
            ((rhs.preamble() == lhs.preamble()) && (rhs.number() <= lhs.number()));
    }
}
}

#endif // __LIBEMBER_IMPL_TAG_IPP

