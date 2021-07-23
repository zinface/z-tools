#ifndef INFOMATIONMANAGER_H
#define INFOMATIONMANAGER_H

#include <QObject>
#include <sessionmanager.h>

class Package{
public:
    Package() {
        buffer.resize(0);
    }
    Package(QByteArray &data) {
        buffer.append(data);
    }
    ~Package() {
        buffer.clear();
    };

    static QByteArray lenf(int length, int size){
        QByteArray buf;
        buf.append(QByteArray::number(length));
        buf.resize(size);
        return buf;
    };

    void setByteArrayData(QByteArray &data) {
        buffer.append(data);
    }

    QByteArray &toByteArray() {
        return buffer;
    }

    Package &operator<<(QString value) {
        buffer.append(lenf(value.toLocal8Bit().length(), 8));
        buffer.append(value.toLocal8Bit());return *this;
    }

    Package &operator<<(qint64 number) {
        buffer.append(lenf(number, 8));
        return *this;
    }
    Package &operator<<(qint8 number) {
        buffer.append(lenf(number, 8)); return *this;
    }

    Package &operator>>(QString &value) {
//        value =
        int length = buffer.length();
        int vlen = buffer.left(8).toInt();
        buffer = buffer.right(length-8);
        value = QString(buffer.left(vlen));
        buffer = buffer.right(length-vlen-8);
        return *this;
    }

//    friend QString &operator<<(QString &value, Package package) {
//        int length = buffer.length();
//        int vlen = buffer.left(8).toInt();

//        return value;
//    }

    Package &operator>>(qint64 &number) {
        int length = buffer.length();
        number = buffer.left(8).toLongLong();
        buffer = buffer.right(length-8);

        return *this;
    }

    Package &operator>>(qint8 &number) {
        int length = buffer.length();
        number = buffer.left(8).toUInt();
        buffer = buffer.right(length-8);
        return *this;
    }

private:
    QByteArray buffer;
};

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
