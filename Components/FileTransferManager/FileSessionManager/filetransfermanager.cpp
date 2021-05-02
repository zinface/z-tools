#include "sessionmanager.h"
#include "filetransfermanager.h"

#include <QAbstractSocket>
#include <QApplication>
#include <QGroupBox>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>

FileTransferManager::FileTransferManager(QObject *parent) : QObject(parent)
  , adapter(new SessionManager)
  ,_manager_work(SessionManager::SERVER)
{
    connect(adapter, &SessionManager::newConnectSocket, this, &FileTransferManager::newConnectSocket);
    connect(adapter, &SessionManager::clientCountChanged, this, &FileTransferManager::clientCountChanged);
    connect(adapter, &SessionManager::newAction, this, &FileTransferManager::onNewAction);

    connect(adapter, &SessionManager::connected, this, &FileTransferManager::connected);
    connect(adapter, &SessionManager::readRead, this, &FileTransferManager::readRead);
    connect(adapter, &SessionManager::disconnected, this, &FileTransferManager::disconnected);
}

FileTransferManager::~FileTransferManager()
{

}

void FileTransferManager::setManagerTask(QString host, int port, SessionManager::SessionManagerWorkType type)
{
    adapter->SettingHost(host, port, type);
    this->_manager_work = type;
}

bool FileTransferManager::state()
{
    if (this->_manager_work == SessionManager::SERVER) {
        return adapter->serverState() == SessionManager::LISTENED;
    } else {
        return adapter->serverState() == SessionManager::CONNECTED;
    }
}

bool FileTransferManager::serverState()
{
    return this->adapter->serverState() == SessionManager::LISTENED;
}

bool FileTransferManager::clientState()
{
    return this->adapter->serverState();
}

void FileTransferManager::pushFileAppend(QString filename, qint64 filesize)
{
    broadCaseAction(nullptr, S_APPEND, filename, filesize);
}

void FileTransferManager::pushFileDeleted(QString filename)
{
    broadCaseAction(nullptr, S_DELETE, filename);
}


void FileTransferManager::pushFileClaer()
{
    broadCaseAction(nullptr, S_CLEANR, QString());
}

void FileTransferManager::fetchFileListAction()
{
    QDataStream stream(adapter->c());
    stream.setVersion(QDataStream::Qt_5_0);
    stream << qint8(OP_ALL) << QString();
}

void FileTransferManager::broadCaseAction(QTcpSocket *c, FullEvent e, QString filename)
{
    if (c == nullptr) {
        foreach(QString key, adapter->clients().keys()) {
            QTcpSocket *c = adapter->clients()[key];
            QDataStream stream(c);
            stream.setVersion(QDataStream::Qt_5_0);
            stream << qint8(e) << filename;
        }
    } else {
        QDataStream stream(c);
        stream.setVersion(QDataStream::Qt_5_0);
        stream << qint8(e) << filename;
    }
}

void FileTransferManager::broadCaseAction(QTcpSocket *c, FullEvent e, QString filename, qint64 filesize)
{
    if (c == nullptr) {
        foreach(QString key, adapter->clients().keys()) {
            QTcpSocket *c = adapter->clients()[key];
            QDataStream stream(c);
            stream.setVersion(QDataStream::Qt_5_0);
            stream << qint8(e) << filename << filesize;
        }
    } else {
        QDataStream stream(c);
        stream.setVersion(QDataStream::Qt_5_0);
        stream << qint8(e) << filename << filesize;
    }
}

void FileTransferManager::onNewAction(QTcpSocket *c)
{
    QDataStream stream(c);
    QTextStream(stdout) << QString("FileTransferManager: 接收：%1\n").arg(c->bytesAvailable());
    stream.setVersion(QDataStream::Qt_5_0);
    qint8 action;
    QString filename;
    QStringList filenames;
    qint64 filesize(0);

    stream >> action;
    switch (action) {

    case OP_ALL:
        emit onRemoteFetchFileList(c);
        QTextStream(stdout) << QString("FileTransferManager:指令：OP_ALL: ") << filename << filesize << "\n";
        break;

    case OP_DOWN:
        stream >> filename >> filesize;
        break;

    case S_APPEND:
        stream >> filename >> filesize;
        emit onRemoteFileAppend(filename, filesize);
        QTextStream(stdout) << QString("FileTransferManager:指令：S_APPEND: ") << filename << filesize << "\n";
        break;

    case S_DELETE:
        stream >> filename;
        emit onRemoteFileDelete(filename);
        QTextStream(stdout) << QString("FileTransferManager:指令：S_DELETE: ") << filename << "\n";
        break;

    case S_CLEANR:
        stream >> filename;
        emit onRemoteFileClear();
        QTextStream(stdout) << QString("FileTransferManager:指令：S_CLEANR: ") << "\n";
        break;
    }

    QTextStream(stdout) << QString("FileTransferManager:剩余尾部字节：") << c->bytesAvailable() << "\n";
    while(c->bytesAvailable() > 0) {
        onNewAction(c);
    }
}


/**
//void FileTransferManager::onSocketError(QAbstractSocket::SocketError error)
//{
//    switch (error) {
//    case QAbstractSocket::ConnectionRefusedError:
//        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_cSocket->errorString()));
//        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectionRefusedError";
//        break;
//    case QAbstractSocket::RemoteHostClosedError:
//        qDebug() << __FUNCTION__ << "QAbstractSocket::RemoteHostClosedError";
////        ui->plainTextEditLog->appendPlainText(QStringLiteral("文件传输终止！"));
////        reset();
//        break;
//    case QAbstractSocket::HostNotFoundError:
//        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_cSocket->errorString()));
//        qDebug() << __FUNCTION__ << "QAbstractSocket::HostNotFoundError";
//        break;
//    case QAbstractSocket::SocketTimeoutError:
//        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_cSocket->errorString()));
//        qDebug() << __FUNCTION__ << "QAbstractSocket::SocketTimeoutError";
//        break;
//    case QAbstractSocket::AddressInUseError:
//        qDebug() << __FUNCTION__ << "QAbstractSocket::AddressInUseError";
//        break;
//    default:
//        break;
//    }
//}
*/
