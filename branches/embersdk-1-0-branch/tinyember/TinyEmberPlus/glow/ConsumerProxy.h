#ifndef __TINYEMBER_GLOW_CONSUMERPROXY_H
#define __TINYEMBER_GLOW_CONSUMERPROXY_H

#include "../net/TcpClientFactory.h"
#include "../net/TcpServer.h"
#include "../gadget/Node.h"
#include "Settings.h"

class QApplication;

namespace libember { namespace glow
{
    class GlowContainer;
    class GlowRootElementCollection;
}
}

namespace glow
{
    class ProviderInterface;

    class ConsumerProxy : 
        public gadget::Node::DirtyStateListener,
        private net::TcpClientFactory
    {
        public:
            static Settings& settings();

        public:
            ConsumerProxy(QApplication* app, ProviderInterface* provider);

            ~ConsumerProxy();

            void write(libember::glow::GlowContainer const* container);

            void writeRequestKeepAlive();

            void close();

        private:
            virtual net::TcpClient* create(QTcpSocket* socket);
            
            virtual void notifyStateChanged(gadget::NodeFieldState const &state, gadget::Node const* object);

            void transform(libember::glow::GlowContainer* parent, gadget::Node const* node) const;

            void transform(libember::glow::GlowContainer* parent, gadget::Parameter const* parameter) const;

            void transformQualified(libember::glow::GlowRootElementCollection* root, gadget::Node const* node);

            void transformQualified(libember::glow::GlowRootElementCollection* root, gadget::Parameter const* parameter);

            bool isNotificationRequired(gadget::Node const* node) const;

        private:
            ProviderInterface *const m_provider;
            net::TcpServer* m_server;

            static Settings s_settings;
    };
}

#endif//__TINYEMBER_GLOW_CONSUMERPROXY_H

