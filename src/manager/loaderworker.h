#ifndef LOADERWORKER_H
#define LOADERWORKER_H

#include <QListWidgetItem>
#include <QObject>

class LoaderManager;
class LoaderWorker : public QObject
{
    Q_OBJECT
public:
    explicit LoaderWorker(LoaderManager *parent = nullptr);

public slots:
    void slotPing(QString str);
    void slotThumbnail(const QString &current, const QString &filepath);
    void slotIndexThumbnail(int i, const QString &current, const QString &filepath);

    //WorkerSlots

private:
    LoaderManager *manager;

};

#endif // LOADERWORKER_H
