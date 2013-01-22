#ifndef __TINYEMBER_GLOW_ENCODER_H
#define __TINYEMBER_GLOW_ENCODER_H

#include <vector>
#include <ember/Ember.hpp>
#include <s101\CommandType.hpp>
#include <s101\Dtd.hpp>
#include <s101\MessageType.hpp>
#include <s101\PackageFlag.hpp>
#include <s101\StreamEncoder.hpp>

namespace glow
{
    class Encoder
    {
        public:
            class Packet
            {
                friend class Encoder;
                typedef std::vector<unsigned char> Container;
                public:
                    typedef Container::const_iterator const_iterator;
                    typedef Container::size_type size_type;

                    Packet(Packet const& other);

                    template<typename InputIterator>
                    Packet(InputIterator first, InputIterator last);

                    const_iterator begin() const;

                    const_iterator end() const;

                    size_type size() const;

                private:
                    Container m_encodedBytes;
            };

            typedef std::vector<Packet> PacketCollection;
            typedef PacketCollection::const_iterator const_iterator;
            typedef PacketCollection::size_type size_type;

        public:
            static Encoder createEmberMessage(libember::glow::GlowContainer const* container);

            static Encoder createRequestKeepAliveMessage();

            const_iterator begin() const;

            const_iterator end() const;

            size_type size() const;

        private:
            explicit Encoder(libember::dom::Node const* node);

            template<typename InputIterator>
            Encoder(InputIterator first, InputIterator last);

            template<typename InputIterator>
            void finishPacket(InputIterator first, InputIterator last, bool isLastPacket);

        private:
            bool m_isFirstPacket;
            PacketCollection m_packets;

        private:
            class Stream : public libember::util::OctetStream
            {
                public:
                    explicit Stream(Encoder *const encoder);

                    void finish();

                private:
                    virtual void flush(iterator first, iterator last);

                private:
                    Encoder *const m_encoder;
            };

            friend class Stream;
    };


    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/


    template<typename InputIterator>
    inline Encoder::Packet::Packet(InputIterator first, InputIterator last)
        : m_encodedBytes(first, last)
    {}

    template<typename InputIterator>
    inline Encoder::Encoder(InputIterator first, InputIterator last)
        : m_isFirstPacket(true)
    {
        m_packets.push_back(Packet(first, last));
    }

    template<typename InputIterator>
    inline void Encoder::finishPacket(InputIterator first, InputIterator last, bool isLastPacket)
    {
        auto encoder = libs101::StreamEncoder<unsigned char>();
        auto const version = libember::glow::GlowDtd::version();
        auto const isEmpty = first == last;
        auto const flags = unsigned char(
                (m_isFirstPacket ? libs101::PackageFlag::FirstPackage : 0) |
                (isLastPacket ? libs101::PackageFlag::LastPackage : 0) |
                (isEmpty ? libs101::PackageFlag::EmptyPackage : 0)
            );

        encoder.encode(0x00);                       // Slot
        encoder.encode(libs101::MessageType::EmBER);// Message type
        encoder.encode(libs101::CommandType::EmBER);// Ember Command
        encoder.encode(0x01);                       // Version
        encoder.encode(flags);                      // Flags
        encoder.encode(libs101::Dtd::Glow);         // Glow Dtd
        encoder.encode(0x02);                       // App bytes low
        encoder.encode((version >> 0) & 0xFF);      // App specific, minor revision
        encoder.encode((version >> 8) & 0xFF);      // App specific, major revision
        encoder.encode(first, last);
        encoder.finish();

        m_isFirstPacket = false;
        m_packets.push_back(Packet(encoder.begin(), encoder.end()));
    }
}

#endif//__TINYEMBER_GLOW_ENCODER_H
