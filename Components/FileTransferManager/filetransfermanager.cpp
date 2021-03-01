#include "filetransfermanager.h"

#include <QAbstractSocket>
#include <QApplication>
#include <QGroupBox>
#include <QMessageBox>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTextEdit>
#include <QVBoxLayout>

FileTransferManager::FileTransferManager(QWidget *parent) : QWidget(parent)
    ,fileSendBox(new QGroupBox("Send:"))
    ,fileRecevieBox(new QGroupBox("Receive:"))
    ,fileTransferBox(new QGroupBox("FileTranfer"))
    ,sendComb(new QComboBox)
    ,sendText(new QTextEdit)
    ,receiveText(new QTextEdit)
    ,clients(new QLabel("clients: 0"))
{
    createFileTransferManager();

    tss.listen(QHostAddress::LocalHost, 9999);
    tsc.connectToHost(QHostAddress::LocalHost, 9999);

    connect(&tsc, &QTcpSocket::readyRead, this, &FileTransferManager::onReadRead);
    connect(&tsc, &QTcpSocket::disconnected, this, &FileTransferManager::ondisConnect);
    connect(&tsc, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));

    connect(&tss, &QTcpServer::newConnection, this, &FileTransferManager::onNewConnection);
    connect(sendText, &QTextEdit::textChanged, this, &FileTransferManager::onSendChanged);

    setFixedSize(500,480);
    setWindowTitle(QApplication::applicationName() + QStringLiteral("文件传输管理"));
}

FileTransferManager::~FileTransferManager()
{
    smap.clear();
    tss.close();
    tsc.disconnect();
    tsc.close();
}

void FileTransferManager::onSendChanged()
{
    QString content = sendText->toPlainText();

    for (QString key : smap.keys()) {
        smap[key]->write(content.toUtf8());
    }
}

void FileTransferManager::onNewConnection()
{
    QTcpSocket *tmp = tss.nextPendingConnection();
    connect(tmp, &QTcpSocket::disconnected, this, &FileTransferManager::ondisConnect);
//    connect(tmp, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
    smap.insert(QString::number(count++),tmp);
    receiveText->setText(QString::number(tmp->localPort()));
    clients->setText(QString("clients: %1").arg(QString::number(smap.keys().length())));
}

void FileTransferManager::ondisConnect()
{
    receiveText->setText("client disconnect");
    for (QString key : smap.keys()) {
        QTcpSocket *tmp = smap[key];
        if (tmp->state() == QAbstractSocket::UnconnectedState) {
            smap.remove(key);
            tmp->deleteLater();
        }
    }
    clients->setText(QString("clients: %1").arg(QString::number(smap.keys().length())));
}

void FileTransferManager::onReadRead()
{
    receiveText->setText(QString(tsc.readAll()));
}

void FileTransferManager::onSocketError(QAbstractSocket::SocketError error)
{
    switch (error) {
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_cSocket->errorString()));
        qDebug() << __FUNCTION__ << "QAbstractSocket::ConnectionRefusedError";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::RemoteHostClosedError";
//        ui->plainTextEditLog->appendPlainText(QStringLiteral("文件传输终止！"));
//        reset();
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_cSocket->errorString()));
        qDebug() << __FUNCTION__ << "QAbstractSocket::HostNotFoundError";
        break;
    case QAbstractSocket::SocketTimeoutError:
        QMessageBox::critical(this, QStringLiteral("错误"), QStringLiteral("%1").arg(m_cSocket->errorString()));
        qDebug() << __FUNCTION__ << "QAbstractSocket::SocketTimeoutError";
        break;
    case QAbstractSocket::AddressInUseError:
        qDebug() << __FUNCTION__ << "QAbstractSocket::AddressInUseError";
        break;
    default:
        break;
    }
}

void FileTransferManager::createFileTransferManager()
{
    QVBoxLayout *sendLayout = new QVBoxLayout;
    QVBoxLayout *receiveLayout = new QVBoxLayout;

    sendLayout->addWidget(sendText);
    receiveLayout->addWidget(receiveText);

    fileSendBox->setLayout(sendLayout);
    fileRecevieBox->setLayout(receiveLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(fileSendBox);
    mainLayout->addWidget(fileRecevieBox);
    mainLayout->addWidget(clients);

    setLayout(mainLayout);
}
