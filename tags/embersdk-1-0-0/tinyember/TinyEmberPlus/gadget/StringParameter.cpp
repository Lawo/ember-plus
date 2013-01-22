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

    void StringParameter::setValue(const_reference value)
    {
        if (m_value != value)
        {
            m_value = value;
            markDirty(ParameterField::Value, true);
        }
    }
}
