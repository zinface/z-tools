#ifndef FILETRANSFERTASK_H
#define FILETRANSFERTASK_H

#include <QDataStream>
#include <QFile>
#include <QTcpSocket>
#include <QThread>

class FileTransferTask : public QThread
{
    Q_OBJECT
public:
    explicit FileTransferTask();
    ~FileTransferTask();
//    explicit FileTransferTask(QTcpSocket *c, QString filename, qint64 filesize);
//    explicit FileTransferTask(TaskType t = OTHER, QTcpSocket *c, QString filename, qint64 filesize);
//    explicit FileTransferTask(TaskType t = OTHER, QTcpSocket *c, QString filename, qint64 filesize);

    enum TaskType {
        OTHER = 1,
        UPLOAD,
        DOWNLOAD,
    };

    // UPLOAD -> DOWNLOAD
    // Type, QTcpSocket, FileName, FileSize, FileMD5
//    void setTaskParam(TaskType t, QTcpSocket *c, QString fileName, QString filePath, qint64 fileSize);
    void setTaskParam(TaskType t, QTcpSocket *c, QString fileName, qint64 fileSize, QString filePath);


    // DOWNLOAD -> UPLOAD
    // Type, IP, FileName, FileSize, FizeStartSize, FileMD5
    void setTaskParam(TaskType t, QString ipAddres, int ipPort, QString fileName, qint64 fileSize, QString fileSavePath);

private slots:
    void onConnected();
    void onReadyRead();
    void onWriteBytes(qint64 bytes);

signals:
    void onFinished(FileTransferTask *s);
    void onTotalWriteBytes(qint64 bytes);

private:
    qint64 _readBlock = 4096;
    // Task Parameters
    TaskType _t;
    QTcpSocket *_c;
    QString _ipAddress;
    int _ipPort = 0;
    QString _fileName;
    QString _filePath;
    QString _fileSavePath;
    qint64 _fileSize;

    // this buffer
    QDataStream _mstream;
    QString _mfileName;
    qint64 _mfileSize;

    QTcpSocket _msocket;
    QFile _mfile;

    // Recording data
    qint64 _mtotalSize;

    // QThread interface
protected:
    void run();
};

#endif // FILETRANSFERTASK_H
