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

    /** 文件传输管理器可处理事件 */
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
        FetchWorkDirectoryTree,
        FetchWorkDirectoryFile,

    };

    /** 对于目录传输相对的条目标志 */
    enum FileType {
        RAWDIR,
        RAWFILE
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

    /** 文件传输管理器动作封装 */
    // 多级重用函数
    void broadCaseAction(QTcpSocket *c, FullEvent e, QString filename);
    void broadCaseAction(QTcpSocket *c, FullEvent e, QString filename, qint64 filesize);

    // Gui 动作相关事件函数
    // 推送添加文件
    void pushFileAppend(QString filename, qint64 filesize);
    // 推送删除文件
    void pushFileDeleted(QString filename);
    // 推送清空文件
    void pushFileClaer();

    /** 下载任务创建方法 */
    // 创建任务 - 创建/响应上传任务，远程连接，远程文件名，远程文件大小，下载储存路径
    void fetchFileAction(QTcpSocket *c, const QString filename, qint64 filesize, const QString savePath);
    // 创建任务 - 文件信息体
    void fetchFileItemInfoAction(const FileItemInfo &fileinfo);
    // 创建任务 -
    void fetchDirectoryFileAction(QTcpSocket *c, const QString filename, qint64 filesize, const QString savePath);

    /** 上传任务创建方法 */
    // 上传文件确认 - 进行创建文件
    void fetchPushFileConfirm(const QString filename, qint64 filesize);
    void broadCasePushFileConfirm(QTcpSocket *c, const QString filename, qint64 filesize, const QString savePath);
    // 文件广播函数：准备上传/响应下载任务，远程连接，文件名，文件大小，文件路径
    void broadCaseAction(QTcpSocket *c, FullEvent e, QString fileName, qint64 fileSize, QString filePath);


    /** Other */
    // 拉取/响应工作目录
    void fetchWorkAction();
    void broadCaseWorkAction(QTcpSocket *c, QString work);

    // 拉取/响应当前运行任务数
    void fetchWorkTasksAction();
    void broadCaseWorkTasksAction(QTcpSocket *c);

    /** 顶级基础操作 - CLI 就绪操作 */
    // Client Actions
    // 拉取文件列表
    void fetchFileListAction();
    // 拉取/响应目录树 - 内部自动化 - 需要
    void fetchDirectoryTreeAction();
    Package broadCaseWorkDirectoryTree(qint8 action, QTcpSocket *c, QString parent, QString relative);

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

//    void onRemoteFetchDirectoryTree(QTcpSocket *c);

    /** Manager Actions */
    void onRemoteFileAppend(const QString &filename, qint64 filesize);
    void onRemoteFileDelete(const QString &filename);
    void onRemoteFileClear();
    void onReplyPushFileConfirm(QString filename, qint64 filesize);
    void onReplyFetchWork(QString work);
    void onReplyFetchWorkTasks(QString workTasks);

    void onReplyFetchDirectoryTree(qint8 ftype, qint64 filesize, QString path);

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
