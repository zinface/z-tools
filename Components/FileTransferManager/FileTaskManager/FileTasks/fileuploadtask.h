#ifndef FILEUPLOADTASK_H
#define FILEUPLOADTASK_H

#include <QObject>
#include <filetransfertask.h>

class FileUploadTask : public QObject
{
    Q_OBJECT
public:
    explicit FileUploadTask(QObject *parent = nullptr);
    ~FileUploadTask();
    void setTaskParam(FileTransferTask::TaskType t, QTcpSocket *c, QString fileName, qint64 fileSize, QString filePath);

public slots:
    void onStartUpload();

private slots:
    void onWriteBytes(qint64 bytes);

signals:
    void onFinished();

private:
    qint64 _readBlock = 4096;
    FileTransferTask::TaskType _t;
    FileTransferTask::TASK_STATUS state;

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
