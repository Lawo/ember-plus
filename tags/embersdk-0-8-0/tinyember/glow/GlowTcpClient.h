#ifndef GLOWTCPCLIENT_H
#define GLOWTCPCLIENT_H

#include "../net/TcpClient.h"
#include "s101/StreamDecoder.hpp"

namespace glow
{
    /** Forward declarations **/
    class GlowClientMessageListener;

    /**
     * Implements the S101 glow decoder
     */
    class GlowTcpClient : public net::TcpClient
    {
    public:
        typedef libs101::StreamDecoder<unsigned char> S101Decoder;

        /**
         * Constructor, initalizes a new client stream.
         * @param listener Global message listener, which forwards and process incoming s101 requests.
         * @param socket Accepted socket
         */
        GlowTcpClient(GlowClientMessageListener* listener, QTcpSocket* socket);

    private:
        /** Destructor **/
        virtual ~GlowTcpClient();

        /**
         * Forwards incoming bytes to the s101 decoder.
         * @param first Buffer start.
         * @param last Buffer end.
         * @param size Number of bytes received.
         */
        virtual void onRead(const_iterator first, const_iterator last, size_type size);

        /**
         * Processes a completed S101 message.
         * @param first Begin of the message.
         * @param last End of the message.
         */
        void handleDecodedMessage(S101Decoder::const_iterator first, S101Decoder::const_iterator last);

        /**
         * Static callback which is passed to the decoder to process s101 messages. This method forwards the data to the
         * handleDecodedMessage method.
         * @param first Begin of the message.
         * @param last End of the message.
         * @param client The client to forward the data to.
         */
        static void decoderCallback(S101Decoder::const_iterator first, S101Decoder::const_iterator last, GlowTcpClient* client);

    private:
        GlowClientMessageListener *const m_listener;
        S101Decoder m_decoder;
    };
}

#endif // GLOWTCPCLIENT_H
