#include "infomationmanager.h"

#include <QTcpSocket>

InfomationManager::InfomationManager(QObject *parent) : QObject(parent)
{
    connect(&adapter, &SessionManager::newAction, [&](qint8 action, QTcpSocket *c){ emit onNewAction(action, c); });
    connect(&adapter, &SessionManager::newConnectSocket, [&](QTcpSocket *c){ emit newConnectSocket(c); });

    connect(&adapter, &SessionManager::connected, [&]{ emit connected(); });
    connect(&adapter, &SessionManager::readRead, [&]{ emit readRead(); });
    connect(&adapter, &SessionManager::disconnected, [&]{ emit disconnected(); });

    connect(&adapter, &SessionManager::clientCountChanged, [&](int count){emit clientCountChanged(count); });
    connect(&adapter, &SessionManager::ClientSocketConnected, [&]{ emit ClientSocketConnected(); });
    connect(&adapter, &SessionManager::ClientSocketConnecting, [&]{ emit ClientSocketConnecting(); });
    connect(&adapter, &SessionManager::ClientSocketUnConnected, [&]{ emit ClientSocketUnConnected(); });

    connect(&adapter, &SessionManager::serverUnListenError, [&]{ emit ServerUnListenError(); });
}

void InfomationManager::setManagerMode(QString host, int port, SessionManager::SessionManagerWorkType type)
{
    adapter.SettingHost(host, port, type);
    this->_manager_work = type;
}

void InfomationManager::registerAction(int action, bool raw, bool filter)
{
    handle_t t;
    t.id = action;
    t.israw = raw;
    t.isfilter = filter;
    handles.append(t);
}

void InfomationManager::broadCaseAction(QTcpSocket *c, qint8 action, qint64 length, QByteArray &data) {
    if (c == nullptr) {
        foreach(QString key, adapter.clients().keys()) {
            QTcpSocket *c = adapter.clients()[key];
            QDataStream stream(c);
            stream.setVersion(QDataStream::Qt_5_0);
            stream << qint8(action) << qint64(length) << data;
            c->waitForBytesWritten();
            c->flush();
        }
    } else {
        QDataStream stream(c);
        stream.setVersion(QDataStream::Qt_5_0);
        stream << qint8(action) << qint64(length) << data;
        c->waitForBytesWritten();
        c->flush();
    }
}

void InfomationManager::broadCaseAction(QTcpSocket *c, qint8 action, QString msg)
{
    if (c == nullptr) {
        foreach(QString key, adapter.clients().keys()) {
            QTcpSocket *c = adapter.clients()[key];
            QDataStream stream(c);
            stream.setVersion(QDataStream::Qt_5_0);
            stream << qint8(action) << msg;
            c->waitForBytesWritten();
            c->flush();
        }
    } else {
        QDataStream stream(c);
        stream.setVersion(QDataStream::Qt_5_0);
        stream << qint8(action) << msg;
        c->waitForBytesWritten();
        c->flush();
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
    adapter.c()->waitForBytesWritten();
    adapter.c()->flush();
}

void InfomationManager::onNewAction(qint8 action, QTcpSocket *c)
{
    c->waitForReadyRead(10);
    QDataStream stream(c);
    stream.setVersion(QDataStream::Qt_5_0);

    QString msg;
    qint64 length;
    bool isfilter = false;

    if(action == -1) stream >> action;
    foreach (auto t, handles) {
        if (t.id == action && t.isfilter) {
            isfilter = true;
        }
    }

    if (this->_manager_work == SessionManager::SERVER)
        foreach (handle_t t, handles) {
            if (action == t.id) {
                if (t.israw) {
                    stream >> length;
                    while (c->bytesAvailable() < length) {
                        c->waitForReadyRead();
                    }
                    QByteArray data(length, 0);
                    stream >> data;
//                    QTextStream(stdout) << QString("Server:动作：%3, 头部字节长度:%1, 数据长度：%2\n").arg(length).arg(data.length()).arg(action);
                    emit onRemoteFetchRaw(action, length, data, c);
                }
                else
//                    QTextStream(stdout) << QString("Server:动作：%1\n").arg(action);
                    emit onRemoteFetch(action, c);
            }
        }
    else
        foreach (handle_t t, handles) {
            if (action == t.id) {
                if (t.israw) {
                    stream >> length;
                    while (c->bytesAvailable() < length) {
                        c->waitForReadyRead();
                    }
                    QByteArray data(length, 0);
                    stream >> data;
//                    QTextStream(stdout) << QString("Client:动作：%3, 头部字节长度:%1, 数据长度：%2\n").arg(length).arg(data.length()).arg(action);
                    emit onReplyRaw(action, length, data);
                } else {
                    stream >> msg;
//                    QTextStream(stdout) << QString("Client:动作：%1\n").arg(action);
                    emit onReply(action, msg);
                }
            }
        }
    if (isfilter) return;
    if (c->bytesAvailable() > sizeof(qint8)) onNewAction(-1, c);
}

void InfomationManager::newConnectSocket(QTcpSocket *c)
{
    qint8 action = -1;
    c->waitForReadyRead();
    if(c->bytesAvailable() >= (qint64)sizeof(qint8)) {
        QDataStream(c) >> action;
        foreach (auto t,handles) {
            if (t.id == action && t.isfilter) {
                onNewAction(action, c);
                return;
            }
        }
        onNewAction(action, c);
        adapter.ConnectSocketSignal(c);
    } else {
        adapter.ConnectSocketSignal(c);
    }
}
