#include <algorithm>
#include "Node.h"
#include "Parameter.h"
#include "StreamManager.h"

namespace gadget
{
    Parameter::Parameter(ParameterType const& type, Node* parent, String const& identifier, int number)
        : m_identifier(identifier)
        , m_number(number)
        , m_parent(parent)
        , m_type(type)
        , m_streamIdentifier(-1)
        , m_access(gadget::Access::ReadWrite)
    {
    }

    Parameter::~Parameter()
    {
        StreamManager::instance().unregisterParameter(this);

        auto parent = m_parent;
        if (parent != nullptr)
            parent->remove(this);
    }

    int Parameter::number() const
    {
        return m_number;
    }

    int Parameter::streamIdentifier() const
    {
        return m_streamIdentifier;
    }

    ParameterType const Parameter::type() const
    {
        return m_type;
    }

    String const& Parameter::identifier() const
    {
        return m_identifier;
    }

    String const& Parameter::description() const
    {
        return m_description;
    }

    Formula const& Parameter::formula() const
    {
        return m_formula;
    }

    Access Parameter::access() const
    {
        return static_cast<Access::_Domain>(m_access);
    }

    StreamDescriptor const* Parameter::streamDescriptor() const
    {
        return m_streamDescriptor.get();
    }

    Node const* Parameter::parent() const
    {
        return m_parent;
    }

    bool Parameter::isDirty() const
    {
        return m_state.isDirty();
    }

    ParameterFieldState const& Parameter::dirtyState() const
    {
        return m_state;
    }

    void Parameter::clearDirtyState()
    {
        m_state.clear();
    }

    void Parameter::setDescription(String const& value)
    {
        if (m_description != value)
        {
            m_description = value;
            markDirty(ParameterField::Description, true);
        }
    }

    void Parameter::setAccess(Access const& value)
    {
        if (m_access != value.value())
        {
            m_access = value.value();
            markDirty(ParameterField::Access, true);
        }
    }

    void Parameter::setFormula(Formula const& formula)
    {
        if (m_formula != formula)
        {
            m_formula = formula;
            markDirty(ParameterField::ValueFormula, true);
        }
    }

    void Parameter::setStreamIdentifier(int value)
    {
        if (m_streamIdentifier != value)
        {
            m_streamIdentifier = value;
            markDirty(ParameterField::StreamIdentifier, true);

            if (hasStreamIdentifier())
                subscribed();
            else
                unsubscribed();
        }
    }

    void Parameter::setStreamDescriptor(std::shared_ptr<StreamDescriptor> descriptor)
    {
        m_streamDescriptor = descriptor;
        markDirty(ParameterField::StreamDescriptor, true);

        if (descriptor)
            subscribed();
        else
            unsubscribed();
    }

    void Parameter::setStreamDescriptor(StreamFormat const& format, unsigned offset)
    {
        if (hasStreamDescriptor())
        {
            if (format.value() == m_streamDescriptor->format().value()
            &&  offset == m_streamDescriptor->offset())
            {
                return;
            }
        }

        auto descriptor = std::make_shared<StreamDescriptor>(format, offset);
        setStreamDescriptor(descriptor);
    }

    void Parameter::registerListener(DirtyStateListener* listener)
    {
        auto const first = std::begin(m_listeners);
        auto const last = std::end(m_listeners);
        auto const result = std::find(first, last, listener);

        if (result == last)
        {
            m_listeners.push_back(listener);
        }
    }

    void Parameter::unregisterListener(DirtyStateListener* listener)
    {
        m_listeners.remove(listener);
    }

    void Parameter::markDirty(ParameterField const& field, bool notify)
    {
        m_state.set(field.value);
        if (notify)
            this->notify();

        if (m_parent != nullptr)
            m_parent->markDirty();
    }

    void Parameter::notify() const
    {
        for each(auto listener in m_listeners)
        {
            listener->notifyStateChanged(m_state, this);
        }
    }

    bool Parameter::hasStreamIdentifier() const
    {
        return m_streamIdentifier > -1;
    }

    bool Parameter::hasStreamDescriptor() const
    {
        return m_streamDescriptor != nullptr;
    }

    void Parameter::subscribed()
    {
        if (hasStreamIdentifier() || hasStreamDescriptor() || isSubscribed())
            StreamManager::instance().registerParameter(this);
    }

    void Parameter::unsubscribed()
    {
        if (hasStreamIdentifier() == false && hasStreamDescriptor() == false && isSubscribed() == false)
            StreamManager::instance().unregisterParameter(this);
    }

    void Parameter::subscriptionCountChanged(size_type newCount)
    {
        markDirty(ParameterField::SubscriptionCount, true);
    }
}
