#include "filetransfertask.h"
#include "filetransfertaskmanager.h"

FileTransferTaskManager::FileTransferTaskManager(QObject *parent) : QThread(parent)
{

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
//    connect(taskThread, &FileTransferTask::)
    mTaskQueue.append(taskThread);
}

void FileTransferTaskManager::doStart()
{
    this->start();
}

int FileTransferTaskManager::count()
{
    return this->mTaskQueue.count();
}

void FileTransferTaskManager::onTaskQueueItemFinish(FileTransferTask *s)
{
    taskfinishMutex.lock();
    mTasks.removeOne(s);
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
        }
    }
    taskMutex.lock();
    currentTaskRuning = false;
    taskMutex.unlock();
}
