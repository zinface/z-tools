#include "fileuploadtask.h"

FileUploadTask::FileUploadTask(QObject *parent) : QObject(parent)
  , _readBlock(4096)
  ,_t(FileTransferTask::OTHER)
  ,state(FileTransferTask::NOT_FINISHED)
  ,_fileName(QString())
  ,_filePath(QString())
  ,_fileSize(0)
  ,_mtotalSize(0)
  ,tcp(nullptr)
{

}

FileUploadTask::~FileUploadTask() {
    if (this->tcp != nullptr) delete this->tcp;
}

// 上传任务
void FileUploadTask::setTaskParam(FileTransferTask::TaskType t, QTcpSocket *c, QString fileName, qint64 fileSize, QString filePath)
{
    this->_t = t;
    this->tcp = c;
    this->_fileName = fileName;
    this->_fileSize = fileSize;
    this->_filePath = filePath;

    this->_mfile.setFileName(this->_filePath);
}

void FileUploadTask::onStartUpload()
{
    bool isopen = this->_mfile.open(QIODevice::ReadOnly);
    if (isopen) {
        QByteArray bytes = _mfile.read(this->_readBlock);
        int bytesLen = bytes.size();

        _mstream.setDevice(this->tcp);
        _mstream.setVersion(QDataStream::Qt_5_0);

        _mstream << qint64(this->_fileSize) << this->_fileName;
        if (_mstream.device()->waitForBytesWritten(10)) {
            connect(this->tcp, &QTcpSocket::bytesWritten, this, &FileUploadTask::onWriteBytes);
            _mstream.writeRawData(bytes.constData(), bytesLen);
        }
    }
}

void FileUploadTask::onWriteBytes(qint64 bytes)
{
    this->_mtotalSize += bytes;
    QByteArray array = _mfile.read(_readBlock);
    if (this->_mtotalSize == this->_fileSize) {
        emit onFinished();
    }
    if (array.isEmpty()) {
        return;
    }
    int size = array.size();
    int wl = _mstream.writeRawData(array.constData(), size);
}

