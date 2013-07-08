#ifndef __TINYEMBER_GLOW_CONSUMER_H
#define __TINYEMBER_GLOW_CONSUMER_H

#include <memory>
#include <ember/Ember.hpp>
#include <s101/StreamDecoder.hpp>
#include "../gadget/Subscriber.h"
#include "../net/TcpClient.h"

namespace glow
{
    /** Forward declaration */
    class ProviderInterface;

    /**
     * Represents a consumer that connected via tcp/ip socket.
     */
    class Consumer : public net::TcpClient
    {
        /**
         * Implementation of a simple subscriber.
         */
        class SubscriberImpl : public gadget::Subscriber
        {
            public:
                /**
                 * Initializes a new local subscriber.
                 * @param socket A reference to the client socket. This socket is used
                 *      to create a local name for the subscriber.
                 */
                explicit SubscriberImpl(QTcpSocket* socket);

                /**
                 * Returns the local name of the subscriber.
                 * @return The local name of the subscriber.
                 */
                virtual std::string name() const;

            private:
                std::string m_name;
        };

        /**
         * Implementation of an async DomReader which forwards decoded ember trees
         * to the consumer.
         */
        class DomReader : public libember::dom::AsyncDomReader
        {
            public:
                /**
                 * Initializes a new DomReader.
                 * @param consumer The consumer to notify when a new tree has been decoded.
                 */
                explicit DomReader(Consumer* consumer);

                /** Destructor */
                virtual ~DomReader();

            private:
                /**
                 * This method is invoked by the base class when a tree has been decoded.
                 * This override forwards the root node to the consumer.
                 * @param root The decoded root node.
                 */
                virtual void rootReady(libember::dom::Node* root);

            private:
                Consumer *const m_consumer;
        };

        typedef libs101::StreamDecoder<unsigned char> Decoder;
        public:
            /**
             * Initializes a new Consumer.
             * @param provider The provider which is used to notify consumer requests and subscriptions.
             * @param socket The accepted socket for this consumer.
             */
            Consumer(ProviderInterface* provider, QTcpSocket* socket);

        private:
            /** Destructor */
            virtual ~Consumer();

            /**
             * This method is called by the TcpClient when several bytes have been received. All bytes are
             * forwarded to the s101 decoder.
             * @param first Reference to the first byte that has been received.
             * @param last Points the the first element beyond the rx buffer.
             * @param size The number of bytes that have been received.
             */
            virtual void read(const_iterator first, const_iterator last, size_type size);

            /**
             * This method is called when a s101 message has been decoded.
             * @param first Reference to the first byte of the decoded s101 message.
             * @param last Points the the first element beyond the s101 message buffer.
             */
            void handleMessage(Decoder::const_iterator first, Decoder::const_iterator last);

            /**
             * This method is called by the DomReader when a tree has been decoded.
             * @param root The decoded tree.
             */
            void rootReady(libember::dom::Node* root);

            /**
             * Static callback for the s101 decoded.
             * @param first Reference to the first byte that has been received.
             * @param last Points the the first element beyond the rx buffer.
             * @param state A pointer to the consumer that received the bytes passed.
             */
            static void dispatch(Decoder::const_iterator first, Decoder::const_iterator last, Consumer* state);

        private:
            DomReader m_reader;
            ProviderInterface* m_provider;
            SubscriberImpl* m_subscriber;
            Decoder m_decoder;
    };
}

#endif//__TINYEMBER_GLOW_CONSUMER_H
