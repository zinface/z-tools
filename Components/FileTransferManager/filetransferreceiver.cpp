#include "filetransferreceiver.h"

#include <QFileDialog>
#include <QGroupBox>
#include <QHostAddress>
#include <QLineEdit>
#include <QVBoxLayout>
#include <qmessagebox.h>

FileTransferReceiver::FileTransferReceiver(QWidget *parent) : QWidget(parent)
  ,statusBar(new QLabel("服务器待连接."))
  ,clientStatus(new QLabel("服务器断开连接!"))
  ,m_fileSize(0)
  ,m_fileBytesReceivedSize(0)
  ,listWidget(new QListWidget)
  ,remoteEdit(new QLineEdit)
  ,connectBtn(new QPushButton("连接"))
  ,saveFileBtn(new QPushButton("保存位置"))
{
    createFileTransferReceiver();

    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    remoteEdit->setValidator(new QRegExpValidator(rx,this));

    connect(connectBtn, &QPushButton::clicked, this, &FileTransferReceiver::onConnect);
    connect(saveFileBtn, &QPushButton::clicked, this, &FileTransferReceiver::onConfigFileSavePath);

    setFixedSize(500,500);
}

void FileTransferReceiver::onConfigFileSavePath()
{
    QString srcDirPath = QFileDialog::getExistingDirectory(this, "选择文件夹", QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!srcDirPath.isEmpty()) {
        savePath = srcDirPath;
    }
    onUpdateFileSavePath();
}

void FileTransferReceiver::onUpdateFileSavePath()
{
    switch (int(tcpSocket.state())) {
    case QTcpSocket::ConnectedState:
        clientStatus->setText(QString("服务器连接成功! - 存储地址为：%1").arg(savePath));
        break;
    case QTcpSocket::UnconnectedState:
        clientStatus->setText(QString("服务器断开连接! - 存储地址为：%1").arg(savePath));
    }
}

void FileTransferReceiver::onConnect()
{
    if (savePath.isEmpty()){
        statusBar->setText("请设置存储目录!");
        return;
    }

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
    connect(&tcpSocket, &QTcpSocket::readyRead, this, &FileTransferReceiver::onReadyRead);
    connect(&tcpSocket, &QTcpSocket::disconnected, this, &FileTransferReceiver::disconnected);
}

void FileTransferReceiver::connected()
{
    m_stream.setDevice(&tcpSocket);
    m_stream.setVersion(QDataStream::Qt_5_0);
    onUpdateFileSavePath();
}

void FileTransferReceiver::disconnected()
{
    m_stream.setDevice(nullptr);
    m_stream.setVersion(QDataStream::Qt_5_0);
    onUpdateFileSavePath();
}

void FileTransferReceiver::onReadyRead()
{
    if (0 == m_fileSize && tcpSocket.bytesAvailable() > sizeof(qint64)) {
        m_stream >> m_fileSize >> m_fileName;
        m_file.setFileName(QDir(savePath).absoluteFilePath(m_fileName));
        if (m_file.exists() && m_file.size() == m_fileSize) {
            m_file.close();
            return;
        }
        if (!m_file.open(QIODevice::WriteOnly)) {
            return;
        }
        listWidget->addItem(m_file.fileName());
    } else {
        qint64 size = qMin(tcpSocket.bytesAvailable(), m_fileSize - m_fileBytesReceivedSize);
        QByteArray array(size, 0);
        m_stream.readRawData(array.data(), size);
        if (m_file.isOpen()) {
            m_file.write(array);
        }
        m_fileBytesReceivedSize += size;

        if (m_fileBytesReceivedSize == m_fileSize) {
            m_file.close();
            m_fileName.clear();
            m_fileSize = 0;
            m_fileBytesReceivedSize = 0;
        }
    }
}

void FileTransferReceiver::createFileTransferReceiver()
{
    QGroupBox *receiverBox = new QGroupBox("File Receiver");
    QHBoxLayout *remoteCtlsLayout = new QHBoxLayout;
    QVBoxLayout *receiverBoxLayout = new QVBoxLayout;

    remoteCtlsLayout->addWidget(remoteEdit, 2);
    remoteCtlsLayout->addWidget(connectBtn, 1);
    remoteCtlsLayout->addWidget(saveFileBtn, 1);

    receiverBoxLayout->addLayout(remoteCtlsLayout);
    receiverBoxLayout->addWidget(listWidget);

    receiverBox->setLayout(receiverBoxLayout);

    remoteEdit->setPlaceholderText("127.0.0.1");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(receiverBox);
    mainLayout->addWidget(statusBar);
    mainLayout->addWidget(clientStatus);
    setLayout(mainLayout);
}
