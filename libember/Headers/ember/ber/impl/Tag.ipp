/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
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

