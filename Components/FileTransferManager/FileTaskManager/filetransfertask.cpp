#include "filetransfertask.h"

#include <QDataStream>
#include <QDir>

FileTransferTask::FileTransferTask()
  : _readBlock(4096)
  , _t(OTHER)
  , _c(nullptr)
  , _ipAddress(QString())
  , _fileName(QString())
  , _filePath(QString())
  , _fileSavePath(QString())
  , _fileSize(0)

  , _mfileName(QString())
  , _mfileSize(0)
  ,_msocket(nullptr)
  ,_mtotalSize(0)
{

}

FileTransferTask::~FileTransferTask()
{

}

void FileTransferTask::setTaskParam(FileTransferTask::TaskType t, QTcpSocket *c, QString fileName, qint64 fileSize, QString filePath)
{
    this->_t = t;
    this->_c = c;
    this->_fileName = fileName;
    this->_fileSize = fileSize;
    this->_filePath = filePath;

    this->_mfile.setFileName(this->_filePath);
    bool isopen = this->_mfile.open(QIODevice::ReadOnly);

    if (isopen) {
        QByteArray bytes = _mfile.read(this->_readBlock);
        int bytesLen = bytes.size();

        _mstream.setDevice(this->_c);
        _mstream.setVersion(QDataStream::Qt_5_0);

        _mstream << qint64(this->_fileSize) << this->_fileName;
        if (_mstream.device()->waitForBytesWritten(10)) {
            connect(c, &QTcpSocket::bytesWritten, this, &FileTransferTask::onWriteBytes);
            _mstream.writeRawData(bytes.constData(), bytesLen);
        }
    }
}

void FileTransferTask::setTaskParam(FileTransferTask::TaskType t, QString ipAddres, int ipPort, QString fileName, qint64 fileSize, QString fileSavePath)
{
    this->_t = t;
    this->_ipAddress = ipAddres;
    this->_ipPort = ipPort;
    this->_fileName = fileName;
    this->_fileSize = fileSize;
    this->_fileSavePath = fileSavePath;

    this->_c = new QTcpSocket;
    connect(_c, &QTcpSocket::connected, this, &FileTransferTask::onConnected);
    connect(_c, &QTcpSocket::readyRead, this, &FileTransferTask::onReadyRead);
    _c->connectToHost(this->_ipAddress, this->_ipPort);
}

void FileTransferTask::onConnected()
{
    _mstream.setDevice(_c);
    _mstream.setVersion(QDataStream::Qt_5_0);
    _mstream << qint8(this->_t) << this->_fileName;
}

void FileTransferTask::onReadyRead()
{
    if (0 == _mfileSize && this->_c->bytesAvailable() > sizeof(qint64)) {
        this->_mstream >> this->_mfileSize >> this->_mfileName;
        this->_mfile.setFileName(QDir(this->_fileSavePath).absoluteFilePath(_mfileName));

        if (this->_mfile.exists() && _mfile.size() == this->_mfileSize) {
            _mfile.close();
            return;
        }

        if (!_mfile.open(QIODevice::WriteOnly)) {
            return;
        }

    } else {
        qint64 size = qMin(this->_c->bytesAvailable(), this->_mfileSize - this->_mtotalSize);
        QByteArray array(size, 0);
        int rl = this->_mstream.readRawData(array.data(), size);
        this->_mtotalSize += rl;
        emit onTotalWriteBytes(this->_mtotalSize);

        if (this->_mfile.isOpen()) {
            this->_mfile.write(array.data(), rl);
        }

        if (this->_mtotalSize == this->_mfileSize) {
            this->_mfile.close();
        }
    }
}

void FileTransferTask::onWriteBytes(qint64 bytes)
{
    this->_mtotalSize += bytes;
    _mfile.seek(this->_mtotalSize);
    QByteArray array = _mfile.read(_readBlock);
    if (array.isEmpty()) {
        return;
    }
    int size = array.size();
    int wl = _mstream.writeRawData(array.constData(), size);
}

void FileTransferTask::run()
{

}
