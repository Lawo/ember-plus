#ifndef __TINYEMBER_NET_TCPCLIENT_H
#define __TINYEMBER_NET_TCPCLIENT_H

#include <QtNetwork\qtcpsocket.h>

namespace net
{
    class TcpServer;
    class TcpClient : public QObject
    {
        Q_OBJECT
        public:
            typedef unsigned char value_type;
            typedef value_type const* const_iterator;
            typedef std::size_t size_type;

            virtual ~TcpClient();

            template<typename InputIterator>
            void write(InputIterator first, InputIterator last);

            void write(QByteArray const& array);

        signals:
            void disconnected(TcpClient* client);

        protected:
            explicit TcpClient(QTcpSocket* socket);

            virtual void read(const_iterator first, const_iterator last, size_type size) = 0;

        private slots:
            void onDisconnect();

            void onReadyRead();

        private:
            enum { RxBufferSize = 4096, };

            value_type m_buffer[RxBufferSize];
            QTcpSocket* m_socket;
    };

    template<typename InputIterator>
    inline void TcpClient::write(InputIterator first, InputIterator last)
    {
        auto array = QByteArray();
        std::copy(first, last, std::back_inserter(array));
        write(array);
    }

    inline void TcpClient::write(QByteArray const& array)
    {
        auto socket = m_socket;
        if (socket != nullptr)
            socket->write(array);
    }
}

#endif//__TINYEMBER_NET_TCPCLIENT_H
