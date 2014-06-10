/*
    libember -- C++ 03 implementation of the Ember+ Protocol
    Copyright (C) 2012-2014  L-S-B Broadcast Technologies GmbH

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

