#include "scanworkerpool.h"

ScanWorkerPool::ScanWorkerPool(QObject *parent) : QThread(parent)
  ,taskTogetherRuns(5)
  ,currentTaskRunning(false)
{

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
    delete s;
    emit onTaskThreadChanged();
    taskfinishMutex.unlock();
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
        }
    }
    taskMutex.lock();
    currentTaskRunning = false;
    taskMutex.unlock();
}
