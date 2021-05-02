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
#include <filereceiverview.h>
#include <filetransfermanager.h>
#include <qmessagebox.h>
#include <scanworkerpool.h>

FileTransferReceiver::FileTransferReceiver(QWidget *parent) : QWidget(parent)
  ,manager(new FileTransferManager)
  ,statusBar(new QLabel("服务器待连接."))
  ,clientStatus(new QLabel("服务器断开连接!"))
  ,m_fileSize(0)
  ,m_fileBytesReceivedSize(0)
  ,filelistView(new FileReceiverView)
  ,remoteSwitchBox(new QComboBox)
  ,connectBtn(new QPushButton("选中"))
  ,saveFileBtn(new QPushButton("保存位置"))
  ,scanProgressBar(new QProgressBar)
  ,receiveFileBtn(new QPushButton("下载"))
  ,currentProgressBar(new QProgressBar)
  ,totalProgressBar(new QProgressBar)
  ,pool(new ScanWorkerPool)
{
    createFileTransferReceiver();

    connect(connectBtn, &QPushButton::clicked, this, &FileTransferReceiver::onUseRemoteSwitch);
    connect(saveFileBtn, &QPushButton::clicked, this, &FileTransferReceiver::onConfigFileSavePath);
    connect(pool, &QThread::finished, this, &FileTransferReceiver::onScanFinished);
    connect(pool, &ScanWorkerPool::onTaskThreadChanged, this, &FileTransferReceiver::onScanThreadChanged);

    connect(manager, &FileTransferManager::connected, this, &FileTransferReceiver::connected);
    connect(manager, &FileTransferManager::disconnected, this, &FileTransferReceiver::disconnected);

    connect(manager, &FileTransferManager::onRemoteFileAppend, filelistView, &FileReceiverView::appendFile);
    connect(manager, &FileTransferManager::onRemoteFileDelete, filelistView, &FileReceiverView::deleteFile);
    connect(manager, &FileTransferManager::onRemoteFileClear, filelistView, &FileReceiverView::clearFile);

    connect(manager, &FileTransferManager::connected, this, &FileTransferReceiver::onUpdateFileSavePath);
    connect(manager, &FileTransferManager::disconnected, this, &FileTransferReceiver::onUpdateFileSavePath);

    setFixedSize(500,500);
}

void FileTransferReceiver::onConfigFileSavePath()
{
    QString srcDirPath = QFileDialog::getExistingDirectory(this, "选择文件夹", QDir::homePath(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!srcDirPath.isEmpty()) {
        savePath = srcDirPath;
        filelistView->setSavePath(savePath);
    }
    onUpdateFileSavePath();
}

void FileTransferReceiver::onUpdateFileSavePath()
{
    if (manager->state()) {
        clientStatus->setText(QString("服务器连接成功! - 存储地址为：%1").arg(savePath));
    } else {
        clientStatus->setText(QString("服务器断开连接! - 存储地址为：%1").arg(savePath));
    }
}

void FileTransferReceiver::onConnect()
{
    if (savePath.isEmpty()){
        statusBar->setText("请设置存储目录!");
        onConfigFileSavePath();
        if (savePath.isEmpty()) return;
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

    manager->setManagerTask(address, 8888, SessionManager::CLIENT);
}

void FileTransferReceiver::connected()
{
    filelistView->clearFile();
    onUpdateFileSavePath();
    manager->fetchFileListAction();
}

void FileTransferReceiver::disconnected()
{
    onUpdateFileSavePath();
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
    filelistView->clearFile();
    QTextStream(stdout) << QString("filelistView->clearFile()----------- \n");
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
    QHBoxLayout *currentProgressLayout = new QHBoxLayout;
    QHBoxLayout *totalProgressLayout = new QHBoxLayout;

    remoteCtlsLayout->addWidget(remoteSwitchBox, 2);
    remoteCtlsLayout->addWidget(connectBtn, 1);
    /*************************************/
    QFrame *vline;
    vline = new QFrame();
    vline->setFrameShape(QFrame::VLine);
    vline->setFrameShadow(QFrame::Sunken);
    /*************************************/
    remoteCtlsLayout->addWidget(vline);
    remoteCtlsLayout->addWidget(saveFileBtn, 1);

    receiverBoxLayout->addLayout(remoteCtlsLayout);
    receiverBoxLayout->addWidget(scanProgressBar);
    scanProgressBar->setVisible(false);
    receiverBoxLayout->addWidget(filelistView);

    receiverBox->setLayout(receiverBoxLayout);

    /*************************************/
    QFrame *line;
    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QFrame *line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    /*************************************/

    currentProgressLayout->addWidget(new QLabel("当前进度:"));
    currentProgressLayout->addWidget(currentProgressBar);

    totalProgressLayout->addWidget(new QLabel("全部进度:"));
    totalProgressLayout->addWidget(totalProgressBar);

    remoteSwitchBox->addItem("127.0.0.1(本机)", "127.0.0.1");
    remoteSwitchBox->addItem("手动输入", "");
    remoteSwitchBox->addItem("重新扫描", "");

    QVBoxLayout *mainLayout = new QVBoxLayout;

    mainLayout->addWidget(receiverBox);
    mainLayout->addWidget(line);
    mainLayout->addLayout(currentProgressLayout);
    mainLayout->addLayout(totalProgressLayout);
    mainLayout->addWidget(line2);
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
                if (ipAddr == QString("%1.%2").arg(prefixIp).arg(i)) continue;
                ScanWorkerThread *localScanWorkerThread = new ScanWorkerThread(QString("%1.%2").arg(prefixIp).arg(i), 8888);
                connect(localScanWorkerThread, &ScanWorkerThread::onConnected, this, &FileTransferReceiver::onScanAvilable);
                pool->addThreadTask(localScanWorkerThread);
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
