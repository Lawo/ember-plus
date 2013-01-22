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

