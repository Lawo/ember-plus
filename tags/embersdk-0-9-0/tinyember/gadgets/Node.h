#ifndef NODE_H
#define NODE_H

#include "Entity.h"
#include "State.h"

namespace gadgets
{
    /** Forward declarations **/
    class Parameter;

    /**
     * Node class
     */
    class Node : public Entity
    {
    public:
        /**
         * Root node factory. Use this method to create a new root node
         * @param identifier Node identifier
         * @param description Node description
         */
        static Node *create(String const& identifier, String const& description);

    public:
        /** Destructor */
        virtual ~Node();

        /**
         * Creates a copy of this node, including all children.
         * @param parent Parent node of the copy
         * @param identifier Identifier of the copy
         */
        virtual Entity* clone(Node* parent, const String& identifier) const;

        /**
         * Inserts a new child node
         * @param identifier Child node identifier
         * @param description Child node description
         */
        Node* insert(String const& identifier, String const& description);

        /**
         * Inserts a new parameter
         * @param type Parameter value type
         * @param identifier Identifier of the parameter
         * @param description Parameter description
         */
        Parameter* insert(StateType const type, String const& identifier, String const& description);

    private:
        /**
         * Constructor
         * @param parent Parent node, may be null
         * @param identifier Node identifier
         * @param description Node description
         */
        Node(Node* parent, String const& identifier, String const& description);

        /**
         * Visit implementation
         * @see Entity
         */
        virtual void visitImpl(EntityStateVisitor& visitor, PropertyState const& properties) const;

        /**
         * Visit implementation
         * @see Entity
         */
        virtual void visitImpl(EntityTypeVisitor& typeVisitor);
    };
}

#endif // NODE_H
