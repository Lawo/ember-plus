#ifndef __TINYEMBER_GLOW_CONSUMERPROXY_H
#define __TINYEMBER_GLOW_CONSUMERPROXY_H

#include "../net/TcpClientFactory.h"
#include "../net/TcpServer.h"
#include "../gadget/Node.h"
#include "Consumer.h"
#include "Settings.h"

/** Forward declarations */
class QApplication;

namespace libember { namespace glow
{
    class GlowContainer;
    class GlowRootElementCollection;
}
}

namespace glow
{
    /** Forward declaration */
    class ProviderInterface;

    /**
     * The ConsumerProxy creates a tcp/ip listener which accepts consumers. Additionally, it provides a method
     * to send an ember tree to the connected consumers.
     */
    class ConsumerProxy : 
        public gadget::Node::DirtyStateListener,
        private net::TcpClientFactory
    {
        public:
            /**
             * Returns a reference to the static settings instance. The settings define the response behavior.
             * @return A reference to the settings instance.
             */
            static Settings& settings();

        public:
            /**
             * Initializes a new consumer proxy.
             * @param app Reference to the application object.
             * @param provider Reference to the provider interface.
             * @param port The tcp ip port to listen for connections.
             */
            ConsumerProxy(QApplication* app, ProviderInterface* provider, short port);

            /** Destructor */
            ~ConsumerProxy();

            /** Gets the port the TCP server is listening to. */
            int port() const;

            /**
             * Encodes the passed tree and sends it to all currently connected consumers.
             * @param container The tree to encode and transmit.
             */
            void write(libember::glow::GlowContainer const* container);

            /**
             * Sends a keep-alive request message to all connected clients.
             */
            void writeRequestKeepAlive();

            /**
             * Sends a provider state message to all connected clients.
             * @param state The current provider state.
             */
            void writeProviderState(bool state);

            /**
             * Closes the tcp/ip listener and initiates a disconnect of all consumers.
             */
            void close();

        private:
            /**
             * Creates a new consumer. This is a covariant overload of the TcpClientFactory::create method.
             * @param socket The accepted socket.
             * @return The new Consumer instance.
             */
            virtual Consumer* create(QTcpSocket* socket);
            
        private:
            /**
             * This method is invoked when the root node or one of its children have changed. It then write
             * the updated data to all consumers.
             * @param state The dirty state of the node that changed.
             * @param object The node that has changed its state.
             */
            virtual void notifyStateChanged(gadget::NodeFieldState const &state, gadget::Node const* object);

        private:
            /**
             * Transforms a node into a GlowNode. All children that are marked dirty will be appended as well.
             * @param parent The container to append the node to.
             * @param node The node to convert.
             */
            void transform(libember::glow::GlowContainer* parent, gadget::Node const* node) const;

            /**
             * Transforms a parameter into a GlowParameter.
             * @param parent The container to append the parameter to.
             * @param parameter The parameter to convert.
             */
            void transform(libember::glow::GlowContainer* parent, gadget::Parameter const* parameter) const;

            /**
             * Transforms a node into a GlowQualifiedNode.
             * @param root The root element collection to append the node to.
             * @param node The node to convert.
             */
            void transformQualified(libember::glow::GlowRootElementCollection* root, gadget::Node const* node);

            /**
             * Transforms a parameter into a GlowQualifiedParameter.
             * @param root The root element collection to append the parameter to.
             * @param parameter The parameter to convert.
             */
            void transformQualified(libember::glow::GlowRootElementCollection* root, gadget::Parameter const* parameter);

            /**
             * Tests whether a node needs to be transmitted or not. A node will not be transmitted when only the value of
             * parameter which is transmitted via a stream is marked dirty.
             * @param node The node to test.
             * @return true if the node needs to be transmitted via the default mechanism, false if only a parameter
             *      value is marked dirty which will be transmitted in a stream anyway.
             */
            bool isNotificationRequired(gadget::Node const* node) const;

        private:
            ProviderInterface *const m_provider;
            net::TcpServer* m_server;

            static Settings s_settings;
    };

    inline int ConsumerProxy::port() const
    {
        return m_server->port();
    }
}

#endif//__TINYEMBER_GLOW_CONSUMERPROXY_H

