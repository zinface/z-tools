#include "scanworkerthread.h"

#include <QTcpSocket>

ScanWorkerThread::ScanWorkerThread()
{

}

ScanWorkerThread::~ScanWorkerThread()
{

}

ScanWorkerThread::ScanWorkerThread(QString server, int port)
{
    this->host = server;
    this->port = port;
}

void ScanWorkerThread::setWorkParam(QString server, int port)
{
    this->host = server;
    this->port = port;
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

void ScanWorkerThread::run()
{
    QTcpSocket tcp;
    tcp.connectToHost(host, port);
    bool connected = tcp.waitForConnected(5000);
    if (connected) {
        mutex.lock();
        onConnected(host+":"+QString::number(port));
        mutex.unlock();
    }
}
