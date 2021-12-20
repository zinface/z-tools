#include "sessionmanager.h"
#include "filetransfermanager.h"
#include "filetransfertaskmanager.h"
#include "filedownloadtask.h"
#include "fileuploadtask.h"

#include <QAbstractSocket>
#include <QApplication>
#include <QBuffer>
#include <QGroupBox>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>

FileTransferManager::FileTransferManager(QObject *parent) : QObject(parent)
  , adapter(new InfomationManager)
  ,_manager_work(SessionManager::SERVER)
  ,taskManager(new FileTransferTaskManager)
  ,registedActions(false)
{
    connect(adapter, &InfomationManager::clientCountChanged, this, &FileTransferManager::clientCountChanged);

    connect(adapter, &InfomationManager::connected, this, &FileTransferManager::connected);
    connect(adapter, &InfomationManager::disconnected, this, &FileTransferManager::disconnected);

    connect(adapter, &InfomationManager::ClientSocketConnected, this,&FileTransferManager::ClientSocketConnected);
    connect(adapter, &InfomationManager::ClientSocketConnecting, this,&FileTransferManager::ClientSocketConnecting);
    connect(adapter, &InfomationManager::ClientSocketUnConnected, this,&FileTransferManager::ClientSocketUnConnected);

//    connect(adapter, &SessionManager::serverUnListenError, this, &FileTransferManager::ServerUnListenError);

    // Server Mode
    connect(adapter, &InfomationManager::onRemoteFetch, this, &FileTransferManager::onRemoteFetch);
    connect(adapter, &InfomationManager::onRemoteFetchRaw, this, &FileTransferManager::onRemoteFetchRaw);


    // Client Mode
    connect(adapter, &InfomationManager::onReply, this, &FileTransferManager::onReply);
    connect(adapter, &InfomationManager::onReplyRaw, this, &FileTransferManager::onReplyRaw);
}

FileTransferManager::~FileTransferManager()
{

}

void FileTransferManager::initActionsRegister(bool rawmode)
{
    if (registedActions) return;
    // 需要注册的事件信息：事件动作，数据模式，生命周期接管过滤
    adapter->registerAction(OP_ALL, rawmode);
    adapter->registerAction(OP_UPLOAD, true, true);
    adapter->registerAction(OP_DOWNLOAD, true, true);
    adapter->registerAction(S_APPEND, rawmode);
    adapter->registerAction(S_DELETE, rawmode);
    adapter->registerAction(S_CLEANR, rawmode);
    adapter->registerAction(S_Confirm, true);
    adapter->registerAction(S_ReplyConfirm, rawmode);
    adapter->registerAction(S_Work, rawmode);
    adapter->registerAction(S_ReplyWork, rawmode);
    adapter->registerAction(S_WorkTasks, rawmode);
    adapter->registerAction(S_ReplyWorkTasks, rawmode);

    /* DirectoryActions */
    adapter->registerAction(FetchWorkDirectoryTree, rawmode);
    registedActions = true;
}

void FileTransferManager::setManagerTask(QString host, int port, SessionManager::SessionManagerWorkType type)
{
    initActionsRegister(type == SessionManager::CLIENT);
    adapter->setManagerMode(host, port, type);
    this->_manager_work = type;
}

void FileTransferManager::setMaxTaskToggetherRunningCount(int count)
{
    taskManager->setMaxTaskToggether(count);
}

int FileTransferManager::getToggetherRunningCount() { return this->taskManager->togetherRunningTaskCount();}

void FileTransferManager::pushFileAppend(QString filename, qint64 filesize)
{
    broadCaseAction(nullptr, S_APPEND, filename, filesize);
}

void FileTransferManager::pushFileDeleted(QString filename)
{
    broadCaseAction(nullptr, S_DELETE, filename);
}

void FileTransferManager::pushFileClaer()
{
    broadCaseAction(nullptr, S_CLEANR, QString());
}

/**
 * 响应下载任务，远程连接，文件名，文件大小，文件路径
 * @brief FileTransferManager::broadCaseAction
 * @param c
 * @param e
 * @param fileName
 * @param filePath
 * @param fileSize
 */
void FileTransferManager::broadCaseAction(QTcpSocket *c, FullEvent e, QString fileName, qint64 fileSize, QString filePath)
{
    // Action: UpLoad
    // OP_UPLOAD, remoteSocket, fileName, fileSize, filePath
    FileUploadTask *uploadTask = new FileUploadTask();
    if (c == nullptr) {
        uploadTask->setTaskParam(FileTransferTask::UPLOAD, this->adapter->ra(), this->adapter->rp(), fileName, fileSize, filePath);
    } else {
        uploadTask->setTaskParam(FileTransferTask::UPLOAD, c, fileName, fileSize, filePath);
    }

    FileTransferTask *task = new FileTransferTask(FileTransferTask::UPLOAD);

    connect(task, &FileTransferTask::startUpload, uploadTask, &FileUploadTask::onStartUpload);
    connect(uploadTask, &FileUploadTask::onFinished, task, &FileTransferTask::finishDownload);
    connect(uploadTask, &FileUploadTask::onFinished, uploadTask, &FileUploadTask::deleteLater);

    taskManager->addFileTask(task);
    taskManager->doStart();
}

/**
 * 主动拉取文件列表动作
 * @brief FileTransferManager::fetchFileListAction
 */
void FileTransferManager::fetchFileListAction()
{
    adapter->broadCaseAction(OP_ALL);
}

void FileTransferManager::fetchDirectoryTreeAction()
{
    adapter->broadCaseAction(FetchWorkDirectoryTree);
}

/**
 * 创建下载任务，远程主机地址/端口，远程文件名，远程文件大小，下载储存路径
 * @brief FileTransferManager::fetchFileAction
 * @param filename
 * @param filesize
 * @param savePath
 */
void FileTransferManager::fetchFileAction(QTcpSocket *c, const QString filename, qint64 filesize, const QString savePath)
{
    FileDownloadTask *downloadTask = new FileDownloadTask;
    if (c == nullptr) {
        downloadTask->setTaskParam(FileTransferTask::DOWNLOAD, adapter->ra(), adapter->rp(), filename, filesize, savePath);
    } else {
        downloadTask->setTaskParam(FileTransferTask::DOWNLOAD, c, filename, filesize, savePath);
    }

    FileTransferTask *task = new FileTransferTask(FileTransferTask::DOWNLOAD);

    connect(task, &FileTransferTask::startDownload, downloadTask, &FileDownloadTask::onStartDownload);
    connect(downloadTask, &FileDownloadTask::onFinished, task, &FileTransferTask::finishDownload);
    connect(downloadTask, &FileDownloadTask::onFinished, downloadTask, &FileDownloadTask::deleteLater);

    taskManager->addFileTask(task);
    taskManager->doStart();
}

/**
 * 创建下载任务，远程主机地址/端口，远程文件名，远程文件大小，下载储存路径
 * @brief FileTransferManager::fetchFileItemInfoAction
 * @param fileinfo
 */
void FileTransferManager::fetchFileItemInfoAction(const FileItemInfo &fileinfo)
{
    // Action: Download
    // OP_DOWNLOAD, remoteAddress, remotePort, fileName, fileSize, fileSavePath
    FileDownloadTask *downloadTask = new FileDownloadTask();
    downloadTask->setTaskParam(FileTransferTask::DOWNLOAD, adapter->ra(), adapter->rp(), fileinfo.fileName, fileinfo.filesize, this->savePath);
    connect(downloadTask, &FileDownloadTask::onTotalWriteBytes, &fileinfo, &FileItemInfo::onTotalWriteBytes);

    FileTransferTask *task = new FileTransferTask(FileTransferTask::DOWNLOAD);

    connect(task, &FileTransferTask::startDownload, downloadTask, &FileDownloadTask::onStartDownload);
    connect(downloadTask, &FileDownloadTask::onFinished, task, &FileTransferTask::finishDownload);
//    connect(downloadTask, &FileDownloadTask::onFinished, downloadTask, &FileDownloadTask::deleteLater);

    taskManager->addFileTask(task);
    taskManager->doStart();
}

void FileTransferManager::fetchWorkAction()
{
    adapter->broadCaseAction(S_Work);
}

void FileTransferManager::broadCaseWorkAction(QTcpSocket *c, QString work)
{
    if (c->state() == QAbstractSocket::UnconnectedState) return;

    Package package = Package() << work;
    adapter->broadCaseAction(c, S_ReplyWork, package.toByteArray().length() ,package.toByteArray());
}

void FileTransferManager::fetchWorkTasksAction()
{
    adapter->broadCaseAction(S_WorkTasks);
}

void FileTransferManager::broadCaseWorkTasksAction(QTcpSocket *c)
{
    Package package = Package() << QString::number(taskManager->togetherRunningTaskCount());
    adapter->broadCaseAction(c, S_ReplyWorkTasks, package.toByteArray().length() ,package.toByteArray());
}

/**
 * 请求上传文件确认
 * @brief FileTransferManager::fetchPushFileConfirm
 * @param filename
 * @param filesize
 */
void FileTransferManager::fetchPushFileConfirm(const QString filename, qint64 filesize)
{
    Package package = Package() << filename <<filesize;
    adapter->broadCaseAction(adapter->c(), S_Confirm, package.toByteArray().length() ,package.toByteArray());
}

/**
 * 响应上传文件确认
 * @brief FileTransferManager::broadCasePushFileConfirm
 * @param c
 * @param filename
 * @param filesize
 * @param savePath
 */
void FileTransferManager::broadCasePushFileConfirm(QTcpSocket *c, const QString filename, qint64 filesize, const QString savePath)
{
    if (c->state() == QAbstractSocket::UnconnectedState) return;

    Package package = Package() << filename <<filesize;
    adapter->broadCaseAction(c, S_ReplyConfirm, package.toByteArray().length() ,package.toByteArray());
}

void FileTransferManager::broadCaseAction(QTcpSocket *c, FullEvent e, QString filename)
{
    Package package = Package() << filename;
    adapter->broadCaseAction(c, e, package.toByteArray().length() ,package.toByteArray());
}

void FileTransferManager::broadCaseAction(QTcpSocket *c, FullEvent e, QString filename, qint64 filesize)
{
    Package package = Package() << filename << filesize;
    adapter->broadCaseAction(c, e, package.toByteArray().length() ,package.toByteArray());
}

Package FileTransferManager::broadCaseWorkDirectoryTree(qint8 action, QTcpSocket *c, QString parent, QString relative)
{
    Package package;
    foreach(QFileInfo info, QDir(parent).entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries)) {
        QString path = QString("%1/%2").arg(relative).arg(info.fileName());
        if (info.isDir()) {
            package << qint8(RAWDIR) << qint64(-1) << path;
//            adapter->broadCaseAction(c, action, package.toByteArray().length(), package.toByteArray());
            package <<  broadCaseWorkDirectoryTree(action, c, QDir(parent).absoluteFilePath(info.fileName()), relative+"/"+info.fileName());
        } else {
            package << qint8(RAWFILE) << qint64(info.size()) << path;
//            adapter->broadCaseAction(c, action, package.toByteArray().length(), package.toByteArray());
        }
    }
    return package;

}

void FileTransferManager::onRemoteFetch(qint8 action, QTcpSocket *c)
{
    switch (action) {
        case OP_ALL: emit onRemoteFetchFileList(c); break;
        case S_Work: emit onRemoteFetchWork(c); break;
        case S_WorkTasks: broadCaseWorkTasksAction(c); break;
        case FetchWorkDirectoryTree:
            Package package;
            package << broadCaseWorkDirectoryTree(action, c, this->savePath, ".");
            adapter->broadCaseAction(c, action, package.toByteArray().length(), package.toByteArray());
            QTextStream(stdout) << QString("发送包大小: %1\n").arg(package.size());
            break;
    }
}

void FileTransferManager::onRemoteFetchRaw(qint8 action, qint64 length, QByteArray &data, QTcpSocket *c)
{
    QString filename;
    qint64 filesize(0);
    QString work;

    Package package(data);

    switch (action) {
        case S_Confirm:
            package >> filename >> filesize;
            emit onRemotePushFileConfirm(c, filename, filesize); break;
        case OP_UPLOAD:
            package >> filesize >> filename;
            emit onRemotePushFile(c, filename, filesize); break;
        case OP_DOWNLOAD:
            package >> filename;
            emit onRemoteFetchFile(c, filename); break;
    }
}

void FileTransferManager::onReply(qint8 action, QString msg)
{

}

void FileTransferManager::onReplyRaw(qint8 action, qint64 length, QByteArray &data)
{
    QString filename;
    qint64 filesize(0);
    QString work;
    QString workTasks;

    qint8 ftype;
    QString path;

    Package package(data);

    switch (action) {
        case S_APPEND:
            package >> filename >> filesize;
            emit onRemoteFileAppend(filename, filesize); break;
        case S_DELETE:
            package >> filename;
            emit onRemoteFileDelete(filename); break;
        case S_CLEANR:
            package >> filename;
            emit onRemoteFileClear(); break;
        case S_ReplyConfirm:
            package >> filename >> filesize;
            emit onReplyPushFileConfirm(filename, filesize); break;
        case S_ReplyWork:
            package >> work;
            emit onReplyFetchWork(work); break;
        case S_ReplyWorkTasks:
            package >> workTasks;
            emit onReplyFetchWorkTasks(workTasks); break;
        case FetchWorkDirectoryTree:
            QTextStream(stdout) << QString("接收包大小: %1\n").arg(package.size());
            while(package.size() > 0) {
                package >> ftype >> filesize>>  path;
                emit onReplyFetchDirectoryTree(ftype, filesize, path);
            }break;
    }
}


