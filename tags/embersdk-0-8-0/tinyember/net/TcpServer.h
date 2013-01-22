#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <algorithm>
#include <list>
#include <QtNetwork/QTcpServer>
#include <QMutex>
#include <QThread>

namespace net
{
    /** Forward declarations **/
    class TcpClient;
    class TcpClientFactory;

    /**
     * Tcp/Ip Server which listens for incoming connections at a specified port.
     * This implementation uses the client factory to handle accepted sockets and
     * provides the possibility to send a message to all connected clients.
     */
    class TcpServer : public QTcpServer
    {
        friend class TcpClient;

        Q_OBJECT

    public:
        /**
         * Constructor
         * @param parent Object owning this server, usually an instance of QApplication.
         * @param factory Client factory.
         * @param port The port to listen to.
         */
        TcpServer(QObject* parent, TcpClientFactory* factory, qint16 port);

        /** Destructor **/
        virtual ~TcpServer();

        /**
         * Sends the passed array to all connected clients.
         * @param array The array to transmit.
         */
        void writeToClients(QByteArray const& array);

        /**
         * Sends the provided data to all connected clients.
         * @param first Start of the transmission buffer.
         * @param last End of the buffer.
         */
        template<typename InputIterator>
        void writeToClients(InputIterator first, InputIterator last);

    private slots:
        /**
         * This method is called when a new connection has been accepted.
         */
        void clientAccepted();

        /**
         * Called when a client emits a disconnected signal. This method is
         * used to unregister the clients
         * @param client The disconnecting client.
         */
        void clientDisconnected(TcpClient* client);

    private:
        typedef std::list<TcpClient*> ClientList;

        TcpClientFactory* m_factory;
        ClientList m_clients;
        QMutex m_mutex;
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename InputIterator>
    inline void TcpServer::writeToClients(InputIterator first, InputIterator last)
    {
        QByteArray array;
        array.reserve(std::distance(first, last));

        std::copy(first, last, std::back_inserter(array));

        writeToClients(array);
    }
}

#endif // TCPLISTENER_H
