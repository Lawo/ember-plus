#ifndef __TINYEMBER_GLOW_CONSUMER_H
#define __TINYEMBER_GLOW_CONSUMER_H

#include <memory>
#include <ember/Ember.hpp>
#include <s101/StreamDecoder.hpp>
#include "../gadget/Subscriber.h"
#include "../net/TcpClient.h"

namespace glow
{
    class ProviderInterface;

    class Consumer : public net::TcpClient
    {
        class SubscriberImpl : public gadget::Subscriber
        {
            public:
                explicit SubscriberImpl(QTcpSocket* socket);

                virtual std::string name() const;

            private:
                std::string m_name;
        };

        class DomReader : public libember::dom::AsyncDomReader
        {
            public:
                explicit DomReader(Consumer* consumer);

                virtual ~DomReader();

            private:
                virtual void rootReady(libember::dom::Node* root);

            private:
                Consumer *const m_consumer;
        };

        friend class DromReader;

        typedef libs101::StreamDecoder<unsigned char> Decoder;
        public:
            Consumer(ProviderInterface* provider, QTcpSocket* socket);

        private:
            virtual ~Consumer();

            virtual void read(const_iterator first, const_iterator last, size_type size);

            void handleMessage(Decoder::const_iterator first, Decoder::const_iterator last);

            void rootReady(libember::dom::Node* root);

            static void dispatch(Decoder::const_iterator first, Decoder::const_iterator last, Consumer* state);

        private:
            DomReader m_reader;
            ProviderInterface* m_provider;
            SubscriberImpl* m_subscriber;
            Decoder m_decoder;
    };
}

#endif//__TINYEMBER_GLOW_CONSUMER_H
