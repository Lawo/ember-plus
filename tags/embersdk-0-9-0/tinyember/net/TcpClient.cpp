#include <iostream>
#include "TcpClient.h"
#include "TcpServer.h"

namespace net
{
    TcpClient::TcpClient(QTcpSocket* socket)
        : m_socket(socket)
    {
        m_socket->connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
        m_socket->connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    }

    //virtual
    TcpClient::~TcpClient()
    {
        m_socket->disconnect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
        m_socket->disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        m_socket->close();
        m_socket = 0;

        std::cout << "Client closed" << std::endl;
    }

    void TcpClient::onDisconnect()
    {
        emit disconnected(this);
    }

    void TcpClient::onReadyRead()
    {
        while(m_socket->bytesAvailable() > 0)
        {
            size_type const size = m_socket->read((char*)m_buffer, RxBufferSize);
            onRead(m_buffer, m_buffer + size, size);
        }
    }
}
