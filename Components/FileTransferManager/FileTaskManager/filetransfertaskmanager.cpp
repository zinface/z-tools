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
//    QTextStream(stdout) << QString("任务单元结束: 正在运行：%1，已停止：%2，已完成：%3 \n")
//                           .arg(s->isRunning()?"yes":"no")
//                           .arg(s->isFinished()?"yes":"no")
//                           .arg(s->state == FileTransferTask::TASK_STATUS::FINISHED?"yes":"no");

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
