#include "Libember.h"
#include "GlowClientMessageHandler.h"
#include "GlowElementVisitor.h"
#include "GlowStateVisitor.h"
#include "../gadgets/states/EnumState.h"
#include "../gadgets/states/NumericState.h"
#include "../gadgets/states/StringState.h"
#include "../gadgets/Node.h"
#include "../gadgets/Parameter.h"

using namespace libember;
using namespace libember::ber;
using namespace libember::dom;
using namespace libember::glow;
using namespace ::gadgets;
using namespace ::glow;


namespace
{
    /**
     * Helper struct used to compare an entity's number with
     * a number.
     */
    struct FindEntityByNumber
    {
        typedef Entity::number_type number_type;

        /**
         * Constructor
         * @param number Entity number
         */
        explicit FindEntityByNumber(number_type number)
            : number(number)
        {}

        /**
         * Compares this instance's number with the number of the provided entity.
         * @param entity Entity to compare.
         */
        bool operator()(Entity const* entity) const
        {
            return entity->number() == number;
        }

        number_type const number;
    };
}

namespace glow
{
    GlowClientMessageHandler::GlowClientMessageHandler()
    {
    }

    libember::glow::GlowElementCollection* GlowClientMessageHandler::process(GlowElementCollection const* request, Entity* entity, GlowElementCollection* response) const
    {
        GlowElementCollection::const_iterator first = request->begin();
        GlowElementCollection::const_iterator const last = request->end();

        for(/** Nothing **/; first != last; ++first)
        {
            dom::Node const& child = *first;
            ber::Type const type = ber::Type::fromTag(child.typeTag());

            switch(type.value())
            {
                case GlowType::Command:
                {
                    GlowCommand const& command = dynamic_cast<GlowCommand const&>(child);
                    GlowStateVisitor visitor(response);

                    // When we detect a GetDirectory command at root level, we must simple
                    // response with the current root node.
                    if (command.number() == CommandType::GetDirectory)
                    {
                        entity->visit(visitor, gadgets::Property::All);
                    }
                    break;
                }
                case GlowType::Node:
                {
                    // When we detect a Node at root level, we must compare its number
                    // with the root entity.
                    GlowNode const& node = dynamic_cast<GlowNode const&>(child);

                    Entity::number_type const number = static_cast<Entity::number_type>(node.number());

                    if (number == entity->number())
                    {
                        GlowStateVisitor visitor(response);

                        processNode(node, entity, visitor);
                    }
                    break;
                }

                // Parameters are not allowed at root level
            }
        }

        return response;
    }

    void GlowClientMessageHandler::processChildren(GlowElementCollection const* request, gadgets::Entity* entity, GlowStateVisitor& response) const
    {
        GlowElementCollection::const_iterator first = request->begin();
        GlowElementCollection::const_iterator const last = request->end();

        for(; first != last; ++first)
        {
            dom::Node const& child = *first;
            ber::Type const type = Type::fromTag(child.typeTag());

            switch(type.value())
            {
                case GlowType::Command:
                {
                    processCommand(dynamic_cast<GlowCommand const&>(child), entity, response);
                    break;
                }
                case GlowType::Parameter:
                {
                    GlowParameter const& parameter = dynamic_cast<GlowParameter const&>(child);

                    gadgets::Entity::iterator const first = entity->begin();
                    gadgets::Entity::iterator const last = entity->end();
                    gadgets::Entity::iterator const it = std::find_if(first, last, FindEntityByNumber(parameter.number()));

                    if (it != last)
                        processParameter(parameter, *it, *response.createChildVisitor());

                    break;
                }
                case GlowType::Node:
                {
                    GlowNode const& node = dynamic_cast<GlowNode const&>(child);

                    gadgets::Entity::iterator const first = entity->begin();
                    gadgets::Entity::iterator const last = entity->end();
                    gadgets::Entity::iterator const it = std::find_if(first, last, FindEntityByNumber(node.number()));

                    if (it != last)
                        processNode(node, *it, *response.createChildVisitor());

                    break;
                }
            }
        }
    }

    void GlowClientMessageHandler::processCommand(libember::glow::GlowCommand const& command, gadgets::Entity* entity, GlowStateVisitor& response) const
    {
        if (command.number() == libember::glow::CommandType::GetDirectory)
        {
            gadgets::Entity::const_iterator first = entity->begin();
            gadgets::Entity::const_iterator const last = entity->end();

            for(; first != last; ++first)
            {
                GlowStateVisitor *const visitor = response.createChildVisitor();
                (*first)->visit(*visitor, gadgets::Property::All);
            }
        }
    }

    void GlowClientMessageHandler::processParameter(libember::glow::GlowParameter const& parameter, gadgets::Entity* entity, GlowStateVisitor& response) const
    {
        if (parameter.contains(GlowProperty::Value))
        {
            Parameter *const parameterEntity = dynamic_cast<Parameter*>(entity);
            libember::glow::Value const value = parameter.value();

            if (parameterEntity != 0)
            {
                parameterEntity->trySetValue(value);

                entity->visit(response, gadgets::Property::Value);
            }
        }
    }

    void GlowClientMessageHandler::processNode(libember::glow::GlowNode const& node, gadgets::Entity* entity, GlowStateVisitor& response) const
    {
        if (node.contains(GlowProperty::Children))
        {
            GlowElementCollection const* children = node.children();

            entity->visit(response, gadgets::Property::None);
            processChildren(children, entity, response);
        }
    }
}
