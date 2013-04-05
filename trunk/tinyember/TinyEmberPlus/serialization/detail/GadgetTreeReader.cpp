#include <vector>
#include "GadgetTreeReader.h"
#include "../../gadget/BooleanParameter.h"
#include "../../gadget/EnumParameter.h"
#include "../../gadget/IntegerParameter.h"
#include "../../gadget/RealParameter.h"
#include "../../gadget/StringParameter.h"
#include "../../gadget/NodeFactory.h"
#include "../../gadget/ParameterFactory.h"
#include "../../glow/util/NodeConverter.h"
#include "../../glow/util/ParameterConverter.h"

using namespace libember;
using namespace libember::glow;

namespace serialization { namespace detail
{
    GadgetTreeReader::GadgetTreeReader(libember::util::OctetStream& input)
    {
        m_root = nullptr;

        auto& factory = GlowNodeFactory::getFactory();
        auto reader = dom::DomReader();
        auto node = reader.decodeTree(input, factory);

        if (node != nullptr)
        {
            auto const type = ber::Type::fromTag(node->typeTag());
            if(type.isApplicationDefined())
            {
                if (type.value() == GlowType::RootElementCollection)
                {
                    auto collection = dynamic_cast<GlowRootElementCollection*>(node);
                    if (collection != nullptr)
                    {
                        iterate(collection);
                    }
                }
            }
        }
    }

    GadgetTreeReader::~GadgetTreeReader()
    {}

    void GadgetTreeReader::iterate(libember::glow::GlowRootElementCollection* collection)
    {
        auto const last = std::end(*collection);
        for(auto it = std::begin(*collection); it != last; ++it)
        {
            auto& child = *it;
            auto const type = ber::Type::fromTag(child.typeTag());
            if (type.isApplicationDefined())
            {
                if (type.value() == GlowType::Node)
                {
                    auto& node = *dynamic_cast<GlowNode*>(&child);
                    if (m_root == nullptr)
                    {
                        m_root = gadget::NodeFactory::createRoot(node.identifier());
                        m_root->setDescription(node.description());
                        
                        iterate(m_root, node.children());
                        return;
                    }
                }
            }
        }
    }

    void GadgetTreeReader::iterate(gadget::Node* parent, libember::glow::GlowElementCollection* collection)
    {
        if (collection != nullptr)
        {
            auto const last = std::end(*collection);
            for(auto it = std::begin(*collection); it != last; ++it)
            {
                auto& child = *it;
                auto const type = ber::Type::fromTag(child.typeTag());
                if (type.isApplicationDefined())
                {
                    if (type.value() == GlowType::Node)
                    {
                        auto& glownode = *dynamic_cast<GlowNode*>(&child);
                        auto node = gadget::NodeFactory::createNode(parent, glownode.identifier());
                        node->setDescription(glownode.description());

                        iterate(node, glownode.children());
                    }
                    else if (type.value() == GlowType::Parameter)
                    {
                        auto& glowparam = dynamic_cast<GlowParameter&>(child);
                        auto identifier = glowparam.identifier();
                        auto paramtype = glowparam.effectiveType();
                        switch(paramtype.value())
                        {
                            case libember::glow::ParameterType::Boolean:
                                transform(gadget::ParameterFactory::create(parent, identifier, false), &glowparam);
                                break;

                            case libember::glow::ParameterType::Enum:
                                transform(gadget::ParameterFactory::create(parent, identifier), &glowparam);
                                break;

                            case libember::glow::ParameterType::Integer:
                                transform(gadget::ParameterFactory::create(parent, identifier, 0, 1000, 0), &glowparam);
                                break;

                            case libember::glow::ParameterType::Octets:
                                break;

                            case libember::glow::ParameterType::Real:
                                transform(gadget::ParameterFactory::create(parent, identifier, 0.0, 1000.0, 0.0), &glowparam);
                                break;

                            case libember::glow::ParameterType::String:
                                transform(gadget::ParameterFactory::create(parent, identifier, std::string("text")), &glowparam);
                                break;

                            case libember::glow::ParameterType::Trigger:
                                break;
                        }
                    }
                }
            }
        }
    }

    void GadgetTreeReader::transformBase(gadget::Parameter* param, libember::glow::GlowParameter* source) const
    {
        param->setDescription(source->description());
        param->setAccess(static_cast<gadget::Access::_Domain>(source->access().value()));
        
        if (source->contains(libember::glow::ParameterProperty::Formula))
        {
            auto const formula = source->formula();
            param->setFormula(gadget::Formula(formula.providerToConsumer(), formula.consumerToProvider()));
        }

        if (source->contains(libember::glow::ParameterProperty::StreamDescriptor))
        {
            auto const descriptor = source->streamDescriptor();
            auto const format = static_cast<gadget::StreamFormat::_Domain>(descriptor->format().value());
            param->setStreamDescriptor(format, descriptor->offset());
        }

        if (source->contains(libember::glow::ParameterProperty::StreamIdentifier))
        {
            auto identifier = source->streamIdentifier();
            param->setStreamIdentifier(identifier);
        }
    }

    void GadgetTreeReader::transform(gadget::EnumParameter* param, libember::glow::GlowParameter* source) const
    {
        transformBase(param, source);

        auto const enumeration = source->contains(libember::glow::ParameterProperty::EnumMap)
            ? source->enumerationMap()
            : source->enumeration();
        auto enumstrings = std::vector<std::string>();

        std::transform(std::begin(enumeration), std::end(enumeration), std::back_inserter(enumstrings), 
            [](libember::glow::Enumeration::value_type const& tuple) -> std::string
        {
            return tuple.first;
        });

        param->assign(std::begin(enumstrings), std::end(enumstrings));
        param->setIndex(source->value().toInteger());
    }

    void GadgetTreeReader::transform(gadget::IntegerParameter* param, libember::glow::GlowParameter* source) const
    {
        transformBase(param, source);

        if (source->contains(libember::glow::ParameterProperty::Minimum))
            param->setMin(source->minimum().toInteger());

        if (source->contains(libember::glow::ParameterProperty::Maximum))
            param->setMax(source->maximum().toInteger());

        if (source->contains(libember::glow::ParameterProperty::Value))
            param->setValue(source->value().toInteger());
    }

    void GadgetTreeReader::transform(gadget::RealParameter* param, libember::glow::GlowParameter* source) const
    {
        transformBase(param, source);

        if (source->contains(libember::glow::ParameterProperty::Minimum))
            param->setMin(source->minimum().toReal());

        if (source->contains(libember::glow::ParameterProperty::Maximum))
            param->setMax(source->maximum().toReal());

        if (source->contains(libember::glow::ParameterProperty::Value))
            param->setValue(source->value().toReal());
    }

    void GadgetTreeReader::transform(gadget::StringParameter* param, libember::glow::GlowParameter* source) const
    {
        transformBase(param, source);

        if (source->contains(libember::glow::ParameterProperty::Maximum))
            param->setMaxLength(source->maximum().toInteger());

        if (source->contains(libember::glow::ParameterProperty::Value))
            param->setValue(source->value().toString());
    }

    void GadgetTreeReader::transform(gadget::BooleanParameter* param, libember::glow::GlowParameter* source) const
    {
        transformBase(param, source);

        if (source->contains(libember::glow::ParameterProperty::Value))
            param->setValue(source->value().toBoolean());
    }
}
}
