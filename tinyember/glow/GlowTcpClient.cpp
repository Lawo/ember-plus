#include <algorithm>
#include "../Libember.h"
#include "s101/Command.hpp"
#include "s101/Dtd.hpp"
#include "GlowClientMessageListener.h"
#include "GlowTcpClient.h"

using namespace libember::dom;
using namespace libember::glow;
using namespace libember::util;

namespace glow
{
    //static
    void GlowTcpClient::decoderCallback(S101Decoder::const_iterator first, S101Decoder::const_iterator last, GlowTcpClient* client)
    {
        client->handleDecodedMessage(first, last);
    }

    GlowTcpClient::GlowTcpClient(GlowClientMessageListener* listener, QTcpSocket* socket)
        : net::TcpClient(socket)
        , m_listener(listener)
    {

    }

    //virtual
    GlowTcpClient::~GlowTcpClient()
    {

    }

    //virtual
    void GlowTcpClient::onRead(const_iterator first, const_iterator last, size_type)
    {
        m_decoder.read(first, last, GlowTcpClient::decoderCallback, this);
    }

    void GlowTcpClient::handleDecodedMessage(S101Decoder::const_iterator first, S101Decoder::const_iterator last)
    {
        first++;     // Slot
        std::size_t const command = *first++;     // Command

        if (command == libs101::Command::EmBER)
        {
            first++; // Version
            first++; // Flags
            first++; // DTD

            std::size_t appbytes = (*first++ | (*first++ << 8));

            while(appbytes-- > 0)
                first++;

            DomReader reader;
            OctetStream input;
            input.append(first, last);

            Node *const root = reader.decodeTree(input, GlowNodeFactory::getFactory());

            // Notify node
            if (m_listener && root)
                m_listener->notify(root);
        }
    }
}
