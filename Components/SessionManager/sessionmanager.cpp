#include "sessionmanager.h"
#include "sessionmanager.h"

#include <QTcpSocket>

SessionManager::SessionManager(QObject *parent) : QObject(parent)
  ,conn_cnt(0)
  ,server(new QTcpServer)
  ,client(new QTcpSocket)
  ,m_remoteAddress("")
  ,m_remotePort(0)
  ,_workState(UNLISTENED)
  ,_workType(SERVER)
{

    /** Server 模式 **/
    // 产生了新的客户端
    connect(server, &QTcpServer::newConnection, [&]() mutable{
        QTcpSocket *c = server->nextPendingConnection();
        emit newConnectSocket(c);
    });
    // 模式监听失败
    connect(server, &QTcpServer::acceptError, this, &SessionManager::onError);

    /** Client 模式 **/
    // 已连接到服务器
    connect(client, &QTcpSocket::connected, [&]{ this->_workState = CONNECTED; emit connected();});
    // 准备读取数据
    connect(client, &QTcpSocket::readyRead, [&]{
        if (client->bytesAvailable() > qint64(sizeof(qint8))) {
            emit newAction(-1, client);
        }
        emit readRead();
    });

    // 已断开连接
    connect(client, &QTcpSocket::disconnected, [&]{
        this->_workState = UNCONNECTED;
        emit disconnected();
    });

    // 连接状态发生改变
    connect(client, &QTcpSocket::stateChanged, this, &SessionManager::onStateChanged);
    // 发生错误
    connect(client, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::error), this, &SessionManager::onError);
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
    QTextStream(stdout) << QString("Session: 工作状态改变\n");
    this->_workType = type;
    switch (type) {
    case SessionManager::SERVER:
        _workState = UNLISTENED;
        server->close();
        if (server->listen(QHostAddress::Any, port)) {
            _workState = LISTENED;
            QTextStream(stdout) << QString("Session: 工作状态Listened\n");
        } else {
            _workState = UNLISTENED;
            QTextStream(stdout) << QString("Session: 工作状态UnListened\n");
            emit serverUnListenError();
        }
        break;
    case SessionManager::CLIENT:
        _workState = UNCONNECTED;
        client->close();
        client->connectToHost(host,port);
        this->m_remoteAddress = host;
        this->m_remotePort = port;
        break;
    }
}

void SessionManager::ConnectSocketSignal(QTcpSocket *c)
{
    clientMap.insert(QString::number(conn_cnt++), c);
    connect(c, &QTcpSocket::readyRead, this, &SessionManager::onReadyRead);
    connect(c, &QTcpSocket::disconnected, this, &SessionManager::onDisconnected);
//    connect(c, &QTcpSocket::disconnected, c, &QObject::deleteLater);
    emit clientCountChanged(clientMap.count());
}

void SessionManager::onStateChanged(QAbstractSocket::SocketState state)
{
//    QTextStream(stdout) << QString("Client: 连接状态改变\n");
    // UnconnectedState,
    // HostLookupState,
    // ConnectingState,
    // ConnectedState,
    // BoundState,
    // ListeningState,
    // ClosingState
    switch (state) {
    case QAbstractSocket::UnconnectedState: QTextStream(stdout) << QString("Client: UnconnectedState\n");
        emit ClientSocketUnConnected();
        break;
    case QAbstractSocket::HostLookupState: QTextStream(stdout) << QString("Client: HostLookupState\n");break;
    case QAbstractSocket::ConnectingState: QTextStream(stdout) << QString("Client: ConnectingState\n");
        emit ClientSocketConnecting();
        break;
    case QAbstractSocket::ConnectedState: QTextStream(stdout) << QString("Client: ConnectedState\n");
        emit ClientSocketConnected();
        break;
    case QAbstractSocket::BoundState: QTextStream(stdout) << QString("Client: BoundState\n");break;
    case QAbstractSocket::ListeningState: QTextStream(stdout) << QString("Client: ListeningState\n");break;
    case QAbstractSocket::ClosingState: QTextStream(stdout) << QString("Client: ClosingState\n");break;
    }

}

void SessionManager::onError(QAbstractSocket::SocketError error)
{
    switch (this->_workType) {
    case SERVER:
        break;
    case CLIENT:
        break;
    }
    // ConnectionRefusedError,                  /* 0 */
    // RemoteHostClosedError,
    // HostNotFoundError,
    // SocketAccessError,
    // SocketResourceError,
    // SocketTimeoutError,                     /* 5 */
    // DatagramTooLargeError,
    // NetworkError,
    // AddressInUseError,
    // SocketAddressNotAvailableError,
    // UnsupportedSocketOperationError,        /* 10 */
    // UnfinishedSocketOperationError,
    // ProxyAuthenticationRequiredError,
    // SslHandshakeFailedError,
    // ProxyConnectionRefusedError,
    // ProxyConnectionClosedError,             /* 15 */
    // ProxyConnectionTimeoutError,
    // ProxyNotFoundError,
    // ProxyProtocolError,
    // OperationError,
    // SslInternalError,                       /* 20 */
    // SslInvalidUserDataError,
    // TemporaryError,
}

void SessionManager::onReadyRead()
{
    foreach(QString key, clientMap.keys()) {
        QTcpSocket *localTake = clientMap[key];
        if (localTake->bytesAvailable() >= qint64(sizeof(qint8))) {
            emit newAction(-1, localTake);
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

/**
//void FileTransferManager::onSocketError(QAbstractSocket::SocketError error)
//{
//    switch (error) {
//    case QAbstractSocket::ConnectionRefusedError:
//        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_cSocket->errorString()));
//        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectionRefusedError";
//        break;
//    case QAbstractSocket::RemoteHostClosedError:
//        qDebug() << __FUNCTION__ << "QAbstractSocket::RemoteHostClosedError";
////        ui->plainTextEditLog->appendPlainText(QStringLiteral("文件传输终止！"));
////        reset();
//        break;
//    case QAbstractSocket::HostNotFoundError:
//        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_cSocket->errorString()));
//        qDebug() << __FUNCTION__ << "QAbstractSocket::HostNotFoundError";
//        break;
//    case QAbstractSocket::SocketTimeoutError:
//        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_cSocket->errorString()));
//        qDebug() << __FUNCTION__ << "QAbstractSocket::SocketTimeoutError";
//        break;
//    case QAbstractSocket::AddressInUseError:
//        qDebug() << __FUNCTION__ << "QAbstractSocket::AddressInUseError";
//        break;
//    default:
//        break;
//    }
//}
*/

