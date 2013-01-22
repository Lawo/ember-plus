#ifndef GLOWTCPINTERFACE_H
#define GLOWTCPINTERFACE_H

#include "../gadgets/EntityStateListener.h"
#include "../net/TcpClientFactory.h"
#include "../net/TcpServer.h"
#include "GlowStreamWriter.h"


namespace libember { namespace glow {
    /** Forward declaration **/
    class GlowElementCollection;
}
}

namespace glow
{
    /** Forward declaration **/
    class GlowClientMessageListener;
    class GlowStateVisitor;

    /**
     * The Glow Tcp Interface is used to encode and decode glow messages. It accepts incoming
     * connections and sends glow message to all currently connected clients.
     */
    class GlowTcpInterface :
            public GlowStreamWriter,
            public gadgets::EntityStateListener,
            private net::TcpClientFactory
    {
    public:
        /**
         * Constructor, initializes the interface and creates a tcp listener at port 9000.
         * @param parent Pointer to the parent object, should be QApplication.
         * @param eventListener Message listener whos notify method will be called when a new
         *      message has been decoded.
         */
        explicit GlowTcpInterface(QObject* parent, GlowClientMessageListener* eventListener);

        /** Destructor **/
        virtual ~GlowTcpInterface();

        /**
         * @note See GlowStreamWriter
         */
        virtual void write(libember::glow::GlowElementCollection const* collection);

    private:
        /**
         * Creates a new Glow TCP/IP Client.
         * @see TcpClientFactory
         * @param socket The accepted socket.
         * @return Returns the new client object.
         */
        virtual net::TcpClient* createClient(QTcpSocket* socket);

        /**
         * This method is called when a property of an entity has been changed via the TinyEmber UI. When
         * a change is being notified, all dirty entities will be packed into a glow structure and
         * then transmitted to the clients.
         * @param entity The entity that has changed, usually the root node.
         * @param state The flags that have changed.
         */
        virtual void dirtyStateChanged(gadgets::Entity const& entity, gadgets::PropertyState const& state);

        /**
         * Recursively collects all dirty entities.
         * @param entity Dirty entity.
         * @param entityVisitor The visitor collecting the updated properties of the current entity.
         */
        void visitDirtyElements(gadgets::Entity const& entity, GlowStateVisitor& entityVisitor) const;

    private:
        GlowClientMessageListener *const m_eventListener;
        net::TcpServer* m_tcpServer;
    };
}

#endif // GLOWTCPINTERFACE_H
