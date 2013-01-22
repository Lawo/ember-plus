#ifndef __LIBEMBER_GLOW_GLOWCONTENTELEMENT_IPP
#define __LIBEMBER_GLOW_GLOWCONTENTELEMENT_IPP

#include <algorithm>
#include "../../util/Inline.hpp"
#include "../detail/CompareNodeTag.hpp"
#include "../traits/ValueTypeToBerType.hpp"
#include "../GlowTags.hpp"
#include "../GlowProperty.hpp"


namespace libember { namespace glow {
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
            iterator const begin = m_parent.begin();
            iterator const end = m_parent.end();
            iterator const it = std::find_if(begin, end, detail::CompareNodeTag(m_contentTag));

            if (it == end)
            {
                m_container = new dom::Set(m_contentTag);
                m_parent.insert(end, m_container);
            }
            else
            {
                m_container = &dynamic_cast<dom::Set&>(*it);
                m_propertyFlags = generatePropertyFlags(m_container->begin(), m_container->end());
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

            if (tag.getClass() == ber::Class::Application)
                flags |= (1 << tag.number());
        }

        return flags;
    }

    LIBEMBER_INLINE
    bool Contents::contains(GlowProperty const& property) const
    {
        return (m_propertyFlags & (1 << property.value)) != 0;
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

    LIBEMBER_INLINE
    void Contents::insert(iterator where, dom::Node* child)
    {
        assureContainer();
        
        ber::Tag const tag = child->applicationTag();

        m_propertyFlags |= (tag.getClass() == ber::Class::Application ? (1 << tag.number()) : 0);
        m_container->insert(where, child);
    }




    LIBEMBER_INLINE
    GlowContentElement::GlowContentElement(GlowType const& type, ber::Tag const& contentTag)
        : GlowElement(type)
        , m_contents(*this, contentTag)
    {}

    LIBEMBER_INLINE
    GlowContentElement::GlowContentElement(GlowType const& type, ber::Tag const& contentTag, ber::Tag const& tag)
        : GlowElement(type, tag)
        , m_contents(*this, contentTag)
    {
    }

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
