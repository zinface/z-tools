#include "scanworkerthread.h"

#include <QFile>
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

QString ScanWorkerThread::getProtocal(int port)
{
    QString protocal("[tcp/*]");
    QFile f("/etc/services");
    if (f.exists() && f.open(QIODevice::ReadOnly)) {
        char buffer[100];
        int len = 0;
        bool found = false;
        while(!found && (len = f.readLine(buffer, 100))>0) {
            QStringList localSplit = QString(buffer).split(QRegExp("\\s{1,}"));
            if (localSplit.length()>=2 && QString("%1/tcp").arg(port).compare(localSplit.at(1))==0) {
                protocal = QString("[tcp/%1]").arg(localSplit.at(0));
                found = true;
            }
        }
        f.close();
    }
    return protocal;
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
            emit onConnected(host, port, getProtocal(port));
            mutex.unlock();
        }
    }
    this->state = FINISHED;
    emit onFinished(this);
}
