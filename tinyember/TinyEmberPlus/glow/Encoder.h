#ifndef __TINYEMBER_GLOW_ENCODER_H
#define __TINYEMBER_GLOW_ENCODER_H

#include <vector>
#include <ember/Ember.hpp>
#include <s101/CommandType.hpp>
#include <s101/Dtd.hpp>
#include <s101/MessageType.hpp>
#include <s101/PackageFlag.hpp>
#include <s101/StreamEncoder.hpp>

namespace glow
{
    /**
     * The Encoder class is used to encode a glow tree and also generates the ready-to-use s101 
     * packets which contain the encoded tree.
     */
    class Encoder
    {
        public:
            /**
             * A packet contains a single s101 message. Depending on the length of the encoded ember tree
             * several packets need to be created.
             */
            class Packet
            {
                friend class Encoder;
                typedef std::vector<unsigned char> Container;
                public:
                    typedef Container::const_iterator const_iterator;
                    typedef Container::size_type size_type;

                    /**
                     * Copy constructor.
                     * @param other The packet to copy the data from.
                     */
                    Packet(Packet const& other);

                    /**
                     * Initializes a new Packet with the provided data.
                     * @param first A reference to the first element of the buffer to copy.
                     * @param last Points to the first item beyond the elements to copy.
                     */
                    template<typename InputIterator>
                    Packet(InputIterator first, InputIterator last);

                    /**
                     * Returns an iterator that points to the first element of this packet.
                     * @return An iterator that points to the first element of this packet.
                     */
                    const_iterator begin() const;

                    /**
                     * Returns an iterator that points one past the last element of this packet.
                     * @return An iterator that points one past the last element of this packet.
                     */
                    const_iterator end() const;

                    /**
                     * Returns the number of bytes this packet contains.
                     * @return The number of bytes this packet contains.
                     */
                    size_type size() const;

                private:
                    Container m_encodedBytes;
            };

            typedef std::vector<Packet> PacketCollection;
            typedef PacketCollection::const_iterator const_iterator;
            typedef PacketCollection::size_type size_type;

        public:
            /**
             * Encodes the passed container and wraps it into one or more s101 packages.
             * @param container The container to encode and wrap.
             * @return A new Encoder instance which contains a collection of s101 packets.
             */
            static Encoder createEmberMessage(libember::glow::GlowContainer const* container);

            /**
             * Creates a new provider state message.
             * @param state The current provider state.
             * @return A new Encoder instance which contains the encoded provider state message.
             */
            static Encoder createProviderStateMessage(bool state);

            /**
             * Creates a new keep-alive request.
             * @return A new Encoder instance which contains the encoded keep-alive request.
             */
            static Encoder createRequestKeepAliveMessage();

            /**
             * Returns an iterator that points to the first s101 packet.
             * @return An iterator that points to the first s101 packet.
             */
            const_iterator begin() const;

            /**
             * Returns an iterator that points one past the last valid s101 packet.
             * @return An iterator that points one past the last valid s101 packet.
             */
            const_iterator end() const;

            /**
             * Returns the total number of s101 packets.
             * @return The total number of s101 packets.
             */
            size_type size() const;

        private:
            /**
             * Initializes a new Encoder instance and generates the s101 packets from the
             * node passed.
             * @param node The node to encode.
             */
            explicit Encoder(libember::dom::Node const* node);

            /**
             * Initializes a new Encoder instance with the provided packets.
             * @param first An iterator that points to the first packet to copy.
             * @param last An iterator that points to the first packet not to copy.
             */
            template<typename InputIterator>
            Encoder(InputIterator first, InputIterator last);

            /**
             * Finishes the current packet. When the provided buffer is empty, an empty
             * packet will be generated.
             * @param first An iterator that points to the first byte of the buffer that contains a portion
             *      of the encoded ember tree.
             * @param last An iterator that points one past the last byte to copy.
             * @param isLastPacket If set to true, the last packet flag will be set in the current s101 message.
             */
            template<typename InputIterator>
            void finishPacket(InputIterator first, InputIterator last, bool isLastPacket);

        private:
            bool m_isFirstPacket;
            PacketCollection m_packets;

        private:
            /**
             * Helper class which forwards encoded ember data to the Encoder, which then 
             * generates a new s101 packet. This stream class is limited to 1024 bytes.
             */
            class Stream : public libember::util::OctetStream
            {
                public:
                    /**
                     * Initializes a new Stream instance.
                     * @param encoder The encoder that finalizes the s101 packets.
                     */
                    explicit Stream(Encoder *const encoder);

                    /**
                     * Finishes the pending data.
                     */
                    void finish();

                private:
                    /**
                     * This method called by the OctetStream when the capacity has been reached
                     * and the buffer resets itself.
                     * @param first An iterator that points to the first item in the current stream buffer.
                     * @param last An iterator that points one past the last item of the buffer to copy.
                     */
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
        auto const flags = (unsigned char)(
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
