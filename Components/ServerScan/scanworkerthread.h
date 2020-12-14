#ifndef SCANWORKERTHREAD_H
#define SCANWORKERTHREAD_H

#include <QMutex>
#include <QObject>
#include <QThread>
#include <QWidget>

class ScanWorkerThread : public QThread
{
    Q_OBJECT
public:
    ScanWorkerThread();
    ~ScanWorkerThread();
    ScanWorkerThread(QString server, int port);
    void setWorkParam(QString server, int port);

signals:
    void onConnected(QString result);

private:
    QMutex mutex;
    QString host;
    int port;

    // QThread interface
protected:
    void run();
};

#endif // SCANWORKERTHREAD_H
