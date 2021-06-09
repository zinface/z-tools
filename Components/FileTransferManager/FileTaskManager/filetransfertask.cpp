#include "filetransfertask.h"

#include <QDataStream>
#include <QDir>
#include <QtMath>

// 虚拟任务
FileTransferTask::FileTransferTask(FileTransferTask::TaskType t)
    : _t(OTHER)
{
    this->_t = t;
}

FileTransferTask::~FileTransferTask()
{

}

void FileTransferTask::showProgress(TaskType type, QString ipaddress, int port, QString filename, qint64 filesize, qint64 receiversize)
{
    QString progress("###################################");
    QString whiteres("...................................");
    QString whitestr("                                                                                  ");
    QString name = filename.mid(0,50);
    QString filetype = type==UPLOAD?QString("推送"):QString("来自");

    int percentage = double(double(receiversize) / double(filesize)) * 100;
    if (percentage == 100){
        QTextStream(stdout) << QString("[%5%6] %7%: %8(%1:%2): 文件: %3%4\n").arg(ipaddress)
                                       .arg(port)
                                       .arg(name)
                                       .arg(whitestr.mid(0,50-name.length()))
                                       .arg(progress.mid(0,ceil(progress.length() * (double(percentage)/100))))
                                       .arg(whiteres.mid(0,whiteres.length()-progress.mid(0,ceil(progress.length() * (double(percentage)/100))).length()))
                                       .arg(percentage)
                                        .arg(filetype);
    } else {
        QTextStream(stdout) << QString("[%5%6] %7%: %8(%1:%2): 文件: %3%4\r").arg(ipaddress)
                               .arg(port)
                               .arg(name)
                               .arg(whitestr.mid(0,50-name.length()))
                               .arg(progress.mid(0,ceil(progress.length() * (double(percentage)/100))))
                               .arg(whiteres.mid(0,whiteres.length()-progress.mid(0,ceil(progress.length() * (double(percentage)/100))).length()))
                               .arg(percentage)
                               .arg(filetype);
    }
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
