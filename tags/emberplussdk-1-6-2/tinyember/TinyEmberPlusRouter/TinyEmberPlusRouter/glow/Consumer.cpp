#include <iostream>
#include <QtCore>
#include <ember/glow/GlowNodeFactory.hpp>
#include <s101\CommandType.hpp>
#include <s101\Dtd.hpp>
#include <s101\PackageFlag.hpp>
#include <s101\StreamEncoder.hpp>
#include <s101\MessageType.hpp>
#include "Dispatcher.h"
#include "Encoder.h"
#include "Consumer.h"

namespace glow
{
   // ========================================================
   //
   // Consumer::DomReader Definitions
   //
   // ========================================================

   Consumer::DomReader::DomReader(Consumer* consumer)
      : libember::dom::AsyncDomReader(libember::glow::GlowNodeFactory::getFactory())
      , m_consumer(consumer)
   {}

   Consumer::DomReader::~DomReader()
   {
   }

   void Consumer::DomReader::rootReady(libember::dom::Node* root)
   {
      m_consumer->rootReady(root);
   }


   // ========================================================
   //
   // Consumer Definitions
   //
   // ========================================================

   Consumer::Consumer(QTcpSocket* socket, Dispatcher* dispatcher)
      : TcpClient(socket)
      , m_dispatcher(dispatcher)
      , m_reader(this)
   {}

   void Consumer::writeGlow(libember::glow::GlowContainer const* glow)
   {
      auto encoder = Encoder::createEmberMessage(glow);

      for each(auto packet in encoder)
         write(packet.begin(), packet.end());
   }

   void Consumer::read(const_iterator first, const_iterator last, size_type size)
   {
      std::cout << "received " << size << " bytes" << std::endl;
      m_decoder.read(first, last, Consumer::onS101Message, this);
   }

   void Consumer::handleS101Message(Decoder::const_iterator first, Decoder::const_iterator last)
   {
      first++;                                                   // Slot
      auto const message = *first++;                             // Message

      if(message == libs101::MessageType::EmBER)
      {
         auto const command = *first++;                          // Command
         first++;                                                // Version
         auto const flags = libs101::PackageFlag(*first++);      // Flags
         first++;                                                // DTD

         if(command == libs101::CommandType::EmBER)
         {
            auto appbytes = *first++;   // 1 Byte AppBytesCount
            while(appbytes-- > 0)
               first++;

            if(flags.value() & libs101::PackageFlag::FirstPackage)
               m_reader.reset();

            try
            {
               m_reader.read(first, last);
            }
            catch(std::runtime_error ex)
            {
               std::cerr << ex.what();
            }
         }
         else if(command == libs101::CommandType::KeepAliveRequest)
         {
            auto encoder = libs101::StreamEncoder<unsigned char>();
            encoder.encode(0x00);                                       // Slot
            encoder.encode(libs101::MessageType::EmBER);                // Message Type
            encoder.encode(libs101::CommandType::KeepAliveResponse);    // Command
            encoder.encode(0x01);                                       // Framing Version (1)
            encoder.finish();
            write(encoder.begin(), encoder.end());
         }
      }
   }

   void Consumer::rootReady(libember::dom::Node* root)
   {
      m_reader.detachRoot();

      auto glow = dynamic_cast<libember::glow::GlowContainer*>(root);

      if(glow != nullptr)
      {
         std::cout << "Received Glow" << std::endl;
         m_dispatcher->receiveGlow(glow, this);
      }
   }

   //static 
   void Consumer::onS101Message(Decoder::const_iterator first, Decoder::const_iterator last, Consumer* state)
   {
      state->handleS101Message(first, last);
   }
}
