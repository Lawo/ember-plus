/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWPARAMETER_IPP
#define __LIBEMBER_GLOW_GLOWPARAMETER_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowStringIntegerPair.hpp"
#include "../GlowTags.hpp"
#include "../GlowNodeBase.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowParameter::GlowParameter(int number)
        : GlowParameterBase(GlowType::Parameter, GlowTags::ElementDefault(), GlowTags::Parameter::Contents(), GlowTags::Parameter::Children())
        , m_cachedNumber(-1)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Parameter::Number(), number));
    }

    LIBEMBER_INLINE
    GlowParameter::GlowParameter(GlowNodeBase* parent, int number)
        : GlowParameterBase(GlowType::Parameter, GlowTags::ElementDefault(), GlowTags::Parameter::Contents(), GlowTags::Parameter::Children())
        , m_cachedNumber(-1)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Parameter::Number(), number));
        if (parent)
        {
            GlowElementCollection* children = parent->children();
            GlowElementCollection::iterator const where = children->end();
            children->insert(where, this);
        }
    }
    
    LIBEMBER_INLINE
    GlowParameter::GlowParameter(int number, ber::Tag const& tag)
        : GlowParameterBase(GlowType::Parameter, tag, GlowTags::Parameter::Contents(), GlowTags::Parameter::Children())
        , m_cachedNumber(-1)
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::Parameter::Number(), number));
    }

    LIBEMBER_INLINE
    GlowParameter::GlowParameter(ber::Tag const& tag)
        : GlowParameterBase(GlowType::Parameter, tag, GlowTags::Parameter::Contents(), GlowTags::Parameter::Children())
        , m_cachedNumber(-1)
    {}

    LIBEMBER_INLINE
    int GlowParameter::number() const
    {
        if (m_cachedNumber == -1)
        {
            ber::Tag const tag = GlowTags::Parameter::Number();
            const_iterator const first = begin();
            const_iterator const last = end();
            const_iterator const result = util::find_tag(first, last, tag);
            if (result != last)
            {
                m_cachedNumber = util::ValueConverter::valueOf(&*result, -1);
            }
            else
            {
                m_cachedNumber = -1;
            }
        }

        return m_cachedNumber;
    }

    LIBEMBER_INLINE
    GlowParameter::iterator GlowParameter::insertImpl(iterator const& where, Node* child)
    {
        m_cachedNumber = -1;
        return GlowContainer::insertImpl(where, child);
    }

    LIBEMBER_INLINE
    void GlowParameter::eraseImpl(iterator const& first, iterator const& last)
    {
        m_cachedNumber = -1;
        GlowContainer::eraseImpl(first, last);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWPARAMETER_IPP
