#ifndef __TINYEMBERROUTER_GLOW_CONSUMER_H
#define __TINYEMBERROUTER_GLOW_CONSUMER_H

#include <ember/dom/AsyncDomReader.hpp>
#include <ember/glow/GlowContainer.hpp>
#include <s101/StreamDecoder.hpp>
#include "../net/TcpClient.h"

namespace glow
{
   class Dispatcher;

   class Consumer : public net::TcpClient
   {
      Q_OBJECT;

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
      explicit Consumer(QTcpSocket* socket, Dispatcher* dispatcher);

      /**
        * Encode the passed Glow tree and write the encoded EmBER
        * to the remote consumer.
        * @param glow The root of the Glow tree to encode.
        */
      void writeGlow(libember::glow::GlowContainer const* glow);

   private:
      /**
         * This method is called by the TcpClient when several bytes have been received. All bytes are
         * forwarded to the s101 decoder.
         * @param first Reference to the first byte that has been received.
         * @param last Points the the first element beyond the rx buffer.
         * @param size The number of bytes that have been received.
         */
      virtual void read(const_iterator first, const_iterator last, size_type size);

      /**
         * This method is called by the DomReader when a tree has been decoded.
         * @param root The decoded tree.
         */
      void rootReady(libember::dom::Node* root);

      /**
        * This method is called when a s101 message has been decoded.
        * @param first Reference to the first byte of the decoded s101 message.
        * @param last Points the the first element beyond the s101 message buffer.
        */
      void handleS101Message(Decoder::const_iterator first, Decoder::const_iterator last);

      /**
        * Static callback for the s101 decoder.
        * @param first Reference to the first byte that has been received.
        * @param last Points the the first element beyond the rx buffer.
        * @param state A pointer to the consumer that received the bytes passed.
        */
      static void onS101Message(Decoder::const_iterator first, Decoder::const_iterator last, Consumer* state);

   private:
      Dispatcher* m_dispatcher;
      DomReader m_reader;
      Decoder m_decoder;
   };
}

#endif//__TINYEMBERROUTER_GLOW_CONSUMER_H
