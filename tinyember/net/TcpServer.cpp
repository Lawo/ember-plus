#include <algorithm>
#include <iostream>
#include <QThread>
#include "TcpClient.h"
#include "TcpClientFactory.h"
#include "TcpServer.h"

namespace net
{
    TcpServer::TcpServer(QObject* parent, net::TcpClientFactory* factory, qint16 port)
        : QTcpServer(parent)
        , m_factory(factory)
        , m_mutex(QMutex::NonRecursive)
    {
        connect(this, SIGNAL(newConnection()), this, SLOT(clientAccepted()));
        listen(QHostAddress::Any, port);
    }

    //virtual
    TcpServer::~TcpServer()
    {
        ClientList clients;
        {
            QMutexLocker const lock(&m_mutex);
            std::copy(m_clients.begin(), m_clients.end(), std::back_inserter(clients));

            m_clients.clear();
        }

        ClientList::iterator first = clients.begin();
        ClientList::iterator const last = clients.end();

        for(/** Nothing **/; first != last; ++first)
        {
            delete *first;
        }
    }

    void TcpServer::writeToClients(QByteArray const& array)
    {
        {
            QMutexLocker const lock(&m_mutex);
            ClientList::iterator first = m_clients.begin();
            ClientList::iterator const last = m_clients.end();

            for(; first != last; ++first)
            {
                (*first)->write(array);
            }
        }

        std::cout << "WriteToClients: " << array.size() << " Bytes" << std::endl;
    }

    void TcpServer::clientAccepted()
    {
        QTcpSocket *const socket = nextPendingConnection();

        if (socket != 0)
        {
            TcpClient *const client = m_factory->createClient(socket);
            connect(client, SIGNAL(disconnected(TcpClient*)), this, SLOT(clientDisconnected(TcpClient*)));

            {
                QMutexLocker lock(&m_mutex);

                m_clients.push_back(client);
            }
        }
    }

    void TcpServer::clientDisconnected(TcpClient* client)
    {
        std::cout << "Client Disconnected" << std::endl;

        disconnect(client, SIGNAL(disconnected(TcpClient*)), this, SLOT(clientDisconnected(TcpClient*)));

        {
            QMutexLocker const lock(&m_mutex);
            ClientList::iterator const first = m_clients.begin();
            ClientList::iterator const last = m_clients.end();

            m_clients.erase(std::find(first, last, client));
        }

        delete client;
    }
}
