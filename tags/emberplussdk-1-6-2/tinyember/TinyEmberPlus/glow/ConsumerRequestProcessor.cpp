#include "ConsumerRequestProcessor.h"
#include "..\gadget\Node.h"
#include "..\gadget\BooleanParameter.h"
#include "..\gadget\EnumParameter.h"
#include "..\gadget\IntegerParameter.h"
#include "..\gadget\RealParameter.h"
#include "..\gadget\StringParameter.h"
#include "..\gadget\util\EntityPath.h"
#include "ConsumerProxy.h"
#include "util\NodeConverter.h"
#include "util\ParameterConverter.h"
#include <ember\Ember.hpp>

using namespace libember;
using namespace gadget::util;

namespace glow
{
    ConsumerRequestProcessor::GlowContainer* ConsumerRequestProcessor::execute(GlowContainer const* request, Node* root, GlowRootElementCollection* response, bool& transmitResponse, gadget::Subscriber* subscriber)
    {
        auto context = Context(false, false, subscriber);
        auto const first = request->begin();
        auto const last = request->end();
        for(auto it = first; it != last; ++it)
        {
            auto const& node = *it;
            auto const type = ber::Type::fromTag(node.typeTag());

            switch(type.value())
            {
                case libember::glow::GlowType::Command:
                {
                    // Report root node
                    auto const& glow = dynamic_cast<GlowCommand const&>(node);
                    if (glow.number().value() == libember::glow::CommandType::GetDirectory)
                    {
                        auto const& settings = ConsumerProxy::settings();
                        auto const responseType = settings.responseBehavior();
                        auto const flags = toNodeFieldFlags(glow.dirFieldMask());

                        if(root->isMounted())
                        {
                            if (responseType.value() == ResponseBehavior::ForceQualifiedContainer)
                            {
                                util::NodeConverter::createQualified(response, root, flags.value);
                            }
                            else
                            {
                                util::NodeConverter::create(response, root, flags.value);
                            }
                            context.setTransmitResponse(true);
                        }
                    }
                    break;
                }
                case libember::glow::GlowType::QualifiedNode:
                {
                    auto const& glow = dynamic_cast<libember::glow::GlowQualifiedNode const&>(node);
                    auto const oid = glow.path();
                    auto const path = EntityPath(oid.begin(), oid.end());
                    auto local = resolve_node(root, path.begin(), path.end());
                    if (local != nullptr)
                    {
                        context.setIsQualifiedRequest(true);
                        executeNode(&glow, local, response, context);
                    }
                    break;
                }
                case libember::glow::GlowType::QualifiedParameter:
                {
                    auto const& glow = dynamic_cast<libember::glow::GlowQualifiedParameter const&>(node);
                    auto const oid = glow.path();
                    auto const path = EntityPath(oid.begin(), oid.end());
                    auto local = resolve_parameter(root, path.begin(), path.end());
                    if (local != nullptr)
                    {
                        context.setIsQualifiedRequest(true);
                        executeParameter(&glow, local, response, context);
                    }
                    break;
                }
                case libember::glow::GlowType::Node:
                {
                    context.setIsQualifiedRequest(false);
                    auto const& glow = dynamic_cast<libember::glow::GlowNode const&>(node);
                    auto const number = glow.number();
                    if (number == root->number())
                    {
                        executeNode(&glow, root, response, context);
                    }
                    break;
                }
                case libember::glow::GlowType::Parameter:
                {
                    /* Parameters at root level are not allowed */
                    break;
                }
            }
        }

        transmitResponse = context.transmitResponse();
        return response;
    }

    void ConsumerRequestProcessor::executeNode(GlowNodeBase const* request, Node* node, GlowRootElementCollection* response, Context& context)
    {
        if (request->empty() == false && node->isMounted() && node->isOnline())
        {
            auto children = request->children();
            
            if (children != nullptr)
            {
                for each(auto& child in *children)
                {
                    auto const type = libember::ber::Type::fromTag(child.typeTag());
                    switch(type.value())
                    {
                        case libember::glow::GlowType::Command:
                        {
                            auto const& command = dynamic_cast<libember::glow::GlowCommand const&>(child);
                            executeCommand(&command, node, response, context);
                            break;
                        }
                        case libember::glow::GlowType::Node:
                        {
                            auto const& glow = dynamic_cast<libember::glow::GlowNode const&>(child);
                            auto const& nodes = node->nodes();
                            auto const first = std::begin(nodes);
                            auto const last = std::end(nodes);
                            auto const number = glow.number();
                            auto result = std::find_if(first, last, [&glow](Node const* child) -> bool
                            {
                                return glow.number() == child->number();
                            });

                            if (result != last)
                            {
                                executeNode(&glow, *result, response, context);
                            }
                            break;
                        }
                        case libember::glow::GlowType::Parameter:
                        {
                            auto const& glow = dynamic_cast<libember::glow::GlowParameter const&>(child);
                            auto const& parameters = node->parameters();
                            auto const first = std::begin(parameters);
                            auto const last = std::end(parameters);
                            auto result = std::find_if(first, last, [&glow](Parameter const* child) -> bool
                            {
                                return glow.number() == child->number();
                            });

                            if (result != last)
                            {
                                executeParameter(&glow, *result, response, context);
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    void ConsumerRequestProcessor::executeCommand(GlowCommand const* command, Node* node, GlowRootElementCollection* response, Context& context)
    {
        if (command->number().value() == libember::glow::CommandType::GetDirectory)
        {
            context.setTransmitResponse(true);
            auto const& nodes = node->nodes();
            auto const& parameters = node->parameters();
            auto const& settings = ConsumerProxy::settings();

            auto const mask = command->dirFieldMask();
            auto const nodeFlags = toNodeFieldFlags(mask);
            auto const parameterFlags = toParameterFieldFlags(mask);
            auto const behavior = settings.responseBehavior().value();
            auto const isLeafNode = nodes.empty() && parameters.empty();

            // When the node does not contain any children, it will be reported without 
            // any properties set.
            if (isLeafNode)
            {
                if (behavior == ResponseBehavior::ForceQualifiedContainer 
                || (context.isQualifiedRequest() && behavior != ResponseBehavior::ForceExpandedContainer))
                {
                    util::NodeConverter::createQualified(response, node);
                }
                else
                {
                    util::NodeConverter::createStructured(response, node);
                }
            }
            else
            {
                if (behavior == ResponseBehavior::ForceQualifiedContainer 
                || (context.isQualifiedRequest() && behavior != ResponseBehavior::ForceExpandedContainer))
                {
                    for each(auto child in nodes)
                    {
                        if(child->isMounted())
                        {
                            util::NodeConverter::createQualified(response, child, nodeFlags.value);
                        }
                    }

                    for each(auto parameter in parameters)
                    {
                        util::ParameterConverter::createQualified(response, parameter, parameterFlags.value);
                    }
                }
                else
                {
                    auto local = util::NodeConverter::createStructured(response, node);
                    auto container = local->children();

                    for each(auto parameter in parameters)
                    {
                        util::ParameterConverter::create(container, parameter, parameterFlags.value);
                    }
                    for each(auto child in nodes)
                    {
                        util::NodeConverter::create(container, child, nodeFlags.value);
                    }
                }
            }
        }
    }

    void ConsumerRequestProcessor::executeParameter(GlowParameterBase const* request, Parameter* parameter, GlowRootElementCollection* response, Context& context)
    {
        if (request->empty() == false)
        {
            if (request->contains(libember::glow::ParameterProperty::Value))
            {
                auto const value = request->value();
                auto const type = parameter->type();
                auto const forceNotification = true;
                switch(type.value())
                {
                    case gadget::ParameterType::Boolean:
                    {
                        auto boolean = dynamic_cast<gadget::BooleanParameter*>(parameter);
                        boolean->setValue(value.toBoolean(), forceNotification);
                        break;
                    }
                    case gadget::ParameterType::Enum:
                    {
                        auto enumeration = dynamic_cast<gadget::EnumParameter*>(parameter);
                        enumeration->setIndex(static_cast<gadget::EnumParameter::size_type>(value.toInteger()), forceNotification);
                        break;
                    }
                    case gadget::ParameterType::Integer:
                    {
                        auto integer = dynamic_cast<gadget::IntegerParameter*>(parameter);
                        integer->setValue(value.toInteger(), forceNotification);
                        break;
                    }
                    case gadget::ParameterType::Real:
                    {
                        auto real = dynamic_cast<gadget::RealParameter*>(parameter);
                        real->setValue(value.toReal(), forceNotification);
                        break;
                    }
                    case gadget::ParameterType::String:
                    {
                        auto string = dynamic_cast<gadget::StringParameter*>(parameter);
                        string->setValue(value.toString(), forceNotification);
                        break;
                    }
                }
            }

            auto children = request->children();
            if (children != nullptr)
            {
                for each(auto& child in *children)
                {
                    auto const type = libember::ber::Type::fromTag(child.typeTag());
                    switch(type.value())
                    {
                        case libember::glow::GlowType::Command:
                            auto const& command = dynamic_cast<libember::glow::GlowCommand const&>(child);
                            executeCommand(&command, parameter, response, context);
                            break;
                    }
                }
            }
        }
    }

    void ConsumerRequestProcessor::executeCommand(GlowCommand const* command, Parameter* parameter, GlowRootElementCollection* response, Context& context)
    {
        auto const number = command->number();
        if (number.value() == libember::glow::CommandType::GetDirectory)
        {
            context.setTransmitResponse(true);
            auto const& settings = ConsumerProxy::settings();
            auto const flags = toParameterFieldFlags(command->dirFieldMask());
            auto const behavior = settings.responseBehavior().value();

            if (behavior == ResponseBehavior::ForceQualifiedContainer
            || (context.isQualifiedRequest() && behavior != ResponseBehavior::ForceExpandedContainer))
            {
                util::ParameterConverter::createQualified(response, parameter, flags.value);
            }
            else
            {
                util::ParameterConverter::createStructured(response, parameter, flags.value);
            }
        }
        else if (number.value() == libember::glow::CommandType::Subscribe)
        {
            parameter->subscribe(context.subscriber());
        }
        else if (number.value() == libember::glow::CommandType::Unsubscribe)
        {
            parameter->unsubscribe(context.subscriber());
        }
    }

    gadget::NodeField ConsumerRequestProcessor::toNodeFieldFlags(libember::glow::DirFieldMask const& mask)
    {
        auto const value = mask.value();
        if (value == libember::glow::DirFieldMask::Default)
        {
            return gadget::NodeField::All;
        }
        else if (value != libember::glow::DirFieldMask::All)
        {
            auto flags = 0;
            if (value & libember::glow::DirFieldMask::Description)
                flags |= gadget::NodeField::Description;

            if (value & libember::glow::DirFieldMask::Identifier)
                flags |= gadget::NodeField::Identifier;

            return flags;
        }
        else
        {
            return gadget::NodeField::All;
        }
    }

    gadget::ParameterField ConsumerRequestProcessor::toParameterFieldFlags(libember::glow::DirFieldMask const& mask)
    {
        auto const value = mask.value();

        if (value == libember::glow::DirFieldMask::Default)
        {
            return gadget::ParameterField::All;
        }
        else if (value != libember::glow::DirFieldMask::All)
        {
            auto flags = 0;
            if (value & libember::glow::DirFieldMask::Description)
                flags |= gadget::ParameterField::Description;

            if (value & libember::glow::DirFieldMask::Identifier)
                flags |= gadget::ParameterField::Identifier;

            if (value & libember::glow::DirFieldMask::Value)
                flags |= gadget::ParameterField::Value;

            return flags;
        }
        else
        {
            return gadget::ParameterField::All;
        }
    }
}
