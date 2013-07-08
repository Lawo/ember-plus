#ifndef __TINYEMBER_GLOW_CONSUMERREQUESTPROCESSOR_H
#define __TINYEMBER_GLOW_CONSUMERREQUESTPROCESSOR_H

#include "../gadget/ParameterField.h"
#include "../gadget/NodeField.h"

/** Forward declarations */
namespace libember { namespace glow
{
    struct DirFieldMask;

    class GlowContainer;
    class GlowCommand;
    class GlowNode;
    class GlowNodeBase;
    class GlowParameter;
    class GlowParameterBase;
    class GlowQualifiedNode;
    class GlowQualifiedParameter;
    class GlowRootElementCollection;
}
}

namespace gadget
{
    class Node;
    class Parameter;
    class Subscriber;
}

namespace glow
{
    /**
     * This class processes incoming consumer requests.
     */
    class ConsumerRequestProcessor
    {
        /**
         * Helper struct which stores some local settings while processing a request.
         */
        struct Context
        {
            /**
             * Initializes a new Context.
             * @param isQualifiedRequest Indicates whether qualified types have been used in the request
             *      sent by the consumer.
             * @param transmitResponse Indicates whether it is necessary to transmit the resulting response.
             * @param subscriber A pointer to a subscriber who represents the consumer that sent the request.
             */
            explicit Context(bool isQualifiedRequest, bool transmitResponse, gadget::Subscriber* subscriber)
                : m_isQualifiedRequest(isQualifiedRequest)
                , m_transmitResponse(transmitResponse)
                , m_subscriber(subscriber)
            {}

            /**
             * Updates the isQualifiedRequest property.
             * @param value The new value to assume.
             */
            void setIsQualifiedRequest(bool value)
            {
                m_isQualifiedRequest = value;
            }

            /**
             * Updates the transmitResponse property.
             * @param value The new value to assume.
             */
            void setTransmitResponse(bool value)
            {
                m_transmitResponse = value;
            }

            /**
             * Returns the isQualifiedRequest property.
             * @return true, if the request contained qualified types.
             */
            bool isQualifiedRequest() const
            {
                return m_isQualifiedRequest;
            }

            /**
             * Returns the transmitResponse property.
             * @return true, if the generated response shall be transmitted.
             */
            bool transmitResponse() const
            {
                return m_transmitResponse;
            }

            /**
             * Returns the subscriber that represents the consumer who sent the request.
             * @return The subscriber object.
             */
            gadget::Subscriber* subscriber() const
            {
                return m_subscriber;
            }

            private:
                gadget::Subscriber* m_subscriber;
                bool m_isQualifiedRequest;
                bool m_transmitResponse;
        };
        public:
            typedef libember::glow::GlowContainer GlowContainer;
            typedef libember::glow::GlowNode GlowNode;
            typedef libember::glow::GlowParameter GlowParameter;
            typedef libember::glow::GlowCommand GlowCommand;
            typedef libember::glow::GlowNodeBase GlowNodeBase;
            typedef libember::glow::GlowParameterBase GlowParameterBase;
            typedef libember::glow::GlowQualifiedNode GlowQualifiedNode;
            typedef libember::glow::GlowQualifiedParameter GlowQualifiedParameter;
            typedef libember::glow::GlowRootElementCollection GlowRootElementCollection;

            typedef gadget::Node Node;
            typedef gadget::Parameter Parameter;

            /**
             * Executes a consumer request and generates a response.
             * @param request The decoded consumer request.
             * @param root The root node of the local provider.
             * @param response The root element collection to append the response to.
             * @param transmitResponse A reference to a boolean parameter which will be set to true
             *      when the generated response needs to be transmitted to the consumer.
             * @param subscriber The subscriber representing the consumer.
             * @return The response root node.
             */
            static GlowContainer* execute(GlowContainer const* request, Node* root, GlowRootElementCollection* response, bool& transmitResponse, gadget::Subscriber* subscriber);

        private:
            /**
             * Scans a node.
             * @param request The current node within the request that is being processed.
             * @param node A pointer to the local provider node the request was asking for. This node may be null if it could 
             *      not be found.
             * @param response The root element collection to append a response to.
             * @param context The context containing some local settings.
             */
            static void executeNode(GlowNodeBase const* request, Node* node, GlowRootElementCollection* response, Context& context);

            /**
             * Scans a parameter.
             * @param request The current parameter within the request that is being processed.
             * @param parameter A pointer to the local provider parameter the request was asking for. This parameter may be null if it could 
             *      not be found.
             * @param response The root element collection to append a response to.
             * @param context The context containing some local settings.
             */
            static void executeParameter(GlowParameterBase const* request, Parameter* parameter, GlowRootElementCollection* response, Context& context);

            /**
             * Scans a command that has been appended to a node.
             * @param command The command to scan.
             * @param node The node the command has to be applied on.
             * @param response The root element collection to append a response to.
             * @param context The context containing some local settings.
             */
            static void executeCommand(GlowCommand const* command, Node* node, GlowRootElementCollection* response, Context& context);

            /**
             * Scans a command that has been appended to a parameter.
             * @param command The command to scan.
             * @param parameter The parameter the command has to be applied on.
             * @param response The root element collection to append a response to.
             * @param context The context containing some local settings.
             */
            static void executeCommand(GlowCommand const* command, Parameter* parameter, GlowRootElementCollection* response, Context& context);

            /**
             * Converts a DirFieldMask into a new NodeField instance which contains all flags for the
             * properties that are requested by the mask.
             * @param mask The mask to convert.
             * @return A NodeField instance which contains all properties requested by the mask.
             */
            static gadget::NodeField toNodeFieldFlags(libember::glow::DirFieldMask const& mask);

            /**
             * Converts a DirFieldMask into a new ParameterField instance which contains all flags for the
             * properties that are requested by the mask.
             * @param mask The mask to convert.
             * @return A ParameterField instance which contains all properties requested by the mask.
             */
            static gadget::ParameterField toParameterFieldFlags(libember::glow::DirFieldMask const& mask);
    };
}

#endif//__TINYEMBER_GLOW_CONSUMERREQUESTPROCESSOR_H
