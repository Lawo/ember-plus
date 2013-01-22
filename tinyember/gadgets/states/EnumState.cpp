#include <algorithm>
#include <iostream>
#include "../EntityStateVisitor.h"
#include "../EntityTypeVisitor.h"
#include "../../Libember.h"
#include "EnumState.h"

using namespace libember::glow;

namespace gadgets { namespace states
{
    EnumState::EnumState(Entity const& owner, bool writeable)
        : State(owner, writeable, false)
        , m_index(0)
    {
    }

    //virtual
    EnumState::~EnumState()
    {
    }

    //virtual
    State* EnumState::clone(Entity const& owner) const
    {
        EnumState* clone = new EnumState(owner, isWriteable());
        clone->setCommand(isCommand());
        clone->setIndex(index());
        clone->assign(begin(), end());

        return clone;
    }

    //virtual
    bool EnumState::trySetValue(libember::glow::Value const& value)
    {
        if (value.type() == libember::ber::Type::Integer)
        {
            index_type const index = value.toInteger();
            setIndex(index);
            return true;
        }
        else
        {
            return true;
        }
    }

    void EnumState::setIndex(index_type index)
    {
        index_type const max = m_entries.empty() ? 0 : m_entries.size() - 1;
        index_type const min = 0;
        index = std::min(std::max(index, min), max);

        std::cout << "EnumState index changed from " << m_index << " to " << index << std::endl;

        if (m_index != index)
        {
            m_index = index;

            markDirty(Property::Value);
        }
    }

    EnumState::index_type EnumState::index() const
    {
        return m_index;
    }

    //virtual
    void EnumState::visitImpl(EntityTypeVisitor& visitor, Parameter& parameter)
    {
        visitor.visit(parameter, *this);
    }

    //virtual
    void EnumState::visitImpl(EntityStateVisitor& visitor, Parameter const&, PropertyState const& state) const
    {
        if (state.isSet(Property::ValueEnumeration))
            visitor.addProperty(Property::ValueEnumeration, m_entries);

        if (state.isSet(Property::Value))
            visitor.addProperty(Property::Value, static_cast<int>(m_index));
    }
}
}


