#include "scanworkerthread.h"

#include <QTcpSocket>

ScanWorkerThread::ScanWorkerThread()
{

}

ScanWorkerThread::ScanWorkerThread(QString server, int port)
{
    setWorkParam(server, port);
}

ScanWorkerThread::ScanWorkerThread(QString server, QList<int> ports)
{
    setWorkParam(server, ports);
}

ScanWorkerThread::~ScanWorkerThread()
{
    this->wait();
}

void ScanWorkerThread::setWorkParam(QString server, int port)
{
    this->host = server;
    this->ports.append(port);
}

void ScanWorkerThread::setWorkParam(QString server, QList<int> ports)
{
    this->host = server;
    this->ports.append(ports);
}

int ScanWorkerThread::status()
{
    return this->state;
}

void ScanWorkerThread::run()
{
    foreach (int port, ports) {
        QTcpSocket tcp;
        tcp.connectToHost(host, port);
        bool connected = tcp.waitForConnected(1000);
        if (connected) {
            mutex.lock();
            emit onConnectedHostIP(host+":"+QString::number(port));
            emit onConnected(host, port);
            mutex.unlock();
        }
    }
    this->state = FINISHED;
    emit onFinished(this);
}
