/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWCONTENTELEMENT_IPP
#define __LIBEMBER_GLOW_GLOWCONTENTELEMENT_IPP

#include <algorithm>
#include "../../util/Inline.hpp"
#include "../traits/ValueTypeToBerType.hpp"
#include "../GlowTags.hpp"


namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    Contents::Contents(GlowContentElement& parent, ber::Tag const& contentTag)
        : m_parent(parent)
        , m_contentTag(contentTag)
        , m_propertyFlags(0)
        , m_container(0)
    {}

    LIBEMBER_INLINE
    void Contents::assureContainer() const
    {
        if (m_container == 0)
        {
            iterator const first = m_parent.begin();
            iterator const last = m_parent.end();
            iterator const it = util::find_tag(first, last, m_contentTag);

            if (it == last)
            {
                m_container = new dom::Set(m_contentTag);
                m_parent.insert(last, m_container);
            }
            else
            {
                m_container = &dynamic_cast<dom::Set&>(*it);

                if (m_container != 0)
                {
                    m_propertyFlags = generatePropertyFlags(m_container->begin(), m_container->end());
                }
            }
        }
    }

    LIBEMBER_INLINE
    Contents::flag_type Contents::generatePropertyFlags(const_iterator first, const_iterator last) const
    {
        flag_type flags = 0;

        for(; first != last; ++first)
        {
            ber::Tag const tag = first->applicationTag();

            if (tag.getClass() == ber::Class::ContextSpecific)
                flags |= (1 << tag.number());
        }

        return flags;
    }

    LIBEMBER_INLINE
    Contents::iterator Contents::begin()
    {
        assureContainer();
        return m_container->begin();
    }

    LIBEMBER_INLINE
    Contents::iterator Contents::end()
    {
        assureContainer();
        return m_container->end();
    }

    LIBEMBER_INLINE
    Contents::const_iterator Contents::begin() const
    {
        assureContainer();
        return m_container->begin();
    }

    LIBEMBER_INLINE
    Contents::const_iterator Contents::end() const
    {
        assureContainer();
        return m_container->end();
    }

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4355)
#endif
    LIBEMBER_INLINE
    GlowContentElement::GlowContentElement(GlowType const& type, ber::Tag const& tag, ber::Tag const& contentTag)
        : GlowElement(type, tag)
        , m_contents(*this, contentTag)
    {}
#ifdef _MSC_VER
#  pragma warning(pop)
#endif

    LIBEMBER_INLINE
    Contents& GlowContentElement::contents() 
    {
        return m_contents;
    }

    LIBEMBER_INLINE
    const Contents& GlowContentElement::contents() const
    {
        return m_contents;
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWCONTENTELEMENT_IPP
