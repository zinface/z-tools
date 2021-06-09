#include "filetransfertask.h"
#include "filetransfertaskmanager.h"

#include <filedownloadtask.h>

FileTransferTaskManager::FileTransferTaskManager(QObject *parent) : QThread(parent)
  , taskTogetherRuns(5)
  , currentTaskRuning(false)
  , taskPoolIsLock(false)
{
    connect(this, &FileTransferTaskManager::onTaskThreadChanged, this, [=] {
        if (taskPoolIsLock) {
            taskPoolIsLock = false;
            taskPoolIsFull.unlock();
        }
    });
}

FileTransferTaskManager::~FileTransferTaskManager()
{

}

void FileTransferTaskManager::setMaxTaskToggether(int count)
{
    this->taskTogetherRuns = count;
}

void FileTransferTaskManager::addFileTask(FileTransferTask *taskThread)
{
    connect(taskThread, &FileTransferTask::onFinished, this, &FileTransferTaskManager::onTaskQueueItemFinish);
    mTaskQueue.append(taskThread);
}

void FileTransferTaskManager::doStart()
{
    if (!currentTaskRuning)this->start();
}

int FileTransferTaskManager::count()
{
    return this->mTaskQueue.count();
}

void FileTransferTaskManager::onTaskQueueItemFinish(FileTransferTask *s)
{
    taskfinishMutex.lock();
    mTasks.removeOne(s);
    s->quit();
    s->wait();
    delete s;
    emit onTaskThreadChanged();
    taskfinishMutex.unlock();
}

void FileTransferTaskManager::run()
{
    if (currentTaskRuning) {
        return;
    }
    taskMutex.lock();
    currentTaskRuning = true;
    taskMutex.unlock();
    while (!mTaskQueue.isEmpty()) {
        if (mTasks.count() < taskTogetherRuns) {
            FileTransferTask *localTakeFirst = mTaskQueue.takeFirst();
            mTasks.append(localTakeFirst);
            localTakeFirst->start();
        } else {
            taskPoolIsFull.lock();
            taskPoolIsLock = true;
            if (this->mTasks.count() == 0 && taskPoolIsLock) {
                taskPoolIsLock = false;
                taskPoolIsFull.unlock();
            }

        }
    }
    taskMutex.lock();
    currentTaskRuning = false;
    taskMutex.unlock();
}
