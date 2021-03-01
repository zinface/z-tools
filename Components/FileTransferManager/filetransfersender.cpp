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
  ,listWidget(new QListWidget)
  ,addFileBtn(new QPushButton("添加"))
  ,delFileBtn(new QPushButton("删除"))
  ,clrFileBtn(new QPushButton("清空"))
  ,listenPort(new QLabel("....."))
  ,statusBar(new QLabel("....."))
  ,clientStatus(new QLabel("客户端连接数: 0"))
  ,currentLab(new QLabel("当前进度:"))
  ,totalLab(new QLabel("全部进度:"))
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

    connect(this, &FileTransferSender::filesChanged, this, &FileTransferSender::onFileListChanged);

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

    listWidget->addItems(openFileNames);
    emit filesAppended();
    emit filesChanged();
}

void FileTransferSender::delFile()
{
    int row = listWidget->currentRow();
    if (row < 0) {
        return;
    }
    delete listWidget->takeItem(row);
    emit filesDeleted();
    emit filesChanged();
}

void FileTransferSender::clrFile()
{
    int count = listWidget->count();
    if (0 == count) {
        return;
    }
    int ret = QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("确定清空列表吗?"),
                                       QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel);
    if (QMessageBox::Ok != ret) {
        return;
    }

    for (int i = 0; i < count; i++) {
        delete listWidget->takeItem(0);
    }
    emit filesCleanded();
    emit filesChanged();
}

void FileTransferSender::onFileListChanged()
{
    if (listWidget->count() > 0) {
        currentProgressBar.setFormat(QStringLiteral("%1 : %p%").arg(listWidget->item(0)->text()));
    }
    if (listWidget->count() == 0) {
        currentProgressBar.setFormat(QStringLiteral("%p%"));
    }
}

void FileTransferSender::onTimerout()
{
    totalProgressBar.setValue(value++);
    currentProgressBar.setValue(value);
}

void FileTransferSender::createFileTransferSender()
{
    QGroupBox *senderBox = new QGroupBox("File Sender");
    QHBoxLayout *fileCtlsLayout = new QHBoxLayout;
    QHBoxLayout *currentProgressLayout = new QHBoxLayout;
    QHBoxLayout *totalProgressLayout = new QHBoxLayout;
    QVBoxLayout *senderBoxLayout = new QVBoxLayout;

    fileCtlsLayout->addWidget(addFileBtn);
    fileCtlsLayout->addWidget(delFileBtn);
    fileCtlsLayout->addWidget(clrFileBtn);

    senderBoxLayout->addLayout(fileCtlsLayout);
    senderBoxLayout->addWidget(listWidget);

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
    statusBar->setText(statusBar->text() + " - " + listenPort->text());
    setLayout(mainLayout);
}
