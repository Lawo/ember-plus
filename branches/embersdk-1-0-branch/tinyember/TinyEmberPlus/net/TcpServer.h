#ifndef __TINYEMBER_NET_TCPSERVER_H
#define __TINYEMBER_NET_TCPSERVER_H

#include <vector>
#include <QtNetwork/qtcpserver.h>
#include <qmutex.h>
#include <qthread.h>

namespace net
{
    class TcpClient;
    class TcpClientFactory;

    class TcpServer : public QTcpServer
    {
        Q_OBJECT;
        public:
            TcpServer(QApplication* app, TcpClientFactory* factory, short port);

            virtual ~TcpServer();

            void write(QByteArray const& array);

            template<typename InputIterator>
            void write(InputIterator first, InputIterator last);

        private slots:
            void clientAccepted();

            void clientDisconnected(TcpClient* client);

        private:
            typedef std::vector<TcpClient*> ClientCollection;
            ClientCollection m_clients;
            TcpClientFactory *const m_factory;
            QMutex m_mutex;
    };

    template<typename InputIterator>
    inline void TcpServer::write(InputIterator first, InputIterator last)
    {
        auto array = QByteArray();
        std::copy(first, last, std::back_inserter(array));
        write(array);
    }
}

#endif//__TINYEMBER_NET_TCPSERVER_H
