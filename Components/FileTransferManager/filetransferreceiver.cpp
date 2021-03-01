#include "filetransferreceiver.h"

#include <QGroupBox>
#include <QHostAddress>
#include <QLineEdit>
#include <QVBoxLayout>

FileTransferReceiver::FileTransferReceiver(QWidget *parent) : QWidget(parent)
  ,statusBar(new QLabel("服务器待连接."))
  ,clientStatus(new QLabel("服务器断开连接!"))
  ,listWidget(new QListWidget)
  ,remoteEdit(new QLineEdit)
  ,connectBtn(new QPushButton("连接"))
  ,saveFileBtn(new QPushButton("保存"))
{
    createFileTransferReceiver();

    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    remoteEdit->setValidator(new QRegExpValidator(rx,this));

    connect(connectBtn, &QPushButton::clicked, this, &FileTransferReceiver::onConnect);
}

void FileTransferReceiver::onConnect()
{
    QString address = remoteEdit->text();

    if (address.isEmpty()) {
        address = "127.0.0.1";
    }

    QHostAddress hostAddress;
    if (!hostAddress.setAddress(address)) {
        statusBar->setText("目标网络地址错误!");
        return;
    } else {
        statusBar->setText(QString("目标地址已设置 - %1!").arg(hostAddress.toString()));
    }

    tcpSocket.connectToHost(hostAddress, 8888);
    connect(&tcpSocket, &QTcpSocket::connected, this, &FileTransferReceiver::connected);
    connect(&tcpSocket, &QTcpSocket::disconnected, this, &FileTransferReceiver::disconnected);
}

void FileTransferReceiver::connected()
{
    clientStatus->setText("服务器连接成功!");
}

void FileTransferReceiver::disconnected()
{
    clientStatus->setText("服务器断开连接!");
}

void FileTransferReceiver::createFileTransferReceiver()
{
    QGroupBox *receiverBox = new QGroupBox("File Receiver");
    QHBoxLayout *remoteCtlsLayout = new QHBoxLayout;
    QVBoxLayout *receiverBoxLayout = new QVBoxLayout;

    remoteCtlsLayout->addWidget(remoteEdit, 2);
    remoteCtlsLayout->addWidget(connectBtn, 1);
    remoteCtlsLayout->addWidget(saveFileBtn, 1);

    receiverBoxLayout->addWidget(listWidget);

    receiverBox->setLayout(receiverBoxLayout);

    remoteEdit->setPlaceholderText("127.0.0.1");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(remoteCtlsLayout);
    mainLayout->addWidget(receiverBox);
    mainLayout->addWidget(statusBar);
    mainLayout->addWidget(clientStatus);
    setLayout(mainLayout);
}
