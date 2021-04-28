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

    /**
     * 设置任务参数
     * @brief ScanWorkerThread::setWorkParam
     * @param server IP地址
     * @param port 端口号
     */
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
    void onConnectedHostIP(QString result);
    void onConnected(QString host, int port);
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
