#include "fileuploadtask.h"

#include <infomationmanager.h>

/**
 * @brief FileUploadTask::FileUploadTask
 * @param parent
 * @param mode 主从模式设置
 */
FileUploadTask::FileUploadTask(QObject *parent, FileTransferTask::TaskRole mode) : QObject(parent)
  , _readBlock(4096)
  ,_t(FileTransferTask::OTHER)
  ,_m(FileTransferTask::TaskRole::Slave)
  ,state(FileTransferTask::NOT_FINISHED)
  ,_fileName(QString())
  ,_filePath(QString())
  ,_fileSize(0)
  ,_mtotalSize(0)
  ,tcp(nullptr)
{
    this->_m = mode;
}

FileUploadTask::~FileUploadTask() {
    if (this->_m == FileTransferTask::Master){
        if (this->tcp != nullptr) delete this->tcp;
        if (this->_mfile.isOpen()) this->_mfile.close();
    }
}

// 上传任务: Slave
void FileUploadTask::setTaskParam(FileTransferTask::TaskType t, QTcpSocket *c, QString fileName, qint64 fileSize, QString filePath)
{
    this->_m = FileTransferTask::Slave;
    this->_t = t;
    this->tcp = c;
    this->_fileName = fileName;
    this->_fileSize = fileSize;
    this->_filePath = filePath;

    this->_mfile.setFileName(this->_filePath);
}

// 上传任务: Master
void FileUploadTask::setTaskParam(FileTransferTask::TaskType t, QString ipAddres, int ipPort, QString fileName, qint64 fileSize, QString filePath)
{
    this->_m = FileTransferTask::Master;
    this->_t = t;
    this->_ipAddress = ipAddres;
    this->_ipPort = ipPort;
    this->_fileName = fileName;
    this->_fileSize = fileSize;
    this->_filePath = filePath;

    this->_mfile.setFileName(this->_filePath);

    this->tcp = new QTcpSocket;
    connect(tcp, &QTcpSocket::connected, this, &FileUploadTask::onConnected);
    connect(tcp, &QTcpSocket::disconnected, this, &FileUploadTask::onDisConnected);
}

void FileUploadTask::onConnected() {
    // Master 模式已经连接到服务器，进行请求交互
    QByteArray bytes = _mfile.read(this->_readBlock);
    int bytesLen = bytes.size();

    Package package = Package() << this->_fileSize << this->_fileName;

    _mstream.setDevice(this->tcp);
    _mstream.setVersion(QDataStream::Qt_5_0);
    _mstream << qint8(this->_t) << qint64(package.toByteArray().length()) << package.toByteArray() << this->_fileSize << this->_fileName;

    if (_mstream.device()->waitForBytesWritten(10)) {
        connect(this->tcp, &QTcpSocket::bytesWritten, this, &FileUploadTask::onWriteBytes);
        _mstream.writeRawData(bytes.constData(), bytesLen);
    }
}

void FileUploadTask::onDisConnected()
{
    // 远程已关闭此连接，可能已完成
    disconnect(this->tcp, &QTcpSocket::bytesWritten, this, &FileUploadTask::onWriteBytes);
    emit onFinished();
}

void FileUploadTask::Connect()
{
    this->tcp->connectToHost(this->_ipAddress, this->_ipPort);
}

void FileUploadTask::onStartUpload()
{
    bool isopen = this->_mfile.open(QIODevice::ReadOnly);
    if (this->_m == FileTransferTask::Master) goto _MasterMode;

    // 任务排队开始时，如果客户端连接已经关闭，任务结束
    if (this->tcp->state() == QAbstractSocket::SocketState::UnconnectedState) {
        emit onFinished();
        return;
    }

    if (isopen) {
        QByteArray bytes = _mfile.read(this->_readBlock);
        int bytesLen = bytes.size();
        _mstream.setDevice(this->tcp);
        _mstream.setVersion(QDataStream::Qt_5_0);
        _mstream << qint64(this->_fileSize) << this->_fileName;
        if (_mstream.device()->waitForBytesWritten(100)) {

            connect(tcp, &QTcpSocket::disconnected, this, &FileUploadTask::onDisConnected);
            connect(this->tcp, &QTcpSocket::bytesWritten, this, &FileUploadTask::onWriteBytes);
            _mstream.writeRawData(bytes.constData(), bytesLen);
        }
    }
    return;

    _MasterMode:
    if (isopen) Connect();
    else emit onFinished();
}

void FileUploadTask::onWriteBytes(qint64 bytes)
{
    this->_mtotalSize += bytes;

    FileTransferTask::showProgress(this->_t, this->_ipAddress, this->_ipPort, this->_fileName, this->_fileSize, this->_mtotalSize);
    QByteArray array = _mfile.read(_readBlock);
    if (this->_mtotalSize == this->_fileSize) {
        // 当写入数据达到文件大小时，任务结束
        this->tcp->flush();
        emit onFinished();
    }
    if (array.isEmpty()) {
        // 当无法再读取文件数据块时，停止再次发生数据写入函数
        return;
    }
    int size = array.size();
    int wl = _mstream.writeRawData(array.constData(), size);
}

