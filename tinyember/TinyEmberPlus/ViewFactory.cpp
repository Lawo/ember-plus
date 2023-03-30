/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

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
    : m_parent(parent)
    , m_view(nullptr)
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
