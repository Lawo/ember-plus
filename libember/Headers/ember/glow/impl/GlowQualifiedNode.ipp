/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_GLOW_GLOWQUALIFIEDNODE_IPP
#define __LIBEMBER_GLOW_GLOWQUALIFIEDNODE_IPP

#include "../../util/Inline.hpp"
#include "../util/ValueConverter.hpp"
#include "../GlowRootElementCollection.hpp"

namespace libember { namespace glow 
{
    LIBEMBER_INLINE
    GlowQualifiedNode::GlowQualifiedNode(ber::ObjectIdentifier const& path)
        : GlowNodeBase(GlowType::QualifiedNode, GlowTags::ElementDefault(), GlowTags::QualifiedNode::Contents(), GlowTags::QualifiedNode::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedNode::Path(), path));
    }

    LIBEMBER_INLINE
    GlowQualifiedNode::GlowQualifiedNode(GlowRootElementCollection* parent, ber::ObjectIdentifier const& path)
        : GlowNodeBase(GlowType::QualifiedNode, GlowTags::ElementDefault(), GlowTags::QualifiedNode::Contents(), GlowTags::QualifiedNode::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedNode::Path(), path));
        if (parent)
        {
            GlowRootElementCollection::iterator const where = parent->end();
            parent->insert(where, this);
        }
    }

    LIBEMBER_INLINE
    GlowQualifiedNode::GlowQualifiedNode(ber::ObjectIdentifier const& path, ber::Tag const& tag)
        : GlowNodeBase(GlowType::QualifiedNode, tag, GlowTags::Node::Contents(), GlowTags::Node::Children())
    {
        insert(begin(), new dom::VariantLeaf(GlowTags::QualifiedNode::Path(), path));
    }

    LIBEMBER_INLINE
    GlowQualifiedNode::GlowQualifiedNode(ber::Tag const& tag)
        : GlowNodeBase(GlowType::QualifiedNode, tag, GlowTags::Node::Contents(), GlowTags::Node::Children())
    {}

    LIBEMBER_INLINE
    ber::ObjectIdentifier const& GlowQualifiedNode::path() const
    {
        if (m_cachedPath.empty())
        {
            ber::Tag const tag = GlowTags::QualifiedNode::Path();
            const_iterator const first = begin();
            const_iterator const last = end();
            const_iterator const result = util::find_tag(first, last, tag);
            if (result != last)
            {
                m_cachedPath = util::ValueConverter::valueOf(&*result, ber::ObjectIdentifier());
            }
            else
            {
                m_cachedPath = ber::ObjectIdentifier();
            }
        }

        return m_cachedPath;
    }

    LIBEMBER_INLINE
    GlowQualifiedNode::iterator GlowQualifiedNode::insertImpl(iterator const& where, Node* child)
    {
        m_cachedPath = ber::ObjectIdentifier();
        return GlowContainer::insertImpl(where, child);
    }

    LIBEMBER_INLINE
    void GlowQualifiedNode::eraseImpl(iterator const& first, iterator const& last)
    {
        m_cachedPath = ber::ObjectIdentifier();
        GlowContainer::eraseImpl(first, last);
    }
}
}

#endif  // __LIBEMBER_GLOW_GLOWQUALIFIEDNODE_IPP
