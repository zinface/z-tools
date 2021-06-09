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
    enum TASK_STATUS {
        NOT_FINISHED = 0,
        FINISHED,
    };

    enum TaskType {
        OTHER = 1,
        UPLOAD,
        DOWNLOAD,
    };

    enum TaskRole {
        Master,
        Slave
    };

public:
    explicit FileTransferTask(TaskType t = OTHER);
    ~FileTransferTask();

    TaskType taskType() {
        return this->_t;
    }

    TASK_STATUS state = NOT_FINISHED;

    static void showProgress(TaskType type, QString ipaddress, int port, QString filename, qint64 filesize, qint64 receiversize);

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
