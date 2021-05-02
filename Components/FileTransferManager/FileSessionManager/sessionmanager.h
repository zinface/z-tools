#ifndef FILESESSIONADAPTER_H
#define FILESESSIONADAPTER_H

#include <QDataStream>
#include <QObject>
#include <QTcpServer>

class SessionManager : public QObject
{
    Q_OBJECT
public:
    SessionManager(QObject *parent = nullptr);
    ~SessionManager();

public:
    enum SessionManagerWorkType {
        SERVER,
        CLIENT,
    };

    enum SessionManagerWorkState {
        UNLISTENED,
        LISTENED,
        UNCONNECTED,
        CONNECTED,
    };

    void SettingHost(QString host, int port, SessionManagerWorkType type = SERVER);
    QMap<QString, QTcpSocket*> clients() { return  this->clientMap; }
    QTcpServer* s(){ return this->server;}
    QTcpSocket* c(){ return this->client; }

    // Server Public Function
    SessionManagerWorkState serverState();

    // Client Public Function
    SessionManagerWorkState clientState();

private slots:
    // Server Function
    void onNewConnectSocket();
    void onReadyRead();
    void onDisconnected();

    // Client Host Function
    void onHostConnected();
    void onHostReadyRead();
    void onHostDisconnected();

signals:
    /** Server Signals******/
    void clientCountChanged(int count);
    void newConnectSocket(QTcpSocket *c);
    void newAction(QTcpSocket *c);

    /** Client Signals******/
    void connected();
    void readRead();
    void disconnected();

private:
    int conn_cnt = 0;
    QMap<QString, QTcpSocket*> clientMap;

    QTcpServer *server;
    QTcpSocket *client;

    SessionManagerWorkState _workState;
};

#endif // FILESESSIONADAPTER_H
