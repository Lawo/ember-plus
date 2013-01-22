#include "../Libember.h"
#include "../gadgets/Node.h"
#include "GlowNodeVisitor.h"
#include "GlowStateVisitor.h"

namespace glow
{
    GlowNodeVisitor::GlowNodeVisitor(libember::glow::GlowElementCollection* parent, gadgets::Node const& node)
        : m_node(new libember::glow::GlowNode(node.number()))
    {
        parent->insertElement(m_node);
    }

    //virtual
    GlowStateVisitor* GlowNodeVisitor::createChildVisitor()
    {
        return new GlowStateVisitor(m_node->children());
    }

    //virtual
    void GlowNodeVisitor::addProperty(gadgets::Property, int)
    {
    }

    //virtual
    void GlowNodeVisitor::addProperty(gadgets::Property, double)
    {
    }

    //virtual
    void GlowNodeVisitor::addProperty(gadgets::Property property, String const& value)
    {
        switch(property.value)
        {
        case gadgets::Property::Identifier:
            m_node->setIdentifier(value);
            break;

        case gadgets::Property::Description:
            m_node->setDescription(value);
            break;
        }
    }

    //virtual
    void GlowNodeVisitor::addProperty(gadgets::Property, bool)
    {
    }

    //virtual
    void GlowNodeVisitor::addProperty(gadgets::Property, EnumList const&)
    {
    }
}
