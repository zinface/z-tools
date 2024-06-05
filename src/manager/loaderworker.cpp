#include "loadermanager.h"
#include "loaderworker.h"

LoaderWorker::LoaderWorker(LoaderManager *parent)
    : QObject{}
    , manager(parent)
{

}

void LoaderWorker::slotPing(QString str)
{
    emit manager->pong("woker: " + str);
}

void LoaderWorker::slotThumbnail(const QString &current, const QString &filepath)
{
    emit manager->Thumbnail(current, QPixmap(filepath).scaled(QSize(50, 50)));
}

void LoaderWorker::slotIndexThumbnail(int i, const QString &current, const QString &filepath)
{
    if (manager->workStop())
        return;
    emit manager->IndexThumbnail(i, current, QPixmap(filepath).scaled(QSize(50, 50)));
}
