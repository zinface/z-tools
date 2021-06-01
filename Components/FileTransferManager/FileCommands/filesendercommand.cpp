#include "filesendercommand.h"

#include <QFileInfo>

FileSenderCommand::FileSenderCommand(QObject *parent) : QObject(parent)
  ,workport(8888)
  ,workthreadnums(64)
{
    this->manager.setMaxTaskToggetherRunningCount(this->workthreadnums);
    connect(&manager, &FileTransferManager::onRemoteFetchFileList, this, &FileSenderCommand::onClientFetchFileList);
    connect(&manager, &FileTransferManager::onRemoteFetchFile, this, &FileSenderCommand::onClientFetchFile);
}

void FileSenderCommand::setSenderFiles(QStringList &filepaths)
{
    this->files.append(filepaths);
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
}

void FileSenderCommand::onClientFetchFileList(QTcpSocket *c)
{
    foreach(QString filepath, this->files) {
        QFileInfo info(filepath);
        manager.broadCaseAction(c, FileTransferManager::S_APPEND, info.fileName(), info.size());
    }
}

void FileSenderCommand::onClientFetchFile(QTcpSocket *c, QString filename)
{
    foreach(QString filepath, this->files) {
        QFileInfo info(filepath);
        if (info.fileName() == filename) {
            QTextStream(stdout) << QString(" -- 下载: %1, size: %2\n").arg(filename).arg(info.size());
            manager.broadCaseAction(c, FileTransferManager::OP_UPLOAD, info.fileName(), info.size(), filepath);
        }
    }
}
