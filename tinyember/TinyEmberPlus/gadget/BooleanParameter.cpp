/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "BooleanParameter.h"
#include "ParameterTypeVisitor.h"

namespace gadget
{
    BooleanParameter::BooleanParameter(Node* parent, String const& identifier, int number, value_type value)
        : Parameter(ParameterType::Boolean, parent, identifier, number)
        , m_value(value)
    {}

    void BooleanParameter::setValueImpl(value_type /* value */, bool /* forceNotification */)
    {}

    BooleanParameter::value_type BooleanParameter::value() const
    {
        return m_value;
    }

    void BooleanParameter::setValue(value_type value, bool forceNotification)
    {
        if (m_value != value || forceNotification)
        {
            m_value = value;
            setValueImpl(value, forceNotification);
            markDirty(ParameterField::Value | (forceNotification ? ParameterField::ForceUpdate : 0), true);
        }
    }

    void BooleanParameter::accept(ParameterTypeVisitorConst const& visitor) const
    {
        visitor.visit(this);
    }

    void BooleanParameter::accept(ParameterTypeVisitor& visitor)
    {
        visitor.visit(this);
    }

    std::string BooleanParameter::toDisplayValue() const
    {
        return m_value ? "true" : "false";
    }
}
