#include "filetransfertask.h"

#include <QDataStream>
#include <QDir>

FileTransferTask::FileTransferTask()
    : _t(OTHER)
{

}

FileTransferTask::~FileTransferTask()
{

}

// 虚拟任务
void FileTransferTask::setTaskParam(FileTransferTask::TaskType t)
{
    this->_t = t;
}

void FileTransferTask::onStartUpload()
{

}

void FileTransferTask::finishUpload()
{
    emit onFinished(this);
}

void FileTransferTask::onStartDownload()
{

}

void FileTransferTask::finishDownload()
{
    emit onFinished(this);
}

void FileTransferTask::run()
{
    if (this->_t == TaskType::UPLOAD) {
        // 发送者队列无限制
        emit startUpload();
    } else if (this->_t == TaskType::DOWNLOAD){
        // 接收者队列由任务管理器进行驱动
        emit startDownload();
    }
}
