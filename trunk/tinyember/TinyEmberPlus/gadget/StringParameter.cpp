#include "ParameterTypeVisitor.h"
#include "StringParameter.h"

namespace gadget
{
    StringParameter::StringParameter(Node* parent, String const& identifier, int number, const_reference value, size_type maxLength)
        : Parameter(ParameterType::String, parent, identifier, number)
        , m_value(value)
        , m_maxLength(maxLength)
    {
    }
    
    void StringParameter::setValueImpl(value_type value, bool forceNotification)
    {
    }

    void StringParameter::accept(ParameterTypeVisitorConst const& visitor) const
    {
        visitor.visit(this);
    }
    
    void StringParameter::accept(ParameterTypeVisitor& visitor)
    {
        visitor.visit(this);
    }

    std::string StringParameter::toDisplayValue() const
    {
        return m_value;
    }

    StringParameter::const_reference StringParameter::value() const
    {
        return m_value;
    }

    StringParameter::size_type StringParameter::maxLength() const
    {
        return m_maxLength;
    }

    bool StringParameter::isIndefiniteLength() const
    {
        return m_maxLength == 0;
    }

    void StringParameter::setMaxLength(size_type value)
    {
        if (m_maxLength != value)
        {
            m_maxLength = value;
            markDirty(ParameterField::ValueMax, true);
        }
    }

    void StringParameter::setValue(const_reference value, bool forceNotification)
    {
        if (m_value != value || forceNotification)
        {
            m_value = value;
            setValueImpl(value, forceNotification);
            markDirty(ParameterField::Value | (forceNotification ? ParameterField::ForceUpdate : 0), true);
        }
    }
}
