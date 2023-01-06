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
    ScanWorkerThread(QString server, int port);
    ScanWorkerThread(QString server, QList<int> ports);
    ~ScanWorkerThread();

    void setWorkParam(QString server, int port);
    void setWorkParam(QString server, QList<int> ports);

public:
    int status();

    enum STATUS {
        NOT_FINISHED = 0,
        FINISHED,
    };

    STATUS state = NOT_FINISHED;


signals:
    void onConnected(QString result);
    void onFinished(ScanWorkerThread *s);

private:
    QMutex mutex;
    QString host;
    QList<int> ports;

    // QThread interface
protected:
    void run();
};

#endif // SCANWORKERTHREAD_H
