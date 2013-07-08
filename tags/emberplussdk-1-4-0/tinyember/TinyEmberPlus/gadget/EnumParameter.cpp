#include "EnumParameter.h"
#include "ParameterTypeVisitor.h"

namespace gadget
{
    EnumParameter::EnumParameter(Node* parent, String const& identifier, int number)
        : Parameter(ParameterType::Enum, parent, identifier, number)
        , m_index(0)
    {
    }

    void EnumParameter::setIndexImpl(size_type value, bool forceNotification)
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
            return std::string("");
        }
    }

    void EnumParameter::setIndex(size_type value, bool forceNotification)
    {
        if ((m_index != value || forceNotification) && value < size())
        {
            m_index = value;
            setIndexImpl(value, forceNotification);
            markDirty(ParameterField::Value | (forceNotification ? ParameterField::ForceUpdate : 0), true);
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
