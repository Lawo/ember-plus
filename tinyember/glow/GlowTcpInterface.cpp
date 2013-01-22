#include "GlowStateVisitor.h"
#include "GlowTcpClient.h"
#include "GlowTcpInterface.h"
#include "s101/Command.hpp"
#include "s101/Dtd.hpp"
#include "s101/StreamEncoder.hpp"
#include "../Libember.h"
#include <iostream>

using namespace ::libember::glow;
using namespace ::gadgets;

namespace glow
{
    GlowTcpInterface::GlowTcpInterface(QObject* parent, GlowClientMessageListener *eventListener)
        : m_eventListener(eventListener)
    {
        m_tcpServer = new net::TcpServer(parent, this, 9000);
    }

    //virtual
    GlowTcpInterface::~GlowTcpInterface()
    {
        delete m_tcpServer;
    }

    //virtual
    net::TcpClient* GlowTcpInterface::createClient(QTcpSocket* socket)
    {
        return new GlowTcpClient(m_eventListener, socket);
    }

    void GlowTcpInterface::write(libember::glow::GlowElementCollection const* collection)
    {
        libember::util::OctetStream output;
        collection->encode(output);

        libs101::StreamEncoder<unsigned char> s101Encoder;
        s101Encoder.encode(0x00);                           // Slot
        s101Encoder.encode(libs101::Command::EmBER);        // Ember Command
        s101Encoder.encode(0x01);                           // Version
        s101Encoder.encode(0x00);                           // Flags
        s101Encoder.encode(libs101::Dtd::Glow);             // Glow Dtd
        s101Encoder.encode(0x00);                           // App Bytes Low
        s101Encoder.encode(0x00);                           // App Bytes High
        s101Encoder.encode(output.begin(), output.end());   // Encoded DOM
        s101Encoder.finish();

        m_tcpServer->writeToClients(s101Encoder.begin(), s101Encoder.end());
    }

    //virtual
    void GlowTcpInterface::dirtyStateChanged(gadgets::Entity const& entity, gadgets::PropertyState const&)
    {
        std::cout << "Dirty State Changed: " << entity.identifier() << std::endl;

        GlowElementCollection *const collection = GlowElementCollection::createRoot();
        GlowStateVisitor visitor(collection);

        visitDirtyElements(entity, visitor);
        write(collection);

        delete collection;
    }

    void GlowTcpInterface::visitDirtyElements(gadgets::Entity const& entity, GlowStateVisitor& entityVisitor) const
    {
        entity.visit(entityVisitor, Property::None);

        Entity::const_iterator first = entity.begin();
        Entity::const_iterator const last = entity.end();

        for(/* nothing */; first != last; ++first)
        {
            Entity const& child = **first;

            if (child.isDirty())
                visitDirtyElements(child, *entityVisitor.createChildVisitor());
        }
    }
}
