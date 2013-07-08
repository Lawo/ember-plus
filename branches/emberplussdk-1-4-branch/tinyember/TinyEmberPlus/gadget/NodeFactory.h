#ifndef __TINYEMBER_GADGET_NODEFACTORY_H
#define __TINYEMBER_GADGET_NODEFACTORY_H

#include "../Types.h"

namespace gadget
{
    /** Forward declaration */
    class Node;

    /**
     * A factory that creates a new instance of the default Node implementation.
     */
    class NodeFactory
    {
        public:
            /**
             * Creates a new root node with the passed identifier.
             * @param identifier The node identifier.
             * @return The newly created root node. Please note that this node must be deleted
             *      manually.
             */
            static Node* createRoot(String const& identifier);

            /**
             * Creates a new node and inserts it to the passed parent node.
             * @param parent The parent node to attach the newly created node to.
             * @param identifier The identifier of the new node.
             * @return The newly created node. As long as this node is owned by a parent
             *      node it must not necessarily be deleted manually.
             */
            static Node* createNode(Node* parent, String const& identifier);
    };
}

#endif//__TINYEMBER_GADGET_NODEFACTORY_H
