#ifndef FILEDOWNLOADTASK_H
#define FILEDOWNLOADTASK_H

#include <QDir>
#include <QObject>
#include <QTimer>
#include <filetransfertask.h>

class FileDownloadTask : public QObject
{
    Q_OBJECT
public:
    explicit FileDownloadTask(QObject *parent = nullptr);
    ~FileDownloadTask();
    void setTaskParam(FileTransferTask::TaskType t, QString ipAddres, int ipPort, QString fileName, qint64 fileSize, QString fileSavePath);

public slots:
    void onReadyRead();;
    void onConnected();;
    void Connect();;

signals:
    void onFinished();
    void onTotalWriteBytes(qint64 size);

private:
    FileTransferTask::TaskType _t;
    FileTransferTask::TASK_STATUS state;

    QString _ipAddress;
    int _ipPort = 0;
    QString _fileName;
    QString _filePath;
    QString _fileSavePath;
    qint64 _fileSize;


    QDataStream _mstream;
    QString _mfileName;
    qint64 _mfileSize;
    QFile _mfile;
    qint64 _mtotalSize;

private:
    QTcpSocket *tcp;
};

#endif // FILEDOWNLOADTASK_H
