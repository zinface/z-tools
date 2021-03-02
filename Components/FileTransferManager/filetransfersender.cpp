#include "filetransfersender.h"

#include <QApplication>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QTcpSocket>
#include <QTimer>
#include <QVBoxLayout>

FileTransferSender::FileTransferSender(QWidget *parent) : QWidget(parent)
  ,fileListWidget(new QListWidget)
  ,addFileBtn(new QPushButton("添加"))
  ,delFileBtn(new QPushButton("删除"))
  ,clrFileBtn(new QPushButton("清空"))
  ,currentLab(new QLabel("当前进度:"))
  ,totalLab(new QLabel("全部进度:"))
  ,listenPort(new QLabel("....."))
  ,statusBar(new QLabel("....."))
  ,clientStatus(new QLabel("客户端连接数: 0"))
  ,filesQueueStatus(new QLabel("文件传输队列: 0"))
{
//    qApp->setStyle(QStyleFactory::create("fusion"));

    if (tcpServer.listen(QHostAddress::Any, 8888)) {
        statusBar->setText(QStringLiteral("状态：正在监听"));
    } else {
        statusBar->setText(QStringLiteral("状态：监听失败"));
    }
    listenPort->setText(QString::number(tcpServer.serverPort()));

    createFileTransferSender();

    connect(&tcpServer, &QTcpServer::newConnection, this, &FileTransferSender::onNewConnection);

    connect(addFileBtn, &QPushButton::clicked,this,&FileTransferSender::addFile);
    connect(delFileBtn, &QPushButton::clicked,this,&FileTransferSender::delFile);
    connect(clrFileBtn, &QPushButton::clicked,this,&FileTransferSender::clrFile);

    connect(this, &FileTransferSender::filesAppended, this, &FileTransferSender::onfilesAppended);
    connect(this, &FileTransferSender::filesDeleted, this, &FileTransferSender::onfilesDeleted);
    connect(this, &FileTransferSender::filesCleanded, this, &FileTransferSender::onfilesCleanded);
    connect(this, &FileTransferSender::filesChanged, this, &FileTransferSender::onFileListChanged);
    connect(this, &FileTransferSender::emitFilesQueueChange, this, &FileTransferSender::onFilesQueueChange);

    connect(this, &FileTransferSender::clientChanged,this,&FileTransferSender::onClientChanged);
    QTimer *t = new  QTimer;
    t->setInterval(1000);
    connect(t, &QTimer::timeout, this, &FileTransferSender::onTimerout);
    t->start();
}

void FileTransferSender::onNewConnection()
{
    QTcpSocket *localNextPendingConnection = tcpServer.nextPendingConnection();
//    localNextPendingConnection
    m_tcpMap.insert(QString::number(conn_cnt++), localNextPendingConnection);
    connect(localNextPendingConnection, &QTcpSocket::readyRead, this, &FileTransferSender::onReadyRead);
    connect(localNextPendingConnection, &QTcpSocket::disconnected, this, &FileTransferSender::onDisconnected);
    emit clientChanged();
//    connect(localNextPendingConnection, &QTcpSocket::disconnected, localNextPendingConnection, &QObject::deleteLater);
}

void FileTransferSender::onReadyRead()
{

}

void FileTransferSender::onDisconnected()
{
    foreach (QString key, m_tcpMap.keys()) {
        QTcpSocket *socket = m_tcpMap[key];
        if (socket->state() == QTcpSocket::UnconnectedState) {
            m_tcpMap.remove(key);
            socket->deleteLater();
        }
    }
    emit clientChanged();
}

void FileTransferSender::onClientChanged()
{
    clientStatus->setText(QString("客户端连接数: %1").arg(m_tcpMap.count()));
}

void FileTransferSender::addFile()
{
    QStringList openFileNames = QFileDialog::getOpenFileNames(this, QStringLiteral("选择文件"),
                                  QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    if (openFileNames.isEmpty()) {
        return;
    }

    fileListWidget->addItems(openFileNames);
    emit filesAppended(openFileNames);
    emit filesChanged();
}

void FileTransferSender::delFile()
{
    int row = fileListWidget->currentRow();
    if (row < 0) {
        return;
    }
    QListWidgetItem *localTakeItem = fileListWidget->takeItem(row);
    emit filesDeleted(QString(localTakeItem->text()));
    emit filesChanged();
    delete localTakeItem;
}

void FileTransferSender::clrFile()
{
    int count = fileListWidget->count();
    if (0 == count) {
        return;
    }
    int ret = QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("确定清空列表吗?"),
                                       QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    if (QMessageBox::Ok != ret) {
        return;
    }

    for (int i = 0; i < count; i++) {
        delete fileListWidget->takeItem(0);
    }
    emit filesCleanded();
    emit filesChanged();
}

void FileTransferSender::onfilesAppended(QStringList &files)
{
    m_fileQueue.append(files);
    emit emitFilesQueueChange();
}

void FileTransferSender::onfilesDeleted(QString file)
{
    m_fileQueue.removeOne(file);
    emit emitFilesQueueChange();
}

void FileTransferSender::onfilesCleanded()
{
    m_fileQueue.clear();
    emit emitFilesQueueChange();
}

void FileTransferSender::onFileListChanged()
{
    if (fileListWidget->count() > 0) {
        currentProgressBar.setFormat(QStringLiteral("%1 : %p%").arg(fileListWidget->item(0)->text()));
    }
    if (fileListWidget->count() == 0) {
        currentProgressBar.setFormat(QStringLiteral("%p%"));
    }

}

void FileTransferSender::onTimerout()
{
    totalProgressBar.setValue(value++);
    currentProgressBar.setValue(value);
}

void FileTransferSender::onFilesQueueChange()
{
    filesQueueStatus->setText(QString("文件传输队列: %1").arg(m_fileQueue.count()));
}

void FileTransferSender::createFileTransferSender()
{
    QGroupBox *senderBox = new QGroupBox("File Sender");
    QHBoxLayout *fileCtlBtnsLayout = new QHBoxLayout;
    QHBoxLayout *currentProgressLayout = new QHBoxLayout;
    QHBoxLayout *totalProgressLayout = new QHBoxLayout;
    QVBoxLayout *senderBoxLayout = new QVBoxLayout;

    fileCtlBtnsLayout->addWidget(addFileBtn);
    fileCtlBtnsLayout->addWidget(delFileBtn);
    fileCtlBtnsLayout->addWidget(clrFileBtn);

    senderBoxLayout->addLayout(fileCtlBtnsLayout);
    senderBoxLayout->addWidget(fileListWidget);

    senderBox->setLayout(senderBoxLayout);

    currentProgressLayout->addWidget(currentLab);
    currentProgressLayout->addWidget(&currentProgressBar);
    currentProgressBar.setMaximum(100);

    totalProgressLayout->addWidget(totalLab);
    totalProgressLayout->addWidget(&totalProgressBar);
    totalProgressBar.setMaximum(100);

    /*************************************/
    QFrame *line;
    line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    QFrame *line2 = new QFrame();
    line2->setFrameShape(QFrame::HLine);
    line2->setFrameShadow(QFrame::Sunken);
    /*************************************/

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(senderBox);
    mainLayout->addWidget(line);
    mainLayout->addLayout(currentProgressLayout);
    mainLayout->addLayout(totalProgressLayout);
    mainLayout->addWidget(line2);
    mainLayout->addWidget(statusBar);
    mainLayout->addWidget(clientStatus);
    mainLayout->addWidget(filesQueueStatus);
    statusBar->setText(statusBar->text() + " - " + listenPort->text());
    setLayout(mainLayout);
}
