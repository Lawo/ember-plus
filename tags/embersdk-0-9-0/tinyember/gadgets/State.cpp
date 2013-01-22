#include "EntityTypeVisitor.h"
#include "State.h"
#include "states/EnumState.h"
#include "states/NumericState.h"
#include "states/StringState.h"

namespace gadgets {
    //static
    State *State::create(StateType const type, Entity const& owner, bool writeable)
    {
        switch(type.value)
        {
            case StateType::Enumeration:
                return new states::EnumState(owner, writeable);

            case StateType::Integer:
                return new states::NumericState<int>(owner, writeable);

            case StateType::Real:
                return new states::NumericState<double>(owner, writeable);

            case StateType::String:
                return new states::StringState(owner, writeable);

            default:
                return 0;
        }
    }

    State::State(const Entity& owner, bool writeable, bool command)
        : m_owner(owner)
        , m_isCommand(command)
        , m_isWriteable(writeable)
    {
    }

    //virtual
    State::~State()
    {
    }

    //virtual
    void State::visit(EntityTypeVisitor &visitor, Parameter &parameter)
    {
        visitImpl(visitor, parameter);
    }

    //virtual
    void State::visit(EntityStateVisitor &visitor, Parameter const& parameter, PropertyState const& state) const
    {
        if (state.isSet(Property::IsWriteable))
            visitor.addProperty(Property::IsWriteable, m_isWriteable);

        if (state.isSet(Property::IsCommand))
            visitor.addProperty(Property::IsCommand, m_isCommand);

        visitImpl(visitor, parameter, state);
    }
}

