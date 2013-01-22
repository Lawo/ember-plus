#include "EnumParameter.h"
#include "ParameterTypeVisitor.h"

namespace gadget
{
    EnumParameter::EnumParameter(Node* parent, String const& identifier, int number)
        : Parameter(ParameterType::Enum, parent, identifier, number)
        , m_index(0)
    {
    }
    
    void EnumParameter::accept(ParameterTypeVisitorConst const& visitor) const
    {
        visitor.visit(this);
    }
    
    void EnumParameter::accept(ParameterTypeVisitor& visitor) 
    {
        visitor.visit(this);
    }

    std::string EnumParameter::toDisplayValue() const
    {
        auto const index = this->index();
        auto const size = this->size();
        if (index < size)
        {
            return m_enumeration[index];
        }
        else
        {
            return "";
        }
    }

    void EnumParameter::setIndex(size_type value)
    {
        if (m_index != value && value < size())
        {
            m_index = value;
            markDirty(ParameterField::Value, true);
        }
    }

    EnumParameter::size_type EnumParameter::size() const
    {
        return m_enumeration.size();
    }

    bool EnumParameter::empty() const
    {
        return m_enumeration.empty();
    }

    EnumParameter::const_iterator EnumParameter::begin() const
    {
        return m_enumeration.begin();
    }

    EnumParameter::const_iterator EnumParameter::end() const
    {
        return m_enumeration.end();
    }

    EnumParameter::size_type EnumParameter::index() const
    {
        return m_index;
    }
}
