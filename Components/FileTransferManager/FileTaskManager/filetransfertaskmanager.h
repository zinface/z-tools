#ifndef FILETRANSFERTASKMANAGER_H
#define FILETRANSFERTASKMANAGER_H

#include "filetransfertask.h"

#include <QObject>
#include <QQueue>
#include <QMutex>

class FileTransferTaskManager : public QThread
{
    Q_OBJECT
public:
    explicit FileTransferTaskManager(QObject *parent = nullptr);
    ~FileTransferTaskManager();

public:
    void setMaxTaskToggether(int count = 5);
    void addFileTask(FileTransferTask *taskThread);

    void doStart();
    int count();

signals:
    void onTaskThreadChanged();

public slots:
    void onTaskQueueItemFinish(FileTransferTask *s);

private:
    int taskTogetherRuns = 5;
    bool currentTaskRuning = false;
    QMutex taskfinishMutex;
    QMutex taskMutex;
    QQueue<FileTransferTask *> mTaskQueue;
    QList<FileTransferTask *> mTasks;


    // QThread interface
protected:
    void run();
};

#endif // FILETRANSFERTASKMANAGER_H
