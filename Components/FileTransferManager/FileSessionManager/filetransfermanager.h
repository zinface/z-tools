#ifndef FILETRANSFERMANAGER_H
#define FILETRANSFERMANAGER_H

#include "sessionmanager.h"

#include <QObject>
#include <QTcpSocket>

class FileTransferManager : public QObject
{
    Q_OBJECT
public:
    explicit FileTransferManager(QObject *parent = nullptr);
    ~FileTransferManager();

    enum FullEvent{
        OP_ALL = 1,
        OP_DOWN,
        S_APPEND,
        S_DELETE,
        S_CLEANR,
    };

    void setManagerTask(QString host, int port, SessionManager::SessionManagerWorkType type = SessionManager::SERVER);

    bool state();
    bool serverState();
    bool clientState();

    // Server Public Function
    void pushFileAppend(QString filename, qint64 filesize);
    void pushFileDeleted(QString filename);
    void pushFileDeleted(QStringList filenames);
    void pushFileClaer();

    void broadCaseAction(QTcpSocket *c, FullEvent e, QString filename);
    void broadCaseAction(QTcpSocket *c, FullEvent e, QString filename, qint64 filesize);

    // Client Actions
    void fetchFileListAction();

private:
    SessionManager *adapter;
    SessionManager::SessionManagerWorkType _manager_work;

private slots:
    void onNewAction(QTcpSocket *c);

signals:
    /** Manager Signals******/
    void clientCountChanged(int count);
    void newConnectSocket(QTcpSocket *c);
    void onRemoteFetchFileList(QTcpSocket *c);

    void connected();
    void readRead();
    void disconnected();

    /** Manager Actions */
    void onRemoteFileAppend(const QString &filename, qint64 filesize);
    void onRemoteFileDelete(const QString &filename);
    void onRemoteFileClear();

};

#endif // FILETRANSFERMANAGER_H
