#include "scanworkerpool.h"

ScanWorkerPool::ScanWorkerPool(QObject *parent) : QThread(parent)
    , taskPoolIsLock(false)
{
    connect(this, &ScanWorkerPool::onTaskThreadChanged, this, [=] {
        if (taskPoolIsLock) {
            taskPoolIsLock = false;
            taskPoolIsFull.unlock();
        }
    });
}

void ScanWorkerPool::setMaxTaskTogether(int count) {
    this->taskTogetherRuns = count;
}

void ScanWorkerPool::addThreadTask(ScanWorkerThread *thread) {
    connect(thread, &ScanWorkerThread::onFinished, this, &ScanWorkerPool::onTaskQueueItemFinish);
    taskQueue.append(thread);
}

int ScanWorkerPool::count() {
    return this->taskQueue.count();
}

void ScanWorkerPool::doStart(){
    this->start();
}

void ScanWorkerPool::onTaskQueueItemFinish(ScanWorkerThread *s){
    taskfinishMutex.lock();
    tasks.removeOne(s);
    s->quit();
    delete s;
    taskfinishMutex.unlock();
    emit onTaskThreadChanged();
}

void ScanWorkerPool::run() {
    if(currentTaskRunning){
        return;
    }
    taskMutex.lock();
    currentTaskRunning = true;
    taskMutex.unlock();
    while (!taskQueue.isEmpty()) {
        if (tasks.count() < taskTogetherRuns) {
            ScanWorkerThread *localTakeFirst = taskQueue.takeFirst();
            tasks.append(localTakeFirst);
            localTakeFirst->start();
        }  else {
            taskPoolIsFull.lock();
            taskPoolIsLock = true;
        }
    }
    taskMutex.lock();
    currentTaskRunning = false;
    taskMutex.unlock();
}
