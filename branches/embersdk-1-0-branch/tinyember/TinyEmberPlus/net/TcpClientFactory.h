#ifndef __TINYEMBER_NET_TCPCLIENTFACTORY_H
#define __TINYEMBER_NET_TCPCLIENTFACTORY_H

class QTcpSocket;

namespace net
{
    class TcpClient;
    class TcpServer;

    class TcpClientFactory
    {
        public:
            virtual ~TcpClientFactory()
            {}

            virtual TcpClient* create(QTcpSocket* socket) = 0;
    };
}

#endif//__TINYEMBER_NET_TCPCLIENTFACTORY_H
