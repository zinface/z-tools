#include "sessionmanager.h"
#include "filetransfermanager.h"
#include "filetransfertaskmanager.h"
#include "filedownloadtask.h"
#include "fileuploadtask.h"

#include <QAbstractSocket>
#include <QApplication>
#include <QGroupBox>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>

FileTransferManager::FileTransferManager(QObject *parent) : QObject(parent)
  , adapter(new SessionManager)
  ,_manager_work(SessionManager::SERVER)
  ,taskManager(new FileTransferTaskManager)
{
    connect(adapter, &SessionManager::newConnectSocket, this, &FileTransferManager::newConnectSocket);
    connect(adapter, &SessionManager::clientCountChanged, this, &FileTransferManager::clientCountChanged);
    connect(adapter, &SessionManager::newAction, this, &FileTransferManager::onNewAction);

    connect(adapter, &SessionManager::connected, this, &FileTransferManager::connected);
    connect(adapter, &SessionManager::readRead, this, &FileTransferManager::readRead);
    connect(adapter, &SessionManager::disconnected, this, &FileTransferManager::disconnected);

    connect(adapter, &SessionManager::ClientSocketConnected, this,&FileTransferManager::ClientSocketConnected);
    connect(adapter, &SessionManager::ClientSocketConnecting, this,&FileTransferManager::ClientSocketConnecting);
    connect(adapter, &SessionManager::ClientSocketUnConnected, this,&FileTransferManager::ClientSocketUnConnected);
}

FileTransferManager::~FileTransferManager()
{

}

void FileTransferManager::setManagerTask(QString host, int port, SessionManager::SessionManagerWorkType type)
{
    adapter->SettingHost(host, port, type);
    this->_manager_work = type;
}

void FileTransferManager::setMaxTaskToggetherRunningCount(int count)
{
    taskManager->setMaxTaskToggether(count);
}

bool FileTransferManager::state()
{
    if (this->_manager_work == SessionManager::SERVER) {
        return adapter->serverState() == SessionManager::LISTENED;
    } else {
        return adapter->serverState() == SessionManager::CONNECTED;
    }
}

bool FileTransferManager::serverState()
{
    return this->adapter->serverState() == SessionManager::LISTENED;
}

bool FileTransferManager::clientState()
{
    return this->adapter->serverState();
}

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
    uploadTask->setTaskParam(FileTransferTask::UPLOAD, c, fileName, fileSize, filePath);

    FileTransferTask *task = new FileTransferTask;
    task->setTaskParam(FileTransferTask::UPLOAD);

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
    QDataStream stream(adapter->c());
    stream.setVersion(QDataStream::Qt_5_0);
    stream << qint8(OP_ALL);
}

/**
 * 创建下载任务，远程主机地址/端口，远程文件名，远程文件大小，下载储存路径
 * @brief FileTransferManager::fetchFileAction
 * @param filename
 * @param filesize
 * @param savePath
 */
void FileTransferManager::fetchFileAction(const QString filename, qint64 filesize, const QString savePath)
{
    FileDownloadTask *downloadTask = new FileDownloadTask;
    downloadTask->setTaskParam(FileTransferTask::DOWNLOAD, adapter->ra(), adapter->rp(), filename, filesize, savePath);

    FileTransferTask *task = new FileTransferTask;
    task->setTaskParam(FileTransferTask::DOWNLOAD);

    connect(task, &FileTransferTask::startDownload, downloadTask, &FileDownloadTask::Connect);
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

    FileTransferTask *task = new FileTransferTask();
    task->setTaskParam(FileTransferTask::DOWNLOAD);

    connect(task, &FileTransferTask::startDownload, downloadTask, &FileDownloadTask::Connect);
    connect(downloadTask, &FileDownloadTask::onFinished, task, &FileTransferTask::finishDownload);
    connect(downloadTask, &FileDownloadTask::onFinished, downloadTask, &FileDownloadTask::deleteLater);

    taskManager->addFileTask(task);
    taskManager->doStart();
}

void FileTransferManager::broadCaseAction(QTcpSocket *c, FullEvent e, QString filename)
{
    if (c == nullptr) {
        foreach(QString key, adapter->clients().keys()) {
            QTcpSocket *c = adapter->clients()[key];
            QDataStream stream(c);
            stream.setVersion(QDataStream::Qt_5_0);
            stream << qint8(e) << filename;
        }
    } else {
        QDataStream stream(c);
        stream.setVersion(QDataStream::Qt_5_0);
        stream << qint8(e) << filename;
    }
}

void FileTransferManager::broadCaseAction(QTcpSocket *c, FullEvent e, QString filename, qint64 filesize)
{
    if (c == nullptr) {
        foreach(QString key, adapter->clients().keys()) {
            QTcpSocket *c = adapter->clients()[key];
            QDataStream stream(c);
            stream.setVersion(QDataStream::Qt_5_0);
            stream << qint8(e) << filename << filesize;
        }
    } else {
        QDataStream stream(c);
        stream.setVersion(QDataStream::Qt_5_0);
        stream << qint8(e) << filename << filesize;
    }
}

void FileTransferManager::onNewAction(QTcpSocket *c)
{
    c->waitForReadyRead(10);
    QDataStream stream(c);
    stream.setVersion(QDataStream::Qt_5_0);

    qint8 action;
    QString filename;
    QStringList filenames;
    qint64 filesize(0);

    stream >> action;

    switch (action) {

    case OP_ALL:
        emit onRemoteFetchFileList(c);
        break;

    case OP_DOWNLOAD:
        stream >> filename;
        emit onRemoteFetchFile(c, filename);
        break;

    case S_APPEND:
        stream >> filename >> filesize;
        emit onRemoteFileAppend(filename, filesize);
        break;

    case S_DELETE:
        stream >> filename;
        emit onRemoteFileDelete(filename);
        break;

    case S_CLEANR:
        stream >> filename;
        emit onRemoteFileClear();
        break;
    }

    while(c->bytesAvailable() > 0) {
        onNewAction(c);
    }
}


