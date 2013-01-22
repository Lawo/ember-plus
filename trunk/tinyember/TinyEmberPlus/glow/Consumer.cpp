#include <algorithm>
#include <iostream>
#include <ember\Ember.hpp>
#include <s101\CommandType.hpp>
#include <s101\Dtd.hpp>
#include <s101\PackageFlag.hpp>
#include <s101\StreamEncoder.hpp>
#include <s101\MessageType.hpp>
#include <qhostaddress.h>
#include "Consumer.h"
#include "ProviderInterface.h"

namespace glow
{
    Consumer::SubscriberImpl::SubscriberImpl(QTcpSocket* socket)
    {
        auto addr = socket->peerAddress().toString();
        auto port = socket->peerPort();

        m_name = (addr + QString(":") + QVariant(port).toString()).toStdString();
    }

    std::string Consumer::SubscriberImpl::name() const
    {
        return m_name;
    }



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



    Consumer::Consumer(ProviderInterface* provider, QTcpSocket* socket)
        : ::net::TcpClient(socket)
#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4355)
#endif
        , m_reader(this)
#ifdef _MSC_VER
#  pragma warning(pop)
#endif
        , m_provider(provider)
        , m_subscriber(new SubscriberImpl(socket))
    {
        if (provider != nullptr)
            provider->registerSubscriberAsync(m_subscriber);
    }

    Consumer::~Consumer()
    {
        auto provider = m_provider;
        if (provider != nullptr)
            provider->unregisterSubscriberAsync(m_subscriber);

        m_subscriber->releaseRef();
    }

    void Consumer::read(const_iterator first, const_iterator last, size_type size)
    {
        m_decoder.read(first, last, Consumer::dispatch, this);
    }

    void Consumer::handleMessage(Decoder::const_iterator first, Decoder::const_iterator last)
    {
        first++;                                                    // Slot
        auto const message = *first++;                              // Message

        if (message == libs101::MessageType::EmBER)
        {
            auto const command = *first++;                          // Command
            first++;                                                // Version
            auto const flags = libs101::PackageFlag(*first++);      // Flags
            first++;                                                // DTD

            if (command == libs101::CommandType::EmBER)
            {
                auto appbytes = *first++;   // 1 AppByte
                while(appbytes-- > 0)
                    ++first;

                try
                {
                    m_reader.read(first, last);
                }
                catch(std::runtime_error ex)
                {
                    std::cerr << ex.what();
                }

                if (flags.value() & libs101::PackageFlag::LastPackage)
                {
                    m_reader.reset();
                }
            }
            else if (command == libs101::CommandType::KeepAliveRequest)
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

        auto provider = m_provider;
        if (provider != nullptr && root != nullptr)
            provider->notifyAsync(root, m_subscriber);
    }

    //static 
    void Consumer::dispatch(Decoder::const_iterator first, Decoder::const_iterator last, Consumer* state)
    {
        state->handleMessage(first, last);
    }
}
