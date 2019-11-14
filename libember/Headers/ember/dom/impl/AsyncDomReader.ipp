/*
    libember -- C++ 03 implementation of the Ember+ Protocol

    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#ifndef __LIBEMBER_DOM_IMPL_ASYNCDOMREADER_IPP
#define __LIBEMBER_DOM_IMPL_ASYNCDOMREADER_IPP

#include <stdexcept>
#include "../../util/Inline.hpp"
#include "../Container.hpp"

namespace libember { namespace dom 
{
    LIBEMBER_INLINE
    AsyncDomReader::AsyncDomReader(dom::NodeFactory const& factory)
        : m_isRootReady(false)
        , m_root(0)
        , m_current(0)
        , m_factory(factory)
    {
    }

    LIBEMBER_INLINE
    AsyncDomReader::~AsyncDomReader()
    {
        resetImpl();
    }

    LIBEMBER_INLINE
    bool AsyncDomReader::isRootReady() const
    {
        return m_isRootReady;
    }

    LIBEMBER_INLINE
    dom::Node* AsyncDomReader::detachRoot()
    {
        if (m_isRootReady)
        {
            dom::Node* root = m_root;

            m_root = 0;
            m_current = 0;
            m_isRootReady = false;
            return root;
        }
        else
        {
            return 0;
        }
    }

    LIBEMBER_INLINE
    void AsyncDomReader::containerReady(dom::Node*) 
    {
    }

    LIBEMBER_INLINE
    void AsyncDomReader::itemReady(dom::Node*)
    {
    }

    LIBEMBER_INLINE
    void AsyncDomReader::rootReady(dom::Node*)
    {
    }

    LIBEMBER_INLINE
    void AsyncDomReader::resetImpl()
    {
        if (m_current != 0 && m_current->parent() == 0 && m_current != m_root)
            delete m_current;

        if (m_root)
            delete m_root;

        m_root = 0;
        m_current = 0;
        m_isRootReady = false;
    }

    LIBEMBER_INLINE
    void AsyncDomReader::containerReady()
    {
        dom::Node* container = decodeNode(m_factory);
        if (m_isRootReady)
        {
            resetImpl();
        }

        if (container != 0)
        {
            if (m_root == 0)
            {
                m_root = container;
            }
            else
            {
                dom::Container* current = dynamic_cast<dom::Container*>(m_current);
                if (current)
                    current->insert(current->end(), container);
            }

            m_current = container;
            containerReady(container);
        }

    }

    LIBEMBER_INLINE
    void AsyncDomReader::itemReady()
    {
        if(isContainer())
        {
            dom::Node* node = m_current;
            if (m_current == m_root)
            {
                m_isRootReady = true;
                rootReady(m_current);

                m_current = 0;
            }
            else
            {
                m_current = m_current->parent();
            }

            itemReady(node);
        }
        else
        {
            dom::Node* node = decodeNode(m_factory);
            if (node != 0)
            {
                dom::Container* container = dynamic_cast<dom::Container*>(m_current);
                if (container)
                    container->insert(container->end(), node);

                itemReady(node);
            }
        }
    }
}
}

#endif  // __LIBEMBER_DOM_IMPL_ASYNCDOMREADER_IPP

