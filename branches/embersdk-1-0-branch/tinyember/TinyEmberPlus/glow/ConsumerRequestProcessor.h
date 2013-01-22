#ifndef __TINYEMBER_GLOW_CONSUMERREQUESTPROCESSOR_H
#define __TINYEMBER_GLOW_CONSUMERREQUESTPROCESSOR_H

#include "../gadget/ParameterField.h"
#include "../gadget/NodeField.h"

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
    class ConsumerRequestProcessor
    {
        struct Context
        {
            explicit Context(bool isQualifiedRequest, bool transmitResponse, gadget::Subscriber* subscriber)
                : m_isQualifiedRequest(isQualifiedRequest)
                , m_transmitResponse(transmitResponse)
                , m_subscriber(subscriber)
            {}

            void setIsQualifiedRequest(bool value)
            {
                m_isQualifiedRequest = value;
            }

            void setTransmitResponse(bool value)
            {
                m_transmitResponse = value;
            }

            bool isQualifiedRequest() const
            {
                return m_isQualifiedRequest;
            }

            bool transmitResponse() const
            {
                return m_transmitResponse;
            }

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

            static GlowContainer* execute(GlowContainer const* request, Node* root, GlowRootElementCollection* response, bool& transmitTresponse, gadget::Subscriber* subscriber);

        private:
            static void executeNode(GlowNodeBase const* request, Node* node, GlowRootElementCollection* response, Context& context);

            static void executeParameter(GlowParameterBase const* request, Parameter* parameter, GlowRootElementCollection* response, Context& context);

            static void executeCommand(GlowCommand const* command, Node* node, GlowRootElementCollection* response, Context& context);

            static void executeCommand(GlowCommand const* command, Parameter* parameter, GlowRootElementCollection* response, Context& context);

            static gadget::NodeField toNodeFieldFlags(libember::glow::DirFieldMask const& mask);

            static gadget::ParameterField toParameterFieldFlags(libember::glow::DirFieldMask const& mask);
    };
}

#endif//__TINYEMBER_GLOW_CONSUMERREQUESTPROCESSOR_H
