/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <algorithm>
#include "Node.h"
#include "NodeFactory.h"
#include "util/NumberFactory.h"

namespace gadget
{
    //static 
    Node* NodeFactory::createRoot(String const& identifier)
    {
        return new Node(nullptr, identifier, 1);
    }

    //static 
    Node* NodeFactory::createNode(Node* parent, String const& identifier)
    {
        Node* node = nullptr;
        if (parent != nullptr)
        {
            auto& collection = parent->m_children;
            auto const number = util::NumberFactory::create(parent);
            auto const where = std::end(collection);
            node = new Node(parent, identifier, number);
            collection.insert(where, node);
        }

        return node;
    }
}
