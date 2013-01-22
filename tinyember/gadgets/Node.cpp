#include "EntityStateVisitor.h"
#include "EntityTypeVisitor.h"
#include "Node.h"
#include "Parameter.h"


namespace gadgets {
    //static
    Node* Node::create(String const& identifier, String const& description)
    {
        return new Node(0, identifier, description);
    }

    Node::Node(Node* parent, String const& identifier, String const& description)
        : Entity(parent, identifier, description)
    {
    }

    //virtual
    Node::~Node()
    {
    }

    //virtual
    Entity* Node::clone(Node* parent, const String& identifier) const
    {
        Node* clone = new Node(parent, identifier, description());

        for(const_iterator it = begin(); it != end(); ++it)
            (*it)->clone(clone, (*it)->identifier());

        return clone;
    }

    Node* Node::insert(String const& identifier, String const& description)
    {
        return new Node(this, identifier, description);
    }

    Parameter* Node::insert(StateType const type, String const& identifier, String const& description)
    {
        return new Parameter(this, type, identifier, description);
    }

    //virtual
    void Node::visitImpl(EntityStateVisitor& visitor, PropertyState const&) const
    {
        visitor.visit(*this);
    }

    //virtual
    void Node::visitImpl(EntityTypeVisitor& typeVisitor)
    {
        typeVisitor.visit(*this);
    }
}

