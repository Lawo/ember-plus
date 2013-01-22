#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QtNetwork/QTcpSocket>

namespace net
{
    /** Forward declarations **/
    class TcpServer;

    /**
     * Base class of accpeted tcp/ip connections.
     * Implementations must implement onRead in order to process incoming bytes.
     */
    class TcpClient : public QObject
    {
        Q_OBJECT

    public:
        typedef unsigned char* iterator;
        typedef unsigned char const* const_iterator;
        typedef std::size_t size_type;

        /**
         * Destructor
         */
        virtual ~TcpClient();

        /**
         * Writes the provided bytes to the client.
         * @param first The begin of the buffer to transmit.
         * @param last The end of the buffer.
         */
        template<typename InputIterator>
        void write(InputIterator first, InputIterator last);

        /**
         * Writes the passed byte array to the client.
         * @param array Array to transmit.
         */
        void write(QByteArray const& array);

    signals:
        /**
         * This signal is emitted when the socket disconnects.
         */
        void disconnected(TcpClient* client);

    protected:
        /**
         * Constructor
         * @param socket The accepted socket.
         */
        explicit TcpClient(QTcpSocket* socket);

        /**
         * This method is called whenever the client receives new data.
         * @param first First item that has been received.
         * @param last End of the receive buffer.
         * @param size Number of byte that have been received.
         */
        virtual void onRead(const_iterator first, const_iterator last, size_type size) = 0;

    private:
        /**
         * Unregisters from the tcp server.
         */
        void unregister();

    private slots:
        /**
         * Called when the socket receives a disconnect event.
         * this method initializes destruction.
         */
        void onDisconnect();

        /**
         * This slot is called when new data is available. The data is the passed to the
         * virtual onRead method.
         */
        void onReadyRead();

    private:
        enum
        {
            RxBufferSize = 1024
        };

        unsigned char m_buffer[RxBufferSize];
        QTcpSocket* m_socket;
    };

    /**************************************************************************
     * Mandatory inline implementation                                        *
     **************************************************************************/

    template<typename InputIterator>
    inline void TcpClient::write(InputIterator first, InputIterator last)
    {
        QByteArray array;
        array.reserve(std::distance(first, last));

        std::copy(first, last, std::back_inserter(array));
        write(array);
    }

    inline void TcpClient::write(QByteArray const& array)
    {
        if (m_socket != 0)
            m_socket->write(array);
    }
}

#endif // TCPCLIENT_H
