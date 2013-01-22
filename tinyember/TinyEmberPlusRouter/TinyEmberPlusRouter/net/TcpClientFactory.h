#ifndef __TINYEMBERROUTER_NET_TCPCLIENTFACTORY_H
#define __TINYEMBERROUTER_NET_TCPCLIENTFACTORY_H

class QTcpSocket;

namespace net
{
    /** Forward declarations */
    class TcpClient;
    class TcpServer;

    /**
     * Interface which is used as factory for accept tcp clients.
     */
    class TcpClientFactory
    {
        public:
            /** Destructor */
            virtual ~TcpClientFactory()
            {}

            /**
             * The tcp server invokes this method when a new connection has been accepted.
             * @param socket The socket that represents the new connection.
             * @return A new instance of a class that inherits from the TcpClient class.
             */
            virtual TcpClient* create(QTcpSocket* socket) = 0;
    };
}

#endif//__TINYEMBERROUTER_NET_TCPCLIENTFACTORY_H
