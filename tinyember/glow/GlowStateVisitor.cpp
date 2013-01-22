#include <iostream>
#include "GlowStateVisitor.h"
#include "GlowNodeVisitor.h"
#include "GlowParameterVisitor.h"
#include "gadgets/Node.h"
#include "gadgets/Parameter.h"

namespace glow
{
    GlowStateVisitor::GlowStateVisitor(libember::glow::GlowElementCollection* parent)
        : m_proxy(0)
        , m_parent(parent)
    {
    }

    //virtual
    GlowStateVisitor::~GlowStateVisitor()
    {
        VisitorList::iterator first = m_children.begin();
        VisitorList::iterator const last = m_children.end();

        for(; first != last; ++first)
        {
            delete (*first);
        }

        m_children.clear();
    }

    GlowStateVisitor* GlowStateVisitor::createChildVisitor()
    {
        m_children.push_back(m_proxy->createChildVisitor());
        return m_children.back();
    }

    //virtual
    void GlowStateVisitor::visit(gadgets::Node const& node)
    {
        std::cout << "Node " << node.identifier() << " Parent = " << m_parent << std::endl;

        m_proxy = new GlowNodeVisitor(m_parent, node);
    }

    //virtual
    void GlowStateVisitor::visit(gadgets::Parameter const& parameter)
    {
        std::cout << "Parameter " << parameter.identifier() << " Parent = " << m_parent << std::endl;

        m_proxy = new GlowParameterVisitor(m_parent, parameter);
    }

    //virtual
    void GlowStateVisitor::addProperty(gadgets::Property property, int value)
    {
        if (m_proxy)
            m_proxy->addProperty(property, value);
    }

    //virtual
    void GlowStateVisitor::addProperty(gadgets::Property property, double value)
    {
        if (m_proxy)
            m_proxy->addProperty(property, value);
    }

    //virtual
    void GlowStateVisitor::addProperty(gadgets::Property property, String const& value)
    {
        if (m_proxy)
            m_proxy->addProperty(property, value);
    }

    //virtual
    void GlowStateVisitor::addProperty(gadgets::Property property, bool value)
    {
        if (m_proxy)
            m_proxy->addProperty(property, value);
    }

    //virtual
    void GlowStateVisitor::addProperty(gadgets::Property property, EnumList const& enumeration)
    {
        if (m_proxy)
            m_proxy->addProperty(property, enumeration);
    }
}
