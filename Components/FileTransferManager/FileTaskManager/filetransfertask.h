#ifndef FILETRANSFERTASK_H
#define FILETRANSFERTASK_H

#include <QDataStream>
#include <QFile>
#include <QTcpSocket>
#include <QThread>
#include <QMutex>

class FileTransferTask : public QThread
{
    Q_OBJECT
public:
    explicit FileTransferTask();
    ~FileTransferTask();

    enum TASK_STATUS {
        NOT_FINISHED = 0,
        FINISHED,
    };

    TASK_STATUS state = NOT_FINISHED;

    enum TaskType {
        OTHER = 1,
        UPLOAD,
        DOWNLOAD,
    };

    void setTaskParam(TaskType t);

    // UPLOAD -> DOWNLOAD
    // Type, QTcpSocket, FileName, FileSize, FileMD5
    void setTaskParam(TaskType t, QTcpSocket *c, QString fileName, qint64 fileSize, QString filePath);

    // DOWNLOAD -> UPLOAD
    // Type, IP, FileName, FileSize, FizeStartSize, FileMD5
    void setTaskParam(TaskType t, QString ipAddres, int ipPort, QString fileName, qint64 fileSize, QString fileSavePath);

    TaskType taskType() {
        return this->_t;
    }

public slots:
    void onStartUpload();
    void finishUpload();
    void onStartDownload();
    void finishDownload();

signals:
    void startUpload();
    void startDownload();

    void onFinished(FileTransferTask *s);

private:
    TaskType _t;

    // QThread interface
protected:
    void run();
};

#endif // FILETRANSFERTASK_H
