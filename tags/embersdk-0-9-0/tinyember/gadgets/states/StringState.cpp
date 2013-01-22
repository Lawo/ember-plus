#include "../EntityStateVisitor.h"
#include "../EntityTypeVisitor.h"
#include "../../Libember.h"
#include "StringState.h"

namespace gadgets { namespace states {
    StringState::StringState(Entity const& owner, bool writeable)
        : State(owner, writeable, false)
        , m_value("")
        , m_maxLength(0)
    {
    }

    //virtual
    StringState::~StringState()
    {
    }

    //virtual
    State* StringState::clone(Entity const& owner) const
    {
        StringState* clone = new StringState(owner, isWriteable());
        clone->setCommand(isCommand());
        clone->setMaxLength(maxLength());
        clone->setValue(value());

        return clone;
    }

    //virtual
    bool StringState::trySetValue(libember::glow::Value const& value)
    {
        String const text = value.toString();
        setValue(text);
        return true;
    }

    //virtual
    void StringState::visitImpl(EntityTypeVisitor& visitor, Parameter& parameter)
    {
        visitor.visit(parameter, *this);
    }

    //virtual
    void StringState::visitImpl(EntityStateVisitor& visitor, Parameter const&, PropertyState const& state) const
    {
        if (state.isSet(Property::ValueMax))
            visitor.addProperty(Property::ValueMax, m_maxLength);

        if (state.isSet(Property::Value))
            visitor.addProperty(Property::Value, m_value);
    }

    void StringState::setValue(String const& value)
    {
        if (m_value != value)
        {
            m_value = value;

            markDirty(Property::Value);
        }
    }

    void StringState::setMaxLength(int maxLength)
    {
        if (m_maxLength != maxLength)
        {
            m_maxLength = maxLength;

            markDirty(Property::ValueMax);
        }
    }

    String const& StringState::value() const
    {
        return m_value;
    }

    int StringState::maxLength() const
    {
        return m_maxLength;
    }
}
}
