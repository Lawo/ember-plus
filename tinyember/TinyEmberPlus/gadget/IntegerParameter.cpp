#include <formula/TermCompiler.hpp>
#include <sstream>
#include <cstdio>
#include "IntegerParameter.h"
#include "ParameterTypeVisitor.h"

namespace gadget
{
    IntegerParameter::IntegerParameter(Node* parent, String const& identifier, int number, value_type min, value_type max, value_type value)
        : Parameter(ParameterType::Integer, parent, identifier, number)
        , m_min(min)
        , m_max(max)
        , m_value(value)
    {
    }

    void IntegerParameter::setValueImpl(value_type value, bool forceNotification)
    {
    }
    
    void IntegerParameter::accept(ParameterTypeVisitorConst const& visitor) const
    {
        visitor.visit(this);
    }
    
    void IntegerParameter::accept(ParameterTypeVisitor& visitor)
    {
        visitor.visit(this);
    }

    std::string IntegerParameter::toDisplayValue() const
    {
        auto const& formula = this->formula();
        auto value = this->value();
        if (formula.valid())
        {
            auto term = libformula::TermCompiler::compile(formula.providerToConsumer());
            value = term.compute(value);
        }

        if (m_format.size() > 0)
        {
            char buffer[256];
            #ifdef WIN32
            sprintf_s(buffer, 256, m_format.c_str(), value);
            #else
            snprintf(buffer, 256, m_format.c_str(), value);
            #endif
            return std::string(buffer);
        }
        else
        {
            std::stringstream stream;
            stream << value;
            return stream.str();
        }
    }

    IntegerParameter::value_type IntegerParameter::minimum() const
    {
        return m_min;
    }

    IntegerParameter::value_type IntegerParameter::maximum() const
    {
        return m_max;
    }

    IntegerParameter::value_type IntegerParameter::value() const
    {
        return m_value;
    }

    String const& IntegerParameter::format() const
    {
        return m_format;
    }

    void IntegerParameter::setMin(value_type value)
    {
        if (m_min != value)
        {
            m_min = value;
            markDirty(ParameterField::ValueMin, true);
        }
    }

    void IntegerParameter::setMax(value_type value)
    {
        if (m_max != value)
        {
            m_max = value;
            markDirty(ParameterField::ValueMax, true);
        }
    }

    void IntegerParameter::setValue(value_type value, bool forceNotification)
    {
        if (m_value != value || forceNotification)
        {
            m_value = value;
            setValueImpl(value, forceNotification);
            markDirty(ParameterField::Value | (forceNotification ? ParameterField::ForceUpdate : 0), true);
        }
    }

    void IntegerParameter::setFormat(String const& value)
    {
        if (m_format != value)
        {
            m_format = value;
            markDirty(ParameterField::ValueFormat, true);
        }
    }
}
