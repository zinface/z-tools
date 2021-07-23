#ifndef FILESESSIONADAPTER_H
#define FILESESSIONADAPTER_H

#include <QDataStream>
#include <QObject>
#include <QTcpServer>

/**
 * @brief The SessionManager class
 * 会话管理器，工作模式分两种，可负责处理新的连接对象
 */
class SessionManager : public QObject
{
    Q_OBJECT
public:
    friend class InfomationManager;
    SessionManager(QObject *parent = nullptr);
    ~SessionManager();

public:
    /**
     * @brief The SessionManagerWorkType enum
     * 会话管理器工作模式
     */
    enum SessionManagerWorkType {
        SERVER,     /* Server 模式*/
        CLIENT,     /* Client */
    };

    /**
     * @brief The SessionManagerWorkState enum
     * 会话管理器工作状态
     */
    enum SessionManagerWorkState {
        /* Server 模式*/
        UNLISTENED,       // 未监听
        LISTENED,         // 已监听

        /* Client */
        UNCONNECTED,      // 未连接
        CONNECTED,        // 已连接
    };

    /**
     * @brief SettingHost
     * @param host 需要连接的主机IP
     * @param port 需要监听或连接的端口号
     * @param type 工作模式
     *
     */
    void SettingHost(QString host, int port, SessionManagerWorkType type = SERVER);

    /**
     * @brief clients
     * @return 所有客户端连接，QString 标志为第几个连接
     */
    QMap<QString, QTcpSocket*> clients() { return  this->clientMap; }

    /**
     * @brief ConnectSocketSignal
     * @param c 需要维护信号的新客户端
     * 对新增客户端连接维护
     */
    void ConnectSocketSignal(QTcpSocket *c);

    QTcpServer* s(){ return this->server; }
    QTcpSocket* c(){ return this->client; }

    QString ra(){ return this->m_remoteAddress; };
    int rp(){ return this->m_remotePort; };


    /**
     * @brief status
     * @return 返回相对工作状态是否正常
     * 获取工作状态
     */
    bool status() {
        if (this->_workType == SERVER) {
            return this->_workState == LISTENED;
        } else {
            return this->_workState == CONNECTED;
        }
    }

    /**
     * @brief workState
     * @return
     * 获取工作状态和重载状态函数
     */
    SessionManagerWorkState workState() { return this->_workState; }
    SessionManagerWorkState serverState() { return this->_workState; };
    SessionManagerWorkState clientState() { return this->_workState; };


private slots:
    // Client模式 连接状态发生改变时
    void onStateChanged(QAbstractSocket::SocketState state);
    // 任何模式下，发生错误时
    void onError(QAbstractSocket::SocketError error);

    // Server Function
//    void onNewConnectSocket();
    void onReadyRead();
    void onDisconnected();

signals:
    /** Server Signals******/
    // 连接产生变化事件
    void clientCountChanged(int count);
    // 新连接加入事件
    void newConnectSocket(QTcpSocket *c);
    // 新的可读事件
    void newAction(qint8 action, QTcpSocket *c);
    // 服务监听失败事件
    void serverUnListenError();

    /** Client Signals******/
    void connected();
    void readRead();
    void disconnected();

    /** Client Socket Signals **/
    void ClientSocketConnected();
    void ClientSocketConnecting();
    void ClientSocketUnConnected();

private:
    int conn_cnt = 0;
    QMap<QString, QTcpSocket*> clientMap;

    QTcpServer *server;
    QTcpSocket *client;

    QString m_remoteAddress;
    int m_remotePort;

    SessionManagerWorkState _workState;
    SessionManagerWorkType _workType;
};

#endif // FILESESSIONADAPTER_H
