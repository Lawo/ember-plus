#ifndef GLOWCLIENTMESSAGEHANDLER_H
#define GLOWCLIENTMESSAGEHANDLER_H

/** Forward declarations **/
namespace libember { namespace glow
{
    class GlowElementCollection;
    class GlowCommand;
    class GlowNode;
    class GlowParameter;
}
}

namespace gadgets
{
    class Entity;
    class Node;
    class Parameter;
}

namespace glow
{
    /** Forward declarations **/
    class GlowStateVisitor;

    /**
     * This class manages incoming glow requests.
     */
    class GlowClientMessageHandler
    {
    public:
        /**
         *  Constructor
         */
        GlowClientMessageHandler();

        /**
         * Processes a glow request. All nodes being visited will be attached to the provided response
         * collection. The collection can then be transmitted to the client(s).
         * @param request Decoded request.
         * @param entity The root node of the TinyEmber structure.
         * @param response The root collection where the response will be attached to.
         * @return Returns the response collection.
         */
        libember::glow::GlowElementCollection* process(libember::glow::GlowElementCollection const* request, gadgets::Entity* entity, libember::glow::GlowElementCollection* response) const;

    private:
        /**
         * Processes the children of a collection
         * @param request Decoded child collection.
         * @param entity The current entity.
         * @param response The response to attach the current entity to or additional information.
         */
        void processChildren(libember::glow::GlowElementCollection const* request, gadgets::Entity* entity, GlowStateVisitor& response) const;

        /**
         * Processes a single command.
         * @param command Command.
         * @param entity Current entity.
         * @param response response node to attach additional information to.
         */
        void processCommand(libember::glow::GlowCommand const& command, gadgets::Entity* entity, GlowStateVisitor& response) const;

        /**
         * Processes a parameter
         * @param parameter The parameter being traversed.
         * @param entity The current entity.
         * @param response The response where the parameter information may be added to.
         */
        void processParameter(libember::glow::GlowParameter const& parameter, gadgets::Entity* entity, GlowStateVisitor& response) const;

        /**
         * Processes a node.
         * @param node The node being traversed.
         * @param entity The current entity.
         * @param response The response where the node information may be added to.
         */
        void processNode(libember::glow::GlowNode const& node, gadgets::Entity* entity, GlowStateVisitor& response) const;
    };
}

#endif // GLOWCLIENTMESSAGEHANDLER_H
