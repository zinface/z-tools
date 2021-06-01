#include "filedownloadtask.h"

FileDownloadTask::FileDownloadTask(QObject *parent) : QObject(parent)
  , _t(FileTransferTask::TaskType::OTHER)
  , _ipAddress(QString())
  , _fileName(QString())
  , _filePath(QString())
  , _fileSavePath(QString())
  , _fileSize(0)
  , _mfileSize(0)
  ,_mtotalSize(0)
  ,tcp(nullptr)
{

}

FileDownloadTask::~FileDownloadTask() {
    if (this->tcp != nullptr) delete this->tcp;
}

// 下载任务
void FileDownloadTask::setTaskParam(FileTransferTask::TaskType t, QString ipAddres, int ipPort, QString fileName, qint64 fileSize, QString fileSavePath)
{
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

void FileDownloadTask::onReadyRead() {
    if (0 == _mfileSize && this->tcp->bytesAvailable() > sizeof(qint64)) {
        this->_mstream >> this->_mfileSize >> this->_mfileName;
        this->_mfile.setFileName(QDir(this->_fileSavePath).absoluteFilePath(_fileName));
        if (!_mfile.open(QIODevice::WriteOnly)) {
            QTextStream(stdout) << QString("退出了\n");
            return;
        }
        if (this->tcp->bytesAvailable() > 0) goto _receive;
    } else {
    _receive:
        qint64 recint = this->_mfileSize - this->_mtotalSize;
        qint64 size = qMin(this->tcp->bytesAvailable(), this->_mfileSize - this->_mtotalSize);
        QByteArray array(size, 0);
        int rl = this->_mstream.readRawData(array.data(), size);
        this->_mtotalSize += rl;
        QTextStream(stdout) << QString("准备接收: %1 - 约定大小: %2 - 应收: %3 - 实收: %4 - 已收: %5 \n")
                               .arg(this->_mfileName)
                               .arg(this->_mfileSize)
                               .arg(recint)
                               .arg(rl)
                               .arg(this->_mtotalSize);

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

void FileDownloadTask::onConnected() {
    QTextStream(stdout) << QString("onConnected: %1, %2\n").arg(this->_t).arg(this->_fileName);
    _mstream.setDevice(tcp);
    _mstream.setVersion(QDataStream::Qt_5_0);
    _mstream << qint8(this->_t) << this->_fileName;
}

void FileDownloadTask::Connect() {
    QTextStream(stdout) << QString("来自(%1:%2): 文件: %3, 大小: %4, ").arg(this->_ipAddress).arg(this->_ipPort).arg(this->_fileName).arg(this->_fileSize);

    QFileInfo info(QDir(this->_fileSavePath).absoluteFilePath(this->_fileName));
    if (info.exists() && info.size() == this->_fileSize) {
        QTextStream(stdout) << QString("己下载\n");
        emit onFinished();
        return;
    }
    QTextStream(stdout) << QString("准备下载\n");
    this->tcp->connectToHost(this->_ipAddress, this->_ipPort);
}
