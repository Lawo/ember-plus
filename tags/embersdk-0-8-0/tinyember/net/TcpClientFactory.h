#ifndef TCPCLIENTFACTORY_H
#define TCPCLIENTFACTORY_H

#include <QtNetwork/QTcpSocket>

namespace net
{
    /** Forward declarations **/
    class TcpClient;
    class TcpServer;

    /**
     * Client factory which is used to create application defined tcp/ip clients when
     * a server accepts a new connection.
     */
    class TcpClientFactory
    {
    public:
        /** Destructor **/
        virtual ~TcpClientFactory();

        /**
         * Called when a new connection has been accepted.
         * @param socket The accepted socket.
         * @return Returns the created client instance.
         */
        virtual TcpClient* createClient(QTcpSocket* socket) = 0;
    };
}

#endif // TCPCLIENTFACTORY_H
