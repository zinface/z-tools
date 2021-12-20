#include "directoryreceiver.h"
#include "DirectoryActions.h"

#include <QDir>

DirectoryReceiver::DirectoryReceiver(QObject *parent) : QObject(parent)
  ,_depth(0)
  ,_workHost("localhost")
{
    connect(&manager, &FileTransferManager::connected, this, &DirectoryReceiver::onConnected);
    connect(&manager, &FileTransferManager::onReplyFetchDirectoryTree, this, &DirectoryReceiver::onReplyFetchDirectoryTree);
}

void DirectoryReceiver::setDepth(int depth)
{
    this->_depth = depth;
}

void DirectoryReceiver::start()
{
    manager.setManagerTask(this->_workHost, 9999, SessionManager::CLIENT);
}

void DirectoryReceiver::onConnected()
{
    manager.fetchDirectoryTreeAction();
}

void DirectoryReceiver::onReplyFetchDirectoryTree(qint8 ftype, qint64 filesize, QString path)
{
    QDir work(this->_workDir);

    if (ftype == FileType::RAWDIR) {
        // 需要检查相对目录是否存在
        if (QFileInfo(work.absoluteFilePath(path)).exists()) {
            QTextStream(stdout) << QString("已存在目录: %1\n").arg(path);
        } else {
            // 准备创建目录
            if (work.mkpath(path)){
                QTextStream(stdout) << QString("已创建目录: %1\n").arg(path);
            }
        }
    } else {
        QTextStream(stdout) << QString("文件: %1\n").arg(path);
        // 准备处理此处的文件信息，
        QFileInfo temp(QDir(this->_workDir).filePath(path));
        manager.fetchFileAction(nullptr, path, filesize, temp.absolutePath());
    }
}
