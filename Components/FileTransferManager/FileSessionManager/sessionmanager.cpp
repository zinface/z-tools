#include "sessionmanager.h"
#include "sessionmanager.h"

#include <QTcpSocket>

SessionManager::SessionManager(QObject *parent) : QObject(parent)
  ,conn_cnt(0)
  ,server(new QTcpServer)
  ,client(new QTcpSocket)
  ,_workState(UNLISTENED)
{
    connect(server, &QTcpServer::newConnection, this, &SessionManager::onNewConnectSocket);
    connect(client, &QTcpSocket::connected, this, &SessionManager::onHostConnected);
    connect(client, &QTcpSocket::readyRead, this, &SessionManager::onHostReadyRead);
    connect(client, &QTcpSocket::disconnected, this, &SessionManager::onHostDisconnected);
}

SessionManager::~SessionManager()
{
    foreach(QString key, clientMap.keys()) {
        QTcpSocket *localTake = clientMap.take(key);
        localTake->close();
    }
}

void SessionManager::SettingHost(QString host, int port, SessionManager::SessionManagerWorkType type)
{
    switch (type) {
    case SessionManager::SERVER:
        _workState = UNLISTENED;
        server->close();
        if (server->listen(QHostAddress::Any, port)) {
            _workState = LISTENED;
        } else {
            _workState = UNLISTENED;
        }
        break;
    case SessionManager::CLIENT:
        _workState = UNCONNECTED;
        client->close();
        client->connectToHost(host,port);
        break;
    }
}

SessionManager::SessionManagerWorkState SessionManager::serverState()
{
    return _workState;
}

SessionManager::SessionManagerWorkState SessionManager::clientState()
{
    return _workState;
}

void SessionManager::onNewConnectSocket()
{
    QTcpSocket *c = server->nextPendingConnection();
    clientMap.insert(QString::number(conn_cnt++), c);
    connect(c, &QTcpSocket::readyRead, this, &SessionManager::onReadyRead);
    connect(c, &QTcpSocket::disconnected, this, &SessionManager::onDisconnected);
    connect(c, &QTcpSocket::disconnected, c, &QObject::deleteLater);
    emit clientCountChanged(clientMap.count());
    emit newConnectSocket(c);
}

void SessionManager::onReadyRead()
{
    foreach(QString key, clientMap.keys()) {
        QTcpSocket *localTake = clientMap[key];
        if (localTake->bytesAvailable() >= qint64(sizeof(qint8))) {
            emit newAction(localTake);
        }
    }
}

void SessionManager::onDisconnected()
{
    foreach (QString key, clientMap.keys()) {
        QTcpSocket *socket = clientMap[key];
        if (socket->state() == QTcpSocket::UnconnectedState) {
            clientMap.remove(key);
            socket->deleteLater();
        }
    }
    emit clientCountChanged(clientMap.count());
}

void SessionManager::onHostConnected()
{
    this->_workState = CONNECTED;
    emit connected();
}

void SessionManager::onHostReadyRead()
{
    if (client->bytesAvailable() > qint64(sizeof(qint8))) {
        emit newAction(client);
    }
    emit readRead();
}

void SessionManager::onHostDisconnected()
{
    this->_workState = UNCONNECTED;
    emit disconnected();
}

