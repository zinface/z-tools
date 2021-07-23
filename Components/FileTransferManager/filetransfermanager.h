#ifndef FILETRANSFERMANAGER_H
#define FILETRANSFERMANAGER_H

#include "../fileiteminfo.h"

#include <QObject>
#include <QTcpSocket>
#include <infomationmanager.h>

class FileTransferTaskManager;

/**
 * @brief The FileTransferManager class
 * 文件传输管理器，核心API版本
 */
class FileTransferManager : public QObject
{
    Q_OBJECT
public:
    explicit FileTransferManager(QObject *parent = nullptr);
    ~FileTransferManager();

    enum FullEvent{
        /* FileActions */
        OP_ALL = 1,
        OP_UPLOAD,
        OP_DOWNLOAD,
        S_APPEND,
        S_DELETE,
        S_CLEANR,
        S_Confirm,
        S_ReplyConfirm,
        S_Work,
        S_ReplyWork,

        S_WorkTasks,
        S_ReplyWorkTasks,

        /* DirectoryActions */

    };

    void initActionsRegister(bool rawmode);
    /**
     * @brief setManagerTask
     * @param host 连接的主机IP
     * @param port 监听或连接的端口号
     * @param type SessionManager工作模式
     */
    void setManagerTask(QString host, int port, SessionManager::SessionManagerWorkType type = SessionManager::SERVER);
    /**
     * @brief setSavePath
     * @param path 存储目录
     * 设置文件存储目录
     */
    void setSavePath(QString path) {this->savePath = path;}
    void setMaxTaskToggetherRunningCount(int count = 5);
    int getToggetherRunningCount();;

    bool state() { return this->adapter->state(); }
//    typedef stat

    // Server Base Functions
    // 推送添加文件
    void pushFileAppend(QString filename, qint64 filesize);
    // 推送列表删除文件
    void pushFileDeleted(QString filename);
    // 推送列表删除文件(s)
    void pushFileDeleted(QStringList filenames);
    // 推送清空列表
    void pushFileClaer();

    // Base BroadCase Reply Action
    void broadCaseAction(QTcpSocket *c, FullEvent e, QString filename);
    void broadCaseAction(QTcpSocket *c, FullEvent e, QString filename, qint64 filesize);
    void broadCaseAction(QTcpSocket *c, FullEvent e, QString fileName, qint64 fileSize, QString filePath);

    // Client Actions
    // 拉取文件列表
    void fetchFileListAction();              // Base Action
    // 创建任务 - 拉取文件
    void fetchFileAction(QTcpSocket *c, const QString filename, qint64 filesize, const QString savePath);       // CLI Action
    // 创建任务 - 拉取文件信息
    void fetchFileItemInfoAction(const FileItemInfo &fileinfo);  // Donwload

    // Master Actions
    // 拉取上传文件确认
    void fetchPushFileConfirm(const QString filename, qint64 filesize); // Base Action
    void broadCasePushFileConfirm(QTcpSocket *c, const QString filename, qint64 filesize, const QString savePath);  // Upload Confirm

    // 拉取工作目录
    void fetchWorkAction();
    void broadCaseWorkAction(QTcpSocket *c, QString work);

    // 拉取当前运行任务数
    void fetchWorkTasksAction();
    void broadCaseWorkTasksAction(QTcpSocket *c);
    // Slave Actions

private:
//    SessionManager *adapter;
    InfomationManager *adapter;
    SessionManager::SessionManagerWorkType _manager_work;

    FileTransferTaskManager *taskManager;
    QString savePath;

    bool registedActions;

private slots:
    void onRemoteFetch(qint8 action, QTcpSocket *c);
    void onRemoteFetchRaw(qint8 action, qint64 length, QByteArray &data, QTcpSocket *c);

    void onReply(qint8 action, QString msg);
    void onReplyRaw(qint8 action, qint64 length, QByteArray &data);

signals:
    /** Manager Signals******/
    void onRemoteFetchFileList(QTcpSocket *c);
    void onRemoteFetchFile(QTcpSocket *c, QString filename);
    void onRemotePushFileConfirm(QTcpSocket *c, QString filename, qint64 filesize);
    void onRemotePushFile(QTcpSocket *c, QString filename, qint64 filesize);
    void onRemoteFetchWork(QTcpSocket *c);

    /** Manager Actions */
    void onRemoteFileAppend(const QString &filename, qint64 filesize);
    void onRemoteFileDelete(const QString &filename);
    void onRemoteFileClear();
    void onReplyPushFileConfirm(QString filename, qint64 filesize);
    void onReplyFetchWork(QString work);
    void onReplyFetchWorkTasks(QString workTasks);


    /** Session Signals **/
    void connected();
    void readRead();
    void disconnected();

    void clientCountChanged(int count);
    void ClientSocketConnected();
    void ClientSocketConnecting();
    void ClientSocketUnConnected();
    void ServerUnListenError();

};

#endif // FILETRANSFERMANAGER_H
