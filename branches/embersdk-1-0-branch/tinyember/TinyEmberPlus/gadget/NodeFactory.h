#ifndef __TINYEMBER_GADGET_NODEFACTORY_H
#define __TINYEMBER_GADGET_NODEFACTORY_H

#include "../Types.h"

namespace gadget
{
    class Node;

    class NodeFactory
    {
        public:
            static Node* createRoot(String const& identifier);

            static Node* createNode(Node* parent, String const& identifier);
    };
}

#endif//__TINYEMBER_GADGET_NODEFACTORY_H
