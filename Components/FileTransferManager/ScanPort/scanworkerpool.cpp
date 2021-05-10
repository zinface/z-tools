#include "scanworkerpool.h"

ScanWorkerPool::ScanWorkerPool(QObject *parent) : QThread(parent)
  ,taskTogetherRuns(5)
  ,currentTaskRunning(false)
  ,waitQueuefinished(false)
{

}

void ScanWorkerPool::setMaxTaskTogether(int count) {
    this->taskTogetherRuns = count;
}

void ScanWorkerPool::setWaitForFinished(bool waitQueuefinished)
{
    this->waitQueuefinished = waitQueuefinished;
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
    delete s;
    emit onTaskThreadChanged();
    if (this->tasks.count() == 0) {
        this->waitQueuefinished = false;
    }
    taskfinishMutex.unlock();
}

void ScanWorkerPool::run() {
    if(currentTaskRunning){
        return;
    }
    taskMutex.lock();
    currentTaskRunning = true;
    taskMutex.unlock();
    while (!taskQueue.isEmpty() || this->waitQueuefinished) {
        if (tasks.count() < taskTogetherRuns && taskQueue.count() > 0) {
            ScanWorkerThread *localTakeFirst = taskQueue.takeFirst();
            tasks.append(localTakeFirst);
            localTakeFirst->start();
        }
    }
    taskMutex.lock();
    currentTaskRunning = false;
    taskMutex.unlock();
}
