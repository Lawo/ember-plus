#include <formula/TermCompiler.hpp>
#include <sstream>
#include <cstdio>
#include "ParameterTypeVisitor.h"
#include "RealParameter.h"

namespace gadget
{
    RealParameter::RealParameter(Node* parent, String const& identifier, int number, value_type min, value_type max, value_type value)
        : Parameter(ParameterType::Real, parent, identifier, number)
        , m_min(min)
        , m_max(max)
        , m_value(value)
    {
    }

    void RealParameter::setValueImpl(value_type value, bool forceNotification)
    {
    }
    
    void RealParameter::accept(ParameterTypeVisitorConst const& visitor) const
    {
        visitor.visit(this);
    }
    
    void RealParameter::accept(ParameterTypeVisitor& visitor)
    {
        visitor.visit(this);
    }

    std::string RealParameter::toDisplayValue() const
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

    RealParameter::value_type RealParameter::minimum() const
    {
        return m_min;
    }

    RealParameter::value_type RealParameter::maximum() const
    {
        return m_max;
    }

    RealParameter::value_type RealParameter::value() const
    {
        return m_value;
    }

    String const& RealParameter::format() const
    {
        return m_format;
    }

    void RealParameter::setMin(value_type value)
    {
        if (m_min != value)
        {
            m_min = value;
            markDirty(ParameterField::ValueMin, true);
        }
    }

    void RealParameter::setMax(value_type value)
    {
        if (m_max != value)
        {
            m_max = value;
            markDirty(ParameterField::ValueMax, true);
        }
    }

    void RealParameter::setValue(value_type value, bool forceNotification)
    {
        if (m_value != value || forceNotification)
        {
            m_value = value;
            setValueImpl(value, forceNotification);
            markDirty(ParameterField::Value | (forceNotification ? ParameterField::ForceUpdate : 0), true);
        }
    }

    void RealParameter::setFormat(String const& value)
    {
        if (m_format != value)
        {
            m_format = value;
            markDirty(ParameterField::ValueFormat, true);
        }
    }
}
