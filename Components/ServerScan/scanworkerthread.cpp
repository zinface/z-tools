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
//    QTcpSocket tcp;
//    tcp.connectToHost(server, port);
//    bool connected = tcp.waitForConnected(50);
//    if (connected) {
//        mutex.lock();
//// m_model.setStringList(m_model.stringList() << _server+":"+QString::number(_port));
//        onConnected(server+":"+QString::number(port));
//        mutex.unlock();
    //    }
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
            emit onConnected(host+":"+QString::number(port));
            mutex.unlock();
        }
    }
    this->state = FINISHED;
    emit onFinished(this);
}
