#include "infomationmanager.h"

#include <QTcpSocket>

InfomationManager::InfomationManager(QObject *parent) : QObject(parent)
{
    connect(&adapter, &SessionManager::newAction, this, &InfomationManager::onNewAction);
    connect(&adapter, &SessionManager::newConnectSocket, this, &InfomationManager::newConnectSocket);

    connect(&adapter, &SessionManager::connected, this, [=]{ emit connected(); });
    connect(&adapter, &SessionManager::readRead, this, [=]{ emit readRead(); });
    connect(&adapter, &SessionManager::disconnected, this, [=]{ emit disconnected(); });

    connect(&adapter, &SessionManager::clientCountChanged, this, [=]{ });
    connect(&adapter, &SessionManager::ClientSocketConnected, this, [=]{ emit ClientSocketConnected(); });
    connect(&adapter, &SessionManager::ClientSocketConnecting, this, [=]{ emit ClientSocketConnecting(); });
    connect(&adapter, &SessionManager::ClientSocketUnConnected, this, [=]{ emit ClientSocketUnConnected(); });

    connect(&adapter, &SessionManager::serverUnListenError, this, [=]{ });
}

void InfomationManager::setManagerTask(QString host, int port, SessionManager::SessionManagerWorkType type)
{
    adapter.SettingHost(host, port, type);
    this->_manager_work = type;
}

void InfomationManager::registerAction(int action)
{
    handle_t t;
    t.id = action;
    handles.append(t);
}

//void InfomationManager::broadCaseAction(QTcpSocket *c, InfomationManager::FullEvent e, QString msg)
//{
//    broadCaseAction(c, qint8(e), msg);
//}

void InfomationManager::broadCaseAction(QTcpSocket *c, qint8 action, QString msg)
{
    if (c == nullptr) {
        foreach(QString key, adapter.clients().keys()) {
            QTcpSocket *c = adapter.clients()[key];
            QDataStream stream(c);
            stream.setVersion(QDataStream::Qt_5_0);
            stream << qint8(action) << msg;
        }
    } else {
        QDataStream stream(c);
        stream.setVersion(QDataStream::Qt_5_0);
        stream << qint8(action) << msg;
    }
}

//void InfomationManager::broadCaseAction(InfomationManager::FullEvent e)
//{
//    broadCaseAction(qint8(e));
//}

void InfomationManager::broadCaseAction(qint8 e)
{
    if (adapter.c() == nullptr || adapter.c()->state() == QAbstractSocket::UnconnectedState) return;
    QDataStream stream(adapter.c());
    stream.setVersion(QDataStream::Qt_5_0);
    stream << e;
}

void InfomationManager::onNewAction(qint8 action, QTcpSocket *c)
{
    c->waitForReadyRead(10);
    QDataStream stream(c);
    stream.setVersion(QDataStream::Qt_5_0);

    QString msg;

    if(action == -1) stream >> action;

    if (this->_manager_work == SessionManager::SERVER)
        foreach (handle_t t, handles) {
            if (action == t.id) {
                emit onRemoteFetch(action, c);
            }
        }
    else
        foreach (handle_t t, handles) {
            if (action == t.id) {
                stream >> msg;
                emit onReply(action, msg);
            }
        }
}

void InfomationManager::newConnectSocket(QTcpSocket *c)
{
    qint8 action = -1;
    c->waitForReadyRead();
    if(c->bytesAvailable() >= (qint64)sizeof(qint8)) {
        QDataStream(c) >> action;
        onNewAction(action, c);
        adapter.ConnectSocketSignal(c);
    } else {
        adapter.ConnectSocketSignal(c);
    }
}
