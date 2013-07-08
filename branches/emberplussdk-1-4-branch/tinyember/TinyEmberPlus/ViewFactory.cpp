#include "ViewFactory.h"
#include "NodeView.h"
#include "BooleanView.h"
#include "EnumerationView.h"
#include "IntegerView.h"
#include "RealView.h"
#include "StringView.h"

QFrame* ViewFactory::createView(QWidget* parent, gadget::Node* node)
{
    return new NodeView(parent, node);
}

QFrame* ViewFactory::createView(QWidget* parent, gadget::Parameter* parameter)
{
    auto const impl = ViewFactoryImpl(parent, parameter);
    return impl.view();
}


ViewFactory::ViewFactoryImpl::ViewFactoryImpl(QWidget* parent, gadget::Parameter* parameter)
    : m_view(nullptr)
    , m_parent(parent)
{
    parameter->accept(*this);
}

QFrame* ViewFactory::ViewFactoryImpl::view() const
{
    return m_view;
}

void ViewFactory::ViewFactoryImpl::visit(gadget::BooleanParameter* parameter)
{
    m_view = new BooleanView(m_parent, parameter);
}

void ViewFactory::ViewFactoryImpl::visit(gadget::EnumParameter* parameter)
{
    m_view = new EnumerationView(m_parent, parameter);
}

void ViewFactory::ViewFactoryImpl::visit(gadget::StringParameter* parameter)
{
    m_view = new StringView(m_parent, parameter);
}

void ViewFactory::ViewFactoryImpl::visit(gadget::IntegerParameter* parameter)
{
    m_view = new IntegerView(m_parent, parameter);
}

void ViewFactory::ViewFactoryImpl::visit(gadget::RealParameter* parameter)
{
    m_view = new RealView(m_parent, parameter);
}
