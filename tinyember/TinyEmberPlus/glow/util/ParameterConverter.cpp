#include <ember/Ember.hpp>
#include "NodeConverter.h"
#include "ParameterConverter.h"
#include "../ConsumerProxy.h"
#include "../../gadget/BooleanParameter.h"
#include "../../gadget/EnumParameter.h"
#include "../../gadget/StringParameter.h"
#include "../../gadget/IntegerParameter.h"
#include "../../gadget/RealParameter.h"
#include "../../gadget/util/EntityPath.h"

using namespace gadget;
using namespace libember;
using namespace libember::glow;

namespace glow { namespace util 
{
    libember::glow::GlowParameterBase* ParameterConverter::create(libember::glow::GlowContainer* parent, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields)
    {
        auto const result = ParameterConverter(parent, parameter, fields, false);
        return result.m_parameter;
    }

    libember::glow::GlowParameterBase* ParameterConverter::createQualified(libember::glow::GlowRootElementCollection* root, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields)
    {
        auto const result = ParameterConverter(root, parameter, fields, true);
        return result.m_parameter;
    }

    libember::glow::GlowParameterBase* ParameterConverter::createStructured(libember::glow::GlowRootElementCollection* root, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields)
    {
        auto parent = parameter->parent();
        if (parent != nullptr)
        {
            auto owner = NodeConverter::createStructured(root, parent);
            auto children = owner->children();
            auto local = ParameterConverter::create(children, parameter, fields);
            return local;
        }
        else
        {
            return create(root, parameter, fields);
        }
    }

    ParameterConverter::ParameterConverter(libember::glow::GlowContainer* parent, gadget::Parameter const* parameter, gadget::ParameterFieldState const& fields, bool makeQualified)
        : m_fields(fields)
    {
        if (makeQualified)
        {
            auto path = gadget::util::make_path(parameter);
            m_parameter = new GlowQualifiedParameter(libember::ber::ObjectIdentifier(path.begin(), path.end()));
        }
        else
        {
            m_parameter = static_cast<GlowParameterBase*>(new GlowParameter(parameter->number()));
        }

        if (parent != nullptr)
            parent->insert(parent->end(), m_parameter);

        if (m_fields.isSet(ParameterField::Identifier))
            m_parameter->setIdentifier(parameter->identifier());

        if (m_fields.isSet(ParameterField::Description) && parameter->description().size() > 0)
            m_parameter->setDescription(parameter->description());

        if (m_fields.isSet(ParameterField::Schema))
        {
            String const& schema = parameter->schema();

            if (schema.empty() == false)
            {
                m_parameter->setSchemaIdentifiers(schema);
            }
        }

        if (m_fields.isSet(ParameterField::Type) || parameter->type().value() == gadget::ParameterType::Trigger)
        {
            switch(parameter->type().value())
            {
                case gadget::ParameterType::Boolean:
                    m_parameter->setType(libember::glow::ParameterType::Boolean);
                    break;

                case gadget::ParameterType::Enum:
                    m_parameter->setType(libember::glow::ParameterType::Enum);
                    break;

                case gadget::ParameterType::Integer:
                    m_parameter->setType(libember::glow::ParameterType::Integer);
                    break;

                case gadget::ParameterType::Octets:
                    m_parameter->setType(libember::glow::ParameterType::Octets);
                    break;

                case gadget::ParameterType::Real:
                    m_parameter->setType(libember::glow::ParameterType::Real);
                    break;

                case gadget::ParameterType::String:
                    m_parameter->setType(libember::glow::ParameterType::String);
                    break;

                case gadget::ParameterType::Trigger:
                    m_parameter->setType(libember::glow::ParameterType::Trigger);
                    break;
            }
        }

        if (m_fields.isSet(ParameterField::StreamIdentifier) && parameter->hasStreamIdentifier())
        {
            m_parameter->setStreamIdentifier(parameter->streamIdentifier());
        }

        if (m_fields.isSet(ParameterField::StreamDescriptor) && parameter->hasStreamDescriptor())
        {
            auto descriptor = parameter->streamDescriptor();
            auto const format = static_cast<libember::glow::StreamFormat::_Domain>(descriptor->format().value());
            auto const offset = descriptor->offset();
            m_parameter->setStreamDescriptor(format, offset);
        }

        if (m_fields.isSet(ParameterField::Access))
        {
            switch(parameter->access().value())
            {
                case gadget::Access::None:
                    m_parameter->setAccess(libember::glow::Access::None);
                    break;

                case gadget::Access::ReadOnly:
                    m_parameter->setAccess(libember::glow::Access::ReadOnly);
                    break;

                case gadget::Access::ReadWrite:
                    m_parameter->setAccess(libember::glow::Access::ReadWrite);
                    break;

                case gadget::Access::WriteOnly:
                    m_parameter->setAccess(libember::glow::Access::WriteOnly);
                    break;
            }
        }

        if (m_fields.isSet(ParameterField::ValueFormula))
        {
            auto const& formula = parameter->formula();
            if (formula.valid())
                m_parameter->setFormula(formula.providerToConsumer(), formula.consumerToProvider());
        }

        parameter->accept(*this);
    }

    void ParameterConverter::visit(gadget::BooleanParameter const* parameter) const
    {
        if (m_fields.isSet(ParameterField::Value))
            m_parameter->setValue(parameter->value());
    }

    void ParameterConverter::visit(gadget::EnumParameter const* parameter) const
    {
        if (m_fields.isSet(ParameterField::ValueEnumeration))
        {
            if (ConsumerProxy::settings().useEnumMap())
            {
                auto enumeration = libember::glow::Enumeration(parameter->begin(), parameter->end());

                m_parameter->setEnumerationMap(enumeration.begin(), enumeration.end());
            }
            else
            {
                m_parameter->setEnumeration(parameter->begin(), parameter->end());
            }
        }

        if (m_fields.isSet(ParameterField::Value))
            m_parameter->setValue(static_cast<int>(parameter->index()));
    }

    void ParameterConverter::visit(gadget::StringParameter const* parameter) const
    {
        if (m_fields.isSet(ParameterField::ValueMax) && parameter->isIndefiniteLength() == false)
            m_parameter->setMaximum(static_cast<int>(parameter->maxLength()));

        if (m_fields.isSet(ParameterField::Value))
            m_parameter->setValue(parameter->value());
    }

    void ParameterConverter::visit(gadget::IntegerParameter const* parameter) const
    {
        auto const format = m_parameter->format();

        if (m_fields.isSet(ParameterField::ValueMax))
            m_parameter->setMaximum(parameter->maximum());

        if (m_fields.isSet(ParameterField::ValueMin))
            m_parameter->setMinimum(parameter->minimum());

        if (m_fields.isSet(ParameterField::ValueFormat) && format.size() > 0)
            m_parameter->setFormat(format);

        if (m_fields.isSet(ParameterField::Value))
            m_parameter->setValue(static_cast<int>(parameter->value()));
    }

    void ParameterConverter::visit(gadget::RealParameter const* parameter) const
    {
        auto const format = m_parameter->format();

        if (m_fields.isSet(ParameterField::ValueMax))
            m_parameter->setMaximum(parameter->maximum());

        if (m_fields.isSet(ParameterField::ValueMin))
            m_parameter->setMinimum(parameter->minimum());

        if (m_fields.isSet(ParameterField::ValueFormat) && format.size() > 0)
            m_parameter->setFormat(format);

        if (m_fields.isSet(ParameterField::Value))
            m_parameter->setValue(static_cast<double>(parameter->value()));
    }
}
}
