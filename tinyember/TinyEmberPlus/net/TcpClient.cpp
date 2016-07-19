/*
    Copyright (C) 2012-2016 Lawo GmbH (http://www.lawo.com).
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include "TcpClient.h"

namespace net
{
    TcpClient::TcpClient(QTcpSocket* socket)
        : m_socket(socket)
    {
        m_socket->connect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
        m_socket->connect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    }

    TcpClient::~TcpClient()
    {
        m_socket->disconnect(m_socket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
        m_socket->disconnect(m_socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
        m_socket->close();
        m_socket = nullptr;
    }

    void TcpClient::onDisconnect()
    {
        emit disconnected(this);
    }

    void TcpClient::onReadyRead()
    {
        auto socket = m_socket;
        if (socket != nullptr)
        {
            while(socket->bytesAvailable() > 0)
            {
                auto const size = socket->read(reinterpret_cast<char*>(m_buffer), RxBufferSize);
                read(m_buffer, m_buffer + size, size);
            }
        }
    }
}
