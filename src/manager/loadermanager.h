#ifndef LOADERMANAGER_H
#define LOADERMANAGER_H

#include <QListWidgetItem>
#include <QObject>
#include <QThread>

class LoaderManager : public QObject
{
    Q_OBJECT
public:
    explicit LoaderManager(QObject *parent = nullptr);

    void start();
    void stop();

    bool workStop() const;
    void setWorkStop(bool newWorkStop);

signals:
    /**
        void slotPong(QString str);
    */
    void ping(QString str);
    void pong(QString str);

    //ManagerSignals
    void requestThumbnail(const QString &current, const QString &filepath);
    void Thumbnail(const QString &current, const QPixmap &pixmap);

    void requestIndexThumbnail(int i, const QString &current, const QString &filepath);
    void IndexThumbnail(int i, const QString &current, const QPixmap &pixmap);

private:
    QThread m_thread;
    bool m_workStop;
};

#endif // LOADERMANAGER_H
