#include "EntityStateVisitor.h"
#include "EntityTypeVisitor.h"
#include "Node.h"
#include "Parameter.h"
#include "State.h"

namespace gadgets
{
    Parameter::Parameter(Node* parent, StateType const type, String const& identifier, String const& description)
        : Entity(parent, identifier, description)
        , m_type(0)
    {
        setState(type);
    }

    //virtual
    Parameter::~Parameter()
    {
        setState(StateType::None);
    }

    //virtual
    Entity* Parameter::clone(Node* parent, String const& identifier) const
    {
        Parameter* clone = new Parameter(parent, StateType::None, identifier, description());

        clone->m_type = m_type ? m_type->clone(*clone) : 0;
        return clone;
    }

    void Parameter::setState(StateType const type)
    {
        delete m_type;

        m_type = State::create(type, *this, true);
    }

    bool Parameter::trySetValue(libember::glow::Value const& value)
    {
        return m_type ? m_type->trySetValue(value) : false;
    }

    //virtual
    void Parameter::visitImpl(EntityStateVisitor &visitor, PropertyState const& properties) const
    {
        visitor.visit(*this);

        if (m_type)
            m_type->visit(visitor, *this, properties);
    }

    //virtual
    void Parameter::visitImpl(EntityTypeVisitor &typeVisitor)
    {
        if (m_type)
            m_type->visit(typeVisitor, *this);
    }
}
