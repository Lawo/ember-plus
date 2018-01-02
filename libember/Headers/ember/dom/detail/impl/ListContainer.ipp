/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_DETAIL_IMPL_LISTCONTAINER_IPP
#define __LIBEMBER_DOM_DETAIL_IMPL_LISTCONTAINER_IPP

#include "../../../util/DerefIterator.hpp"
#include "../../../util/Inline.hpp"
#include "../../../ber/Encoding.hpp"

namespace libember { namespace dom { namespace detail
{
    LIBEMBER_INLINE    
    ListContainer::ListContainer(ber::Tag tag)
        : Container(tag), m_children(), m_cachedLength(0)
    {}

    LIBEMBER_INLINE    
    ListContainer::ListContainer(ListContainer const& other)
        : Container(other), m_children(), m_cachedLength(0)
    {
        try
        {
            NodeList::const_iterator const begin = other.m_children.begin();
            NodeList::const_iterator const end   = other.m_children.end();
            for (NodeList::const_iterator i = begin; i != end; ++i)
            {
                Node* const child = (*i)->clone();
                m_children.push_back(child);
                fixParent(child);
            }
        }
        catch (...)
        {
            NodeList::const_iterator const begin = m_children.begin();
            NodeList::const_iterator const end   = m_children.end();
            for (NodeList::const_iterator i = begin; i != end; ++i)
            {
                delete (*i);
            }
            throw;
        }
    }

    LIBEMBER_INLINE    
    ListContainer::~ListContainer()
    {
        NodeList::const_iterator const begin = m_children.begin();
        NodeList::const_iterator const end   = m_children.end();
        for (NodeList::const_iterator i = begin; i != end; ++i)
        {
            delete (*i); 
        }
    }

    LIBEMBER_INLINE    
    bool ListContainer::empty() const
    {
        return m_children.empty();
    }

    LIBEMBER_INLINE    
    ListContainer::size_type ListContainer::size() const
    {
        return m_children.size();
    }

    LIBEMBER_INLINE    
    ListContainer::iterator ListContainer::begin()
    {
        util::DerefIterator<NodeList::iterator> const result(m_children.begin());
        return result;
    }

    LIBEMBER_INLINE    
    ListContainer::const_iterator ListContainer::begin() const
    {
        util::DerefIterator<NodeList::const_iterator> const result(m_children.begin());
        return result;
    }

    LIBEMBER_INLINE    
    ListContainer::iterator ListContainer::end()
    {
        util::DerefIterator<NodeList::iterator> const result(m_children.end());
        return result;
    }

    LIBEMBER_INLINE    
    ListContainer::const_iterator ListContainer::end() const
    {
        util::DerefIterator<NodeList::const_iterator> const result(m_children.end());
        return result;
    }

    LIBEMBER_INLINE    
    ListContainer::iterator ListContainer::insertImpl(iterator const& where, Node* child)
    {
        typedef util::DerefIterator<NodeList::iterator> DerefIteratorType;
        NodeList::iterator const i = where.as<DerefIteratorType>().wrappedIterator();
        return DerefIteratorType(m_children.insert(i, child));
    }

    LIBEMBER_INLINE    
    void ListContainer::eraseImpl(iterator const& first, iterator const& last)
    {
        typedef util::DerefIterator<NodeList::iterator> DerefIteratorType;
        NodeList::iterator const f = first.as<DerefIteratorType>().wrappedIterator();
        NodeList::iterator const l = last.as<DerefIteratorType>().wrappedIterator();
        for (NodeList::const_iterator i = f; i != l; ++i)
        {
            delete (*i);
        }
        m_children.erase(f, l);
    }

    LIBEMBER_INLINE    
    std::size_t ListContainer::encodedPayloadLength() const
    {
        std::size_t payloadLength = 0;
        for (NodeList::const_iterator i = m_children.begin(); i != m_children.end(); ++i)
        {
            payloadLength += (*i)->encodedLength(); 
        }
        return payloadLength;
    }

    LIBEMBER_INLINE    
    void ListContainer::encodePayload(util::OctetStream& output) const
    {
        for (NodeList::const_iterator i = m_children.begin(); i != m_children.end(); ++i)
        {
            (*i)->encode(output); 
        }
    }

    LIBEMBER_INLINE
    void ListContainer::updateImpl() const
    {
        std::size_t const innerTagLength = ber::encodedLength(typeTag().toContainer());
        std::size_t const payloadLength  = encodedPayloadLength();
        std::size_t const innerLength    = innerTagLength + ber::encodedLength(ber::make_length(payloadLength)) + payloadLength;
        
        std::size_t const outerTagLength = ber::encodedLength(applicationTag().toContainer());
        std::size_t const outerLength    = outerTagLength + ber::encodedLength(ber::make_length(innerLength)) + innerLength;

        m_cachedLength = outerLength;
    }

    LIBEMBER_INLINE
    void ListContainer::encodeImpl(util::OctetStream& output) const
    {
        ber::Tag const innerContainerTag = typeTag().toContainer();
        std::size_t const innerTagLength = ber::encodedLength(innerContainerTag);
        std::size_t const payloadLength  = encodedPayloadLength();
        std::size_t const innerLength    = innerTagLength + ber::encodedLength(ber::make_length(payloadLength)) + payloadLength;
        
        ber::encode(output, applicationTag().toContainer());
        ber::encode(output, ber::make_length(innerLength));
        
        ber::encode(output, innerContainerTag);
        ber::encode(output, ber::make_length(payloadLength));
        
        encodePayload(output);
    }

    LIBEMBER_INLINE
    std::size_t ListContainer::encodedLengthImpl() const
    {
        return m_cachedLength;
    }
}
}
}

#endif  // __LIBEMBER_DOM_DETAIL_IMPL_LISTCONTAINER_IPP

