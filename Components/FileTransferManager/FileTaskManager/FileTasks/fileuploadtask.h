#ifndef FILEUPLOADTASK_H
#define FILEUPLOADTASK_H

#include <QObject>
#include <filetransfertask.h>

class FileUploadTask : public QObject
{
    Q_OBJECT
public:
//    explicit FileUploadTask(QObject *parent = nullptr);
    explicit FileUploadTask(QObject *parent = nullptr, FileTransferTask::TaskRole mode = FileTransferTask::Slave);
    ~FileUploadTask();

    // UPLOAD -> DOWNLOAD
    // Type, QTcpSocket, FileName, FileSize, FileMD5
    // Slave
    void setTaskParam(FileTransferTask::TaskType t, QTcpSocket *c, QString fileName, qint64 fileSize, QString filePath);
    // Master
    void setTaskParam(FileTransferTask::TaskType t, QString ipAddres, int ipPort, QString fileName, qint64 fileSize, QString filePath);

public slots:
    void onConnected();
    void onDisConnected();
    void Connect();
    void onStartUpload();

private slots:
    void onWriteBytes(qint64 bytes);

signals:
    void onFinished();

private:
    qint64 _readBlock = 4096;
    FileTransferTask::TaskType _t;
    FileTransferTask::TaskRole _m;
    FileTransferTask::TASK_STATUS state;

    QString _ipAddress;
    int _ipPort = 0;

    QString _fileName;
    QString _filePath;
    QString _fileSavePath;
    qint64 _fileSize;

    QFile _mfile;
    QDataStream _mstream;

    qint64 _mtotalSize;

private:
    QTcpSocket *tcp;
};

#endif // FILEUPLOADTASK_H
