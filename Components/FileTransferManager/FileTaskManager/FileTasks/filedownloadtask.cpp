#include "filedownloadtask.h"

#include <QFont>
#include <infomationmanager.h>

/**
 * @brief FileDownloadTask::FileDownloadTask
 * @param parent
 * @param mode 主从模式设置
 */
FileDownloadTask::FileDownloadTask(QObject *parent, FileTransferTask::TaskRole mode) : QObject(parent)
  , _t(FileTransferTask::TaskType::OTHER)
  , _m(FileTransferTask::TaskRole::Master)
  , _ipAddress(QString())
  , _fileName(QString())
  , _filePath(QString())
  , _fileSavePath(QString())
  , _fileSize(0)
  , _mfileSize(0)
  ,_mtotalSize(0)
  ,tcp(nullptr)
{
    this->_m = mode;
}

FileDownloadTask::~FileDownloadTask() {
    if (this->_m == FileTransferTask::Master && this->tcp != nullptr) delete this->tcp;
    if (this->_mfile.isOpen()) _mfile.close();
}

// 下载任务: Master
void FileDownloadTask::setTaskParam(FileTransferTask::TaskType t, QString ipAddres, int ipPort, QString fileName, qint64 fileSize, QString fileSavePath)
{
    this->_m = FileTransferTask::Master;
    this->_t = t;
    this->_ipAddress = ipAddres;
    this->_ipPort = ipPort;
    this->_fileName = fileName;
    this->_fileSize = fileSize;
    this->_fileSavePath = fileSavePath;

    this->tcp = new QTcpSocket;
    connect(tcp, &QTcpSocket::connected, this, &FileDownloadTask::onConnected);
    connect(tcp, &QTcpSocket::readyRead, this, &FileDownloadTask::onReadyRead);
}

// 下载任务: Slave
void FileDownloadTask::setTaskParam(FileTransferTask::TaskType t, QTcpSocket *c, QString fileName, qint64 fileSize, QString fileSavePath)
{
    this->_m = FileTransferTask::Slave;
    this->_t = t;
    this->tcp = c;
    this->_fileName = fileName;
    this->_fileSize = fileSize;
    this->_fileSavePath = fileSavePath;

    connect(tcp, &QTcpSocket::disconnected, this, &FileDownloadTask::onDisConnected);
}

void FileDownloadTask::onReadyRead() {
    if (0 == _mfileSize && this->tcp->bytesAvailable() > sizeof(qint64)) {
        this->_mstream >> this->_mfileSize >> this->_mfileName;
        this->_mfile.setFileName(QDir(this->_fileSavePath).absoluteFilePath(_fileName));
        if (!_mfile.open(QIODevice::WriteOnly)) {
            QTextStream(stdout) << this->_fileName << QString("，WriteOnly 异常退出了\n");
            return;
        }
        if (this->tcp->bytesAvailable() > 0) goto _receive;
    } else {
    _receive:
        qint64 recint = this->_mfileSize - this->_mtotalSize;
        qint64 size = qMin(this->tcp->bytesAvailable(), this->_mfileSize - this->_mtotalSize);
        QByteArray array(size, 0);
        int rl = this->_mstream.readRawData(array.data(), size);
        if (rl==-1) {
            QTextStream(stdout) << this->_fileName << QString(", 异常无数据\n");
            return;
        }
        this->_mtotalSize += rl;

        FileTransferTask::showProgress(this->_t, this->_ipAddress, this->_ipPort, this->_fileName, this->_fileSize, this->_mtotalSize);

        emit onTotalWriteBytes(this->_mtotalSize);

        if (this->_mfile.isOpen()) {
            this->_mfile.write(array.data(), rl);
        }

        if (this->_mtotalSize == this->_mfileSize) {
            this->_mfile.close();
            emit onFinished();
        }
    }
}

void FileDownloadTask::onDisConnected()
{
    disconnect(this->tcp, &QTcpSocket::readyRead, this, &FileDownloadTask::onReadyRead);
    emit onFinished();
}

void FileDownloadTask::onConnected() {
    _mstream.setDevice(tcp);
    _mstream.setVersion(QDataStream::Qt_5_0);
//    _mstream << qint8(this->_t) << this->_fileName;
    Package package = Package() << this->_fileName;
    InfomationManager().broadCaseAction(tcp, this->_t, package.toByteArray().length(), package.toByteArray());

//    Package package = Package() << this->_fileName;

//    _mstream.setDevice(tcp);
//    _mstream.setVersion(QDataStream::Qt_5_0);
//    _mstream << qint8(this->_t) << qint64(package.toByteArray().length()) << package.toByteArray();
}

void FileDownloadTask::Connect() {
    this->tcp->connectToHost(this->_ipAddress, this->_ipPort);
}


void FileDownloadTask::onStartDownload()
{
    QFileInfo info(QDir(this->_fileSavePath).absoluteFilePath(this->_fileName));
    if (this->_m == FileTransferTask::Master) goto _MasterMode;
    if (!(info.exists() && info.size() == this->_fileSize)) {
        _mstream.setDevice(tcp);
        _mstream.setVersion(QDataStream::Qt_5_0);
        connect(tcp, &QTcpSocket::readyRead, this, &FileDownloadTask::onReadyRead);
        onReadyRead();
    }else{
        this->tcp->close();
    }
    return;

    _MasterMode:
    if (!(info.exists() && info.size() == this->_fileSize)) Connect();
    else {
        QTextStream(stdout) << QString("来自(%1:%2): 文件: %3, 己下载\n").arg(this->_ipAddress).arg(this->_ipPort).arg(this->_fileName);
        this->tcp->close();
        emit onFinished();
    }
}
