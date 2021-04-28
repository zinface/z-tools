#include "filetransferreceiver.h"
#include "filetransferrecivermanualhostdialog.h"

#include <QAction>
#include <QComboBox>
#include <QFileDialog>
#include <QGroupBox>
#include <QHostAddress>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMenu>
#include <QNetworkInterface>
#include <QProgressBar>
#include <QPushButton>
#include <QThread>
#include <QVBoxLayout>
#include <qmessagebox.h>
#include <scanworkerpool.h>

FileTransferReceiver::FileTransferReceiver(QWidget *parent) : QWidget(parent)
  ,statusBar(new QLabel("服务器待连接."))
  ,clientStatus(new QLabel("服务器断开连接!"))
  ,m_fileSize(0)
  ,m_fileBytesReceivedSize(0)
  ,listWidget(new QListWidget)
  ,remoteSwitchBox(new QComboBox)
  ,remoteEdit(new QLineEdit)
  ,connectBtn(new QPushButton("选中"))
  ,saveFileBtn(new QPushButton("保存位置"))
  ,scanProgressBar(new QProgressBar)
  ,pool(new ScanWorkerPool)
{
    createFileTransferReceiver();

    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    remoteEdit->setValidator(new QRegExpValidator(rx,this));

    connect(connectBtn, &QPushButton::clicked, this, &FileTransferReceiver::onUseRemoteSwitch);
    connect(saveFileBtn, &QPushButton::clicked, this, &FileTransferReceiver::onConfigFileSavePath);
    connect(pool, &QThread::finished, this, &FileTransferReceiver::onScanFinished);
    connect(pool, &ScanWorkerPool::onTaskThreadChanged, this, &FileTransferReceiver::onScanThreadChanged);

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

    QString address = remoteSwitchBox->currentData().toString();

    if (address.isEmpty() || address.length() == 0 || address == "") {
        return;
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
    tcpSocket.close();
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

void FileTransferReceiver::onUseRemoteSwitch()
{
    if (remoteSwitchBox->currentText() == "重新扫描") {
        scanLocalHost();
        return;
    } else if(remoteSwitchBox->currentText() == "手动输入") {
        FileTransferReciverManualHostDialog dialog;
        if (dialog.exec() != QDialog::Accepted) {
            return;
        } else {
            if (dialog.getHost().isEmpty()) return;
            remoteSwitchBox->insertItem(1, dialog.getHost(), dialog.getHost());
            remoteSwitchBox->setCurrentIndex(1);
        }
        return;
    }
    onConnect();
}

void FileTransferReceiver::onScanAvilable(QString host, int port)
{
    Q_UNUSED(port)
    remoteSwitchBox->addItem(host, host);
}

void FileTransferReceiver::onScanFinished()
{
    remoteSwitchBox->addItem("手动输入", "");
    remoteSwitchBox->addItem("重新扫描", "");
    scanProgressBar->setVisible(false);
}

void FileTransferReceiver::onScanThreadChanged()
{
    scanProgressBar->setValue(scanProgressBar->maximum() - pool->count());
}

void FileTransferReceiver::createFileTransferReceiver()
{
    QGroupBox *receiverBox = new QGroupBox("File Receiver");
    QHBoxLayout *remoteCtlsLayout = new QHBoxLayout;
    QVBoxLayout *receiverBoxLayout = new QVBoxLayout;

//    remoteCtlsLayout->addWidget(remoteEdit, 2);
    remoteCtlsLayout->addWidget(remoteSwitchBox, 2);
    remoteCtlsLayout->addWidget(connectBtn, 1);
    remoteCtlsLayout->addWidget(saveFileBtn, 1);

    receiverBoxLayout->addLayout(remoteCtlsLayout);
    receiverBoxLayout->addWidget(scanProgressBar);
    scanProgressBar->setVisible(false);
    receiverBoxLayout->addWidget(listWidget);

    receiverBox->setLayout(receiverBoxLayout);

    remoteEdit->setPlaceholderText("127.0.0.1");

    remoteSwitchBox->addItem("127.0.0.1(本机)", "127.0.0.1");
    remoteSwitchBox->addItem("手动输入", "");
    remoteSwitchBox->addItem("重新扫描", "");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(receiverBox);
    mainLayout->addWidget(statusBar);
    mainLayout->addWidget(clientStatus);
    setLayout(mainLayout);
}

void FileTransferReceiver::scanLocalHost()
{
    QRegExp rxIp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExpValidator v(rxIp);
    QList<QHostAddress> localAllAddresses = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, localAllAddresses){
        QString ipAddr = address.toString();
        int pos = 0;
        // use the first IP address
        if (ipAddr == "127.0.0.1") continue;
        if (v.validate(ipAddr,pos) == QRegExpValidator::Acceptable){
            QStringList localSplit = ipAddr.split(".");
            QString prefixIp = QString("%1.%2.%3").arg(localSplit[0]).arg(localSplit[1]).arg(localSplit[3]);
            for(int i = 1; i < 255; i++) {
//                for(int j = 1; j < 255; j++) {
                if (ipAddr == QString("%1.%2").arg(prefixIp).arg(i)) continue;
                    ScanWorkerThread *localScanWorkerThread = new ScanWorkerThread(QString("%1.%2").arg(prefixIp).arg(i), 8888);
                    connect(localScanWorkerThread, &ScanWorkerThread::onConnected, this, &FileTransferReceiver::onScanAvilable);
                    pool->addThreadTask(localScanWorkerThread);
//                }
            }
        }
    }
    scanProgressBar->setVisible(true);
    remoteSwitchBox->clear();
    remoteSwitchBox->addItem("127.0.0.1(本机)", "127.0.0.1");
    scanProgressBar->setMaximum(pool->count());
    pool->setMaxTaskTogether(60);
    pool->doStart();
}
