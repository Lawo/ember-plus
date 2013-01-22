#include "../Libember.h"
#include "../gadgets/Parameter.h"
#include "GlowParameterVisitor.h"
#include "GlowStateVisitor.h"
#include <algorithm>
#include <sstream>

using namespace gadgets;
using namespace libember;
using namespace libember::glow;

namespace glow
{
    GlowParameterVisitor::GlowParameterVisitor(libember::glow::GlowElementCollection* parent, gadgets::Parameter const& parameter)
        : m_parameter(new libember::glow::GlowParameter(parameter.number()))
    {
        parent->insertElement(m_parameter);
    }

    //virtual
    GlowStateVisitor* GlowParameterVisitor::createChildVisitor()
    {
        return new GlowStateVisitor(m_parameter->children());
    }

    //virtual
    void GlowParameterVisitor::addProperty(gadgets::Property property, int value)
    {
        switch(property.value)
        {
        case Property::Value:
            m_parameter->setValue(value);
            break;

        case Property::ValueMax:
            m_parameter->setMaximum(value);
            break;

        case Property::ValueMin:
            m_parameter->setMinimum(value);
            break;

        case Property::ValueStep:
            // Not supported yet
            break;

        case Property::ValueFactor:
            m_parameter->setFactor(value);
            break;
        }
    }

    //virtual
    void GlowParameterVisitor::addProperty(gadgets::Property property, double value)
    {
        switch(property.value)
        {
        case Property::Value:
            m_parameter->setValue(value);
            break;

        case Property::ValueMax:
            m_parameter->setMaximum(value);
            break;

        case Property::ValueMin:
            m_parameter->setMinimum(value);
            break;
        }
    }

    //virtual
    void GlowParameterVisitor::addProperty(gadgets::Property property, String const& value)
    {
        switch(property.value)
        {
        case Property::Identifier:
            m_parameter->setIdentifier(value);
            break;

        case Property::Description:
            m_parameter->setDescription(value);
            break;
        }
    }

    //virtual
    void GlowParameterVisitor::addProperty(Property property, bool value)
    {
        switch(property.value)
        {
        case Property::IsCommand:
            m_parameter->setIsCommand(value);
            break;

        case Property::IsWriteable:
            m_parameter->setIsWriteable(value);
            break;
        }
    }

    //virtual
    void GlowParameterVisitor::addProperty(gadgets::Property property, EnumList const& enumeration)
    {
        switch(property.value)
        {
        case Property::ValueEnumeration:
            m_parameter->setEnumeration(enumeration.begin(), enumeration.end());
            break;
        }
    }
}
