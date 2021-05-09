#include "filetransferrecivermanualhostdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRegExpValidator>
#include <QVBoxLayout>
#include <scanworkerpool.h>

FileTransferReciverManualHostDialog::FileTransferReciverManualHostDialog(QWidget *parent) : QDialog(parent)
  ,ipLabel(new QLabel("IP地址："))
  ,hostEdit(new QLineEdit)
  ,hostTestLabel(new QLabel("测试结果:"))
  ,confirmBtn(new QPushButton("确认"))
  ,testBtn(new QPushButton("测试连接"))
  ,pool(new ScanWorkerPool)
  ,connected(false)
{
    createFileTransferReciverManualHostDialog();

    QRegExp rx("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$");
    hostEdit->setValidator(new QRegExpValidator(rx,this));

    connect(confirmBtn, &QPushButton::clicked, this, &FileTransferReciverManualHostDialog::onConfirmChoose);
    connect(testBtn, &QPushButton::clicked, this, &FileTransferReciverManualHostDialog::onTestBtnChoose);
    connect(pool, &ScanWorkerPool::finished, this, &FileTransferReciverManualHostDialog::onTestFinished);
    connect(hostEdit, &QLineEdit::textChanged, this, &FileTransferReciverManualHostDialog::onTestBtnChoose);
    setWindowTitle("寻找目标");
}

void FileTransferReciverManualHostDialog::createFileTransferReciverManualHostDialog()
{
    QHBoxLayout *inputLayout = new QHBoxLayout;
    QHBoxLayout *btnLayout = new QHBoxLayout;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    inputLayout->addWidget(ipLabel);
    inputLayout->addWidget(hostEdit);

    btnLayout->addWidget(testBtn, Qt::AlignLeft);
    btnLayout->addWidget(confirmBtn, Qt::AlignRight);

    mainLayout->addLayout(inputLayout);
    mainLayout->addWidget(hostTestLabel, Qt::AlignVCenter);
    mainLayout->addLayout(btnLayout);

    hostTestLabel->setVisible(false);

    setLayout(mainLayout);
}

QString FileTransferReciverManualHostDialog::getHost()
{
    if (connected) {
        return hostEdit->text();
    }
    return QString();
}

void FileTransferReciverManualHostDialog::onTestConnected()
{
    mute.lock();
    connected = true;
    this->hostTestLabel->setText("测试结果: ip地址正常");
    mute.unlock();
}

void FileTransferReciverManualHostDialog::onTestFinished()
{
    mute.lock();
    if (connected) {
        this->hostTestLabel->setText("测试结果: ip地址正常");
        return;
    }
    this->hostTestLabel->setText("测试结果: ip地址异常");
    mute.unlock();
}

void FileTransferReciverManualHostDialog::onTestBtnChoose()
{
    connected = false;
    this->hostTestLabel->setVisible(true);
    if (hostEdit->text().isEmpty()) {
        this->hostTestLabel->setText("测试结果: ip地址不能为空");
        return;
    }

    if (hostEdit->text().split(".").count() != 4) {
        this->hostTestLabel->setText("测试结果: ip地址不正确");
        return;
    }
    ScanWorkerThread *localScanWorkerThread = new ScanWorkerThread(hostEdit->text(), 8888);
    connect(localScanWorkerThread, &ScanWorkerThread::onConnected, this, &FileTransferReciverManualHostDialog::onTestConnected);
    pool->addThreadTask(localScanWorkerThread);
    pool->start();
}

void FileTransferReciverManualHostDialog::onConfirmChoose()
{
    this->accept();
}
