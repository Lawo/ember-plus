#include "EntityViewFactory.h"
#include "NodeView.h"
#include "EnumParameterView.h"
#include "IntParameterView.h"
#include "RealParameterView.h"
#include "StringParameterView.h"

//static
QFrame *EntityViewFactory::createView(QWidget* parent, gadgets::Entity* entity)
{
    return EntityViewFactory(parent, entity).view();
}

EntityViewFactory::EntityViewFactory(QWidget* parent, gadgets::Entity* entity)
    : m_view(0)
    , m_parent(parent)
{
    if (entity != 0)
        entity->visit(*this);
}

QFrame* EntityViewFactory::view()
{
    return m_view;
}

//virtual
void EntityViewFactory::visit(Node &node)
{
    m_view = new NodeView(m_parent, &node);
}

//virtual
void EntityViewFactory::visit(Parameter &parameter, IntState& state)
{
    m_view = new IntParameterView(m_parent, &parameter, &state);
}

//virtual
void EntityViewFactory::visit(Parameter &parameter, DoubleState& state)
{
    m_view = new RealParameterView(m_parent, &parameter, &state);
}

//virtual
void EntityViewFactory::visit(Parameter &parameter, EnumState& state)
{
    m_view = new EnumParameterView(m_parent, &parameter, &state);
}

//virtual
void EntityViewFactory::visit(Parameter &parameter, StringState& state)
{
    m_view = new StringParameterView(m_parent, &parameter, &state);
}
