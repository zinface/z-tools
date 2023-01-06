#ifndef SCANWORKERPOOL_H
#define SCANWORKERPOOL_H

#include "scanworkerthread.h"

#include <QObject>
#include <QQueue>

class ScanWorkerPool : public QThread
{
    Q_OBJECT
public:
    explicit ScanWorkerPool(QObject *parent = nullptr);

public:
    void setMaxTaskTogether(int count = 5);

    void addThreadTask(ScanWorkerThread *thread);

    int count();

    void doStart();

signals:
    void onTaskThreadChanged();

public slots:
    void onTaskQueueItemFinish(ScanWorkerThread *s);

private:
    int taskTogetherRuns = 5;
    bool currentTaskRunning = false;
    QMutex taskfinishMutex;
    QMutex taskMutex;
    QQueue<ScanWorkerThread*> taskQueue;
    QList<ScanWorkerThread*> tasks;

    QMutex taskPoolIsFull;
    bool taskPoolIsLock = false;

    // QThread interface
protected:
    void run();
};

#endif // SCANWORKERPOOL_H