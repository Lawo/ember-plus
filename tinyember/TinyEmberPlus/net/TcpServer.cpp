#include <algorithm>
#include <qapplication.h>
#include "TcpClient.h"
#include "TcpClientFactory.h"
#include "TcpServer.h"

namespace net
{
    TcpServer::TcpServer(QApplication* app, TcpClientFactory* factory, short port)
        : QTcpServer(app)
        , m_factory(factory)
        , m_mutex(QMutex::Recursive)
        , m_port(port)
    {
        connect(this, SIGNAL(newConnection()), this, SLOT(clientAccepted()));
        listen(QHostAddress::Any, port);
    }

    TcpServer::~TcpServer()
    {
        auto clients = ClientCollection();
        {
            QMutexLocker const lock(&m_mutex);
            std::copy(m_clients.begin(), m_clients.end(), std::back_inserter(clients));
            m_clients.clear();
        }

        auto const last = clients.end();
        for (auto first = clients.begin(); first != last; ++first)
        {
            delete *first;
        }
    }

    void TcpServer::write(QByteArray const& array)
    {
        QMutexLocker const lock(&m_mutex);
        for(auto client : m_clients)
        {
            client->write(array);
        }
    }
     
    void TcpServer::clientAccepted()
    {
        auto const socket = nextPendingConnection();
        if (socket != nullptr)
        {
            auto const client = m_factory->create(socket);
            connect(client, SIGNAL(disconnected(TcpClient*)), this, SLOT(clientDisconnected(TcpClient*)));
            {
                QMutexLocker const lock(&m_mutex);
                m_clients.push_back(client);
            }
        }
    }

    void TcpServer::clientDisconnected(TcpClient* client)
    {
        disconnect(client, SIGNAL(disconnected(TcpClient*)), this, SLOT(clientDisconnected(TcpClient*)));
        {
            QMutexLocker const lock(&m_mutex);
            auto const first = m_clients.begin();
            auto const last = m_clients.end();
            auto const result = std::find(first, last, client);
            if (result != last)
            {
                m_clients.erase(result);
            }

            delete client;
        }
    }
}
