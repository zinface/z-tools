#include "loadermanager.h"
#include "loaderworker.h"

LoaderManager::LoaderManager(QObject *parent)
    : QObject{parent}
    , m_workStop(false)
{

}

void LoaderManager::start()
{
    stop();
    LoaderWorker *worker = new LoaderWorker(this);
    worker->moveToThread(&m_thread);
    connect(&m_thread, &QThread::finished, worker, &LoaderWorker::deleteLater);

    connect(this, &LoaderManager::ping, worker, &LoaderWorker::slotPing);
    //ManagerConnectWorker
    connect(this, &LoaderManager::requestThumbnail, worker, &LoaderWorker::slotThumbnail);
    connect(this, &LoaderManager::requestIndexThumbnail, worker, &LoaderWorker::slotIndexThumbnail);

    m_thread.start();
}

void LoaderManager::stop()
{
    if (m_thread.isRunning()) {
        m_thread.quit();
        m_thread.wait();
    }
}

bool LoaderManager::workStop() const
{
    return m_workStop;
}

void LoaderManager::setWorkStop(bool newWorkStop)
{
    m_workStop = newWorkStop;
}
