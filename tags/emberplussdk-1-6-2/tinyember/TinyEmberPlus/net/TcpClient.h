#ifndef __TINYEMBER_NET_TCPCLIENT_H
#define __TINYEMBER_NET_TCPCLIENT_H

#include <QtNetwork\qtcpsocket.h>

namespace net
{
    /** Forward declaration */
    class TcpServer;

    /**
     * Base class for a tcp/ip client.
     */
    class TcpClient : public QObject
    {
        Q_OBJECT
        public:
            typedef unsigned char value_type;
            typedef value_type const* const_iterator;
            typedef std::size_t size_type;

            /** Destructor */
            virtual ~TcpClient();

            /**
             * Sends data to the connected client.
             * @param first An iterator to the first element to write.
             * @param last An iterator to the first element not to write.
             */
            template<typename InputIterator>
            void write(InputIterator first, InputIterator last);

            /**
             * Sends the passed byte array to the connected client.
             * @param array The array to transmit.
             */
            void write(QByteArray const& array);

        signals:
            /**
             * This signal is emitted when the socket disconnects.
             * @param client Pointer to the client that signals this disconnect event.
             */
            void disconnected(TcpClient* client);

        protected:
            /**
             * Initializes a new TcpClient.
             * @param socket The new socket to use by this client.
             */
            explicit TcpClient(QTcpSocket* socket);

            /**
             * Handles bytes received from the connected client.
             * @param first Points to the first element in the rx buffer.
             * @param last Points one past the last element of the current rx buffer.
             * @param size The number of bytes that have been received.
             */
            virtual void read(const_iterator first, const_iterator last, size_type size) = 0;

        private slots:
            /**
             * Handles a socket disconnect event.
             */
            void onDisconnect();

            /**
             * Fetches the data that has been sent by the connected clients and forwards it to the abstract
             * read method.
             */
            void onReadyRead();

        private:
            enum { RxBufferSize = 4096, };

            value_type m_buffer[RxBufferSize];
            QTcpSocket* m_socket;
    };

    /**************************************************************************
     * Inline implementation                                                  *
     **************************************************************************/

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
