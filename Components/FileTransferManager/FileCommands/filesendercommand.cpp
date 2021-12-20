#include "filesendercommand.h"

#include <QDir>
#include <QFileInfo>
#include <QNetworkInterface>

FileSenderCommand::FileSenderCommand(QObject *parent) : QObject(parent)
  ,workMode("Simple")
  ,workdir(QString())
  ,workport(8888)
  ,workthreadnums(64)
{
    this->manager.setMaxTaskToggetherRunningCount(this->workthreadnums);
    connect(&manager, &FileTransferManager::onRemoteFetchFileList, this, &FileSenderCommand::onClientFetchFileList);
    connect(&manager, &FileTransferManager::onRemoteFetchFile, this, &FileSenderCommand::onClientFetchFile);
    connect(&manager, &FileTransferManager::onRemotePushFileConfirm, this, &FileSenderCommand::onClientPushFileConfirm);
    connect(&manager, &FileTransferManager::onRemotePushFile, this, &FileSenderCommand::onClientPushFile, Qt::QueuedConnection);
    connect(&manager, &FileTransferManager::onRemoteFetchWork, this, &FileSenderCommand::onClientFetchWork, Qt::QueuedConnection);
    connect(&manager, &FileTransferManager::ServerUnListenError, this, &FileSenderCommand::onServerUnListenError);
}

void FileSenderCommand::setSenderFiles(QStringList &filepaths)
{
    this->files.append(filepaths);
}

void FileSenderCommand::setWorkMode(QString mode)
{
    this->workMode = mode;
}

void FileSenderCommand::setWorkDir(QString path)
{
    this->workdir = path;
    manager.setSavePath(workdir);
}

bool FileSenderCommand::setWorkPort(int port)
{
    this->workport = port;
    return true;
}

bool FileSenderCommand::setWorkThreadNums(int num)
{
    if (num <= 0 || num > 64) goto _failthreadnums;
    this->manager.setMaxTaskToggetherRunningCount(num);
    return true;

    _failthreadnums:
    return false;
}

void FileSenderCommand::start()
{
    manager.setManagerTask(QString(), this->workport);
    foreach(QHostAddress address, QNetworkInterface::allAddresses()){
        QString ipAddr = address.toString();
        QTextStream(stdout) << QString(" -- 等待连接中: %1: %2\n").arg(ipAddr).arg(this->workport);
    }
}

void FileSenderCommand::onClientFetchFileList(QTcpSocket *c)
{
    if (workMode == "Simple")
        // 发送原始指定文件
        foreach(QString filepath, this->files) {
            QFileInfo info(filepath);
            manager.broadCaseAction(c, FileTransferManager::S_APPEND, info.fileName(), info.size());
        }
    else if(workMode == "Cloud")
        // 发送本地最新文件
        foreach(QFileInfo info, QDir(this->workdir).entryInfoList(QDir::Files)) {
            manager.broadCaseAction(c, FileTransferManager::S_APPEND, info.fileName(), info.size());
        }
}

void FileSenderCommand::onClientFetchFile(QTcpSocket *c, QString filename)
{
    bool finded = false;
    if (workMode == "Simple")
        foreach(QString filepath, this->files) {
            QFileInfo info(filepath);
            if (info.fileName() == filename) {
                finded = true;
                QTextStream(stdout) << QString(" -- 下载: %1, size: %2\n").arg(filename).arg(info.size());
                manager.broadCaseAction(c, FileTransferManager::OP_UPLOAD, info.fileName(), info.size(), filepath);
            }
        }
    else if(workMode == "Cloud")
        foreach(QFileInfo info, QDir(this->workdir).entryInfoList(QDir::Files)) {
            if (info.fileName() == filename) {
                finded = true;
                QTextStream(stdout) << QString(" -- 下载: %1, size: %2\n").arg(filename).arg(info.size());
                manager.broadCaseAction(c, FileTransferManager::OP_UPLOAD, info.fileName(), info.size(), info.absoluteFilePath());
            }
        }
    if (!finded) {
        QFileInfo temp(QDir(this->workdir).filePath(filename));
//        QTextStream(stdout) << QString(" -- 下载: %1, size: %2\n").arg(filename).arg(temp.size());
        // ./x/y/z.txt
        if (temp.exists()) {
            // QString path = temp.absolutePath(); // ./x/y
            // QString name = temp.fileName(); // z.txt
            manager.broadCaseAction(c, FileTransferManager::OP_UPLOAD, temp.fileName(), temp.size(), temp.absoluteFilePath());
        } else {
            c->close();
        }
    }
}

void FileSenderCommand::onClientPushFileConfirm(QTcpSocket *c, QString filename, qint64 filesize)
{
    if(!this->workdir.isEmpty()) {
        QFileInfo info(QDir(workdir).absoluteFilePath(filename));
        if (info.exists() && info.size() == filesize) return;
        manager.broadCasePushFileConfirm(c, filename, filesize, this->workdir);
    }
}

void FileSenderCommand::onClientPushFile(QTcpSocket *c, QString filename, qint64 filesize)
{
    manager.fetchFileAction(c, filename, filesize, this->workdir);
}

void FileSenderCommand::onClientFetchWork(QTcpSocket *c)
{
    manager.broadCaseWorkAction(c, this->workdir);
}

void FileSenderCommand::onServerUnListenError()
{
    QTextStream(stdout) << QString("好家伙,监听失败(退出).\n");
    exit(0);
}
