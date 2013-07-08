#include "Encoder.h"

namespace glow
{
    Encoder::Packet::Packet(Packet const& other)
        : m_encodedBytes(other.m_encodedBytes)
    {
    }

    Encoder::Packet::const_iterator Encoder::Packet::begin() const
    {
        return std::begin(m_encodedBytes);
    }

    Encoder::Packet::const_iterator Encoder::Packet::end() const
    {
        return std::end(m_encodedBytes);
    }

    Encoder::Packet::size_type Encoder::Packet::size() const
    {
        return m_encodedBytes.size();
    }


    Encoder::Stream::Stream(Encoder *const encoder)
        : libember::util::OctetStream(1024)
        , m_encoder(encoder)
    {}

    void Encoder::Stream::flush(iterator first, iterator last)
    {
        auto const isLastPacket = false;
        m_encoder->finishPacket(first, last, isLastPacket);
    }

    void Encoder::Stream::finish()
    {
        auto const isLastPacket = true;
        m_encoder->finishPacket(std::begin(*this), std::end(*this), isLastPacket);
    }


    Encoder Encoder::createEmberMessage(libember::glow::GlowContainer const* container)
    {
        return Encoder(container);
    }

    Encoder Encoder::createRequestKeepAliveMessage()
    {
        libs101::StreamEncoder<unsigned char> encoder;
        encoder.encode(0x00);                                           // Slot
        encoder.encode(libs101::MessageType::EmBER);                    // Message type
        encoder.encode(libs101::CommandType::KeepAliveRequest);         // Command
        encoder.encode(0x01);                                           // Version
        encoder.finish();
        return Encoder(encoder.begin(), encoder.end());
    }

    Encoder Encoder::createProviderStateMessage(bool state)
    {
        libs101::StreamEncoder<unsigned char> encoder;
        encoder.encode(0x00);                                           // Slot
        encoder.encode(libs101::MessageType::EmBER);                    // Message type
        encoder.encode(libs101::CommandType::ProviderState);            // Command
        encoder.encode(0x01);                                           // Version
        encoder.encode(state ? 0x01 : 0x00);
        encoder.finish();
        return Encoder(encoder.begin(), encoder.end());
    }

    Encoder::Encoder(libember::dom::Node const* node)
        : m_isFirstPacket(true)
    {
        auto stream = Stream(this);
        node->encode(stream);
        stream.finish();
    }

    Encoder::const_iterator Encoder::begin() const
    {
        return m_packets.begin();
    }

    Encoder::const_iterator Encoder::end() const
    {
        return m_packets.end();
    }

    Encoder::size_type Encoder::size() const
    {
        return m_packets.size();
    }
}
