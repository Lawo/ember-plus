/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_IMPL_NODE_IPP
#define __LIBEMBER_DOM_IMPL_NODE_IPP

#include "../../util/Inline.hpp"
#include "../Container.hpp"

namespace libember { namespace dom 
{
    LIBEMBER_INLINE
    Node::Node(ber::Tag tag)
        : m_applicationTag(tag), m_parent(0), m_dirty(true)
    {}

    LIBEMBER_INLINE
    Node::Node(Node const& other)
        : m_applicationTag(other.m_applicationTag), m_parent(0), m_dirty(true)
    {}

    LIBEMBER_INLINE
    Node::~Node()
    {}

    LIBEMBER_INLINE
    ber::Tag Node::applicationTag() const
    {
        return m_applicationTag;
    }

    LIBEMBER_INLINE
    ber::Tag Node::typeTag() const
    {
        return typeTagImpl();
    }

    LIBEMBER_INLINE
    Node* Node::parent()
    {
        return m_parent;
    }
    
    LIBEMBER_INLINE
    Node const* Node::parent() const
    {
        return m_parent;
    }

    LIBEMBER_INLINE
    void Node::setParent(Node* parent)
    {
        m_parent = parent;
    }

    LIBEMBER_INLINE
    void Node::update() const
    {
        if (m_dirty)
        {
            m_dirty = false;
            updateImpl();
        }
    }

    LIBEMBER_INLINE
    void Node::encode(util::OctetStream& output) const
    {
        update();
        encodeImpl(output);
    }


    LIBEMBER_INLINE
    std::size_t Node::encodedLength() const
    {
        update();
        return encodedLengthImpl();
    }

    LIBEMBER_INLINE
    void Node::markDirty() const
    {
        m_dirty = true;
        if (m_parent != 0)
        {
            m_parent->markDirty();
        }
    }

    LIBEMBER_INLINE
    bool Node::isDirty() const
    {
        return m_dirty;
    }
}
}

#endif  // __LIBEMBER_DOM_IMPL_NODE_IPP

