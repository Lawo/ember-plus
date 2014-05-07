#include <algorithm>
#include "Node.h"
#include "Parameter.h"

namespace gadget
{
    Node::Node(Node* parent, String const& identifier, int number)
        : m_identifier(identifier)
        , m_number(number)
        , m_parent(parent)
        , m_state(NodeField::All)
        , m_isOnline(true)
        , m_isMounted(true)
    {
    }

    Node::~Node()
    {
        auto parent = m_parent;
        auto children = m_children;
        auto parameters = m_parameters;
        if (parent != nullptr)
            parent->remove(this);

        for each(auto item in parameters)
        {
            delete item;
        }

        for each(auto item in children)
        {
            delete item;
        }
    }

    int Node::number() const
    {
        return m_number;
    }

    Node const* Node::parent() const
    {
        return m_parent;
    }

    String const& Node::identifier() const
    {
        return m_identifier;
    }

    String const& Node::description() const
    {
        return m_description;
    }

    String const& Node::schema() const
    {
        return m_schema;
    }

    Node::NodeCollection const& Node::nodes() const
    {
        return m_children;
    }

    Node::ParameterCollection const& Node::parameters() const
    {
        return m_parameters;
    }
        
    bool Node::isDirty() const
    {
        return m_state.isDirty();
    }

    bool Node::isOnline() const
    {
        return m_isOnline;
    }

    bool Node::isMounted() const
    {
        return m_isMounted;
    }

    void Node::mount()
    {
        if (m_isMounted == false)
        {
            m_isMounted = true;
            m_state.set(NodeField::IsOnline);

            notify();
            markDirty();
        }
    }

    void Node::unmount()
    {
        if (m_isMounted)
        {
            m_isOnline = false;
            m_state.set(NodeField::IsOnline);

            markDirty();

            m_isMounted = false;
            notify();
        }
    }


    NodeFieldState const& Node::dirtyState() const
    {
        return m_state;
    }

    void Node::setDescription(String const& value)
    {
        if (m_description != value)
        {
            m_description = value;
            m_state.set(NodeField::Description);

            notify();
            markDirty();
        }
    }
    
    void Node::setSchema(String const& value)
    {
        if (m_schema != value)
        {
            m_schema = value;
            m_state.set(NodeField::Schema);

            notify();
            markDirty();
        }
    }

    void Node::setIsOnline(bool isOnline)
    {
        if (m_isOnline != isOnline)
        {
            m_isOnline = isOnline;
            m_state.set(NodeField::IsOnline);

            if (m_isOnline)
                m_isMounted = true;

            notify();
            markDirty();
        }
    }

    void Node::clearDirtyState(bool recursive) const
    {
        m_state.clear();

        if (recursive)
        {
            for each(auto node in m_children)
            {
                node->clearDirtyState(recursive);
            }

            for each(auto parameter in m_parameters)
            {
                parameter->clearDirtyState();
            }
        }
    }

    void Node::registerListener(DirtyStateListener* listener)
    {
        auto const first = std::begin(m_listeners);
        auto const last = std::end(m_listeners);
        auto const result = std::find(first, last, listener);

        if (result == last)
        {
            m_listeners.push_back(listener);
        }
    }

    void Node::unregisterListener(DirtyStateListener* listener)
    {
        m_listeners.remove(listener);
    }

    void Node::notify() const
    {
        for each(auto listener in m_listeners)
        {
            listener->notifyStateChanged(m_state, this);
        }
    }

    void Node::markDirty()
    {
        m_state.set(NodeField::DirtyChildEntity);

        if (m_parent != nullptr)
        {
            m_parent->markDirty();
        }
        else
        {
            notify();
        }
    }

    bool Node::remove(Node *const node)
    {
        auto const first = std::begin(m_children);
        auto const last = std::end(m_children);
        auto const where = std::find(first, last, node);
        auto const result = where != last;
        if (node)
            node->m_parent = nullptr;

        m_children.remove(where);
        return result;
    }

    bool Node::remove(Parameter *const parameter)
    {
        auto const first = std::begin(m_parameters);
        auto const last = std::end(m_parameters);
        auto const where = std::find(first, last, parameter);
        auto const result = where != last;
        if (parameter)
            parameter->m_parent = nullptr;

        m_parameters.remove(where);
        return result;
    }
}
