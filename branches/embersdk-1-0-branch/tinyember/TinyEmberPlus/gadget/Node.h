#ifndef __TINYEMBER_GADGET_NODE_H
#define __TINYEMBER_GADGET_NODE_H

#include <list>
#include "../Types.h"
#include "Collection.h"
#include "DirtyStateListener.h"
#include "NodeField.h"

namespace gadget
{
    class NodeFactory;
    class Parameter;
    class ParameterFactory;

    class Node
    {
        friend class NodeFactory;
        friend class ParameterFactory;
        public:
            typedef Collection<Node*> NodeCollection;
            typedef Collection<Parameter*> ParameterCollection;
            typedef DirtyStateListener<NodeFieldState::flag_type, Node const*> DirtyStateListener;

            ~Node();

            int number() const;

            Node const* parent() const;

            String const& identifier() const;

            String const& description() const;

            NodeCollection const& nodes() const;

            NodeCollection& nodes();

            ParameterCollection const& parameters() const;

            ParameterCollection& parameters();

            bool isDirty() const;

            void setDescription(String const& value);

            void clearDirtyState(bool recursive) const;

            void registerListener(DirtyStateListener* listener);

            void unregisterListener(DirtyStateListener* listener);

            void markDirty();

            bool remove(Node const* node);

            bool remove(Parameter const* parameter);

            NodeFieldState const& dirtyState() const;

        private:
            Node(Node* parent, String const& identifier, int number);

            void notify() const;

            int const m_number;
            String m_description;
            String const m_identifier;
            Node* m_parent;
            NodeCollection m_children;
            ParameterCollection m_parameters;
            std::list<DirtyStateListener*> m_listeners;
            mutable NodeFieldState m_state;
    };
}

#endif//__TINYEMBER_GADGET_NODE_H
