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
            updateImpl();
            m_dirty = false;
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

