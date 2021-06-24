#ifndef INFOMATIONMANAGER_H
#define INFOMATIONMANAGER_H

#include <QObject>
#include <sessionmanager.h>

class InfomationManager : public QObject
{
    Q_OBJECT
public:
    explicit InfomationManager(QObject *parent = nullptr);

    struct handle_data {
        int id;
        bool israw;
    };
    typedef struct handle_data handle_t;

public:
    void setManagerTask(QString host, int port, SessionManager::SessionManagerWorkType type);
    void registerAction(int action = -1, bool raw = false);

    void broadCaseAction(QTcpSocket *c, qint8 action, qint64 length, QByteArray &data);
//    void broadCaseAction(QTcpSocket *c, FullEvent e, QString msg);
    void broadCaseAction(QTcpSocket *c, qint8 action, QString msg);
//    void broadCaseAction(FullEvent e);
    void broadCaseAction(qint8 e);

private slots:
    void onNewAction(qint8 action, QTcpSocket *c);
    void newConnectSocket(QTcpSocket *c);

signals:
    /** Manager Signals **/
    void onRemoteFetchRaw(qint8 action, qint64 length, QByteArray &data, QTcpSocket *c);
    void onRemoteFetch(qint8 action, QTcpSocket *c);

    /** Manager Actions **/
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

private:
    SessionManager adapter;
    SessionManager::SessionManagerWorkType _manager_work;

private:
    QList<handle_t> handles;
};

#endif // INFOMATIONMANAGER_H
