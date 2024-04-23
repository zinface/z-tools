#ifndef INFOMATIONMANAGER_H
#define INFOMATIONMANAGER_H

#include <QObject>
#include <sessionmanager.h>
#include <package.h>

class InfomationManager : public QObject
{
    Q_OBJECT
public:
    explicit InfomationManager(QObject *parent = nullptr);

    struct handle_data {
        int id;
        bool israw;
        bool isfilter;
    };
    typedef struct handle_data handle_t;

public:
    void setManagerMode(QString host, int port, SessionManager::SessionManagerWorkType type);
//    QMap<QString, QTcpSocket*> clients() { return  this->adaclientMap; }

    // 需要注册的事件信息：事件动作，数据模式，生命周期接管过滤
    void registerAction(int action = -1, bool raw = false, bool filter = false);

    void broadCaseAction(QTcpSocket *c, qint8 action, qint64 length, QByteArray &data);
//    void broadCaseAction(QTcpSocket *c, FullEvent e, QString msg);
    void broadCaseAction(QTcpSocket *c, qint8 action, QString msg);
    void broadCaseAction(qint8 e);

    //
    QTcpServer* s(){ return this->adapter.server; }
    QTcpSocket* c(){ return this->adapter.client; }

    QString ra(){ return this->adapter.m_remoteAddress; };
    int rp(){ return this->adapter.m_remotePort; };

    bool status() { return this->adapter.status(); }
    bool state() { return this->adapter.workState(); }

    int clientCount() { return this->adapter.clientMap.count(); }

private slots:
    void onNewAction(qint8 action, QTcpSocket *c);
    void newConnectSocket(QTcpSocket *c);

signals:
    /** Manager Signals Actions **/
    // Subscriber
//    void onActionRaw()
    void onRemoteFetchRaw(qint8 action, qint64 length, QByteArray &data, QTcpSocket *c);
    void onRemoteFetch(qint8 action, QTcpSocket *c);

    // Publisher
    void onReplyRaw(qint8 action, qint64 length, QByteArray &data);
    void onReply(qint8 action, QString msg);

    /** Session Signals **/
    void ClientSocketConnected();
    void ClientSocketConnecting();
    void ClientSocketUnConnected();
    void ServerUnListenError();

    void connected();
    void readRead();
    void disconnected();

    void clientCountChanged(int count);

private:
    friend class SessionManager;
    SessionManager adapter;
    SessionManager::SessionManagerWorkType _manager_work;

private:
    QList<handle_t> handles;
};

#endif // INFOMATIONMANAGER_H
