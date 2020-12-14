#include "serverscan.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QRegExpValidator>
#include <QStringListModel>
#include <QTcpSocket>
#include <QThread>

ServerScan::ServerScan(QWidget *parent) : QWidget(parent)
  ,mServerPrefixLab(new QLabel("服务:"))
  ,mServerPrefix(new QLineEdit(""))
  ,mServerPortLab(new QLabel("端口:"))
  ,mServerPort(new QLineEdit(""))
  ,mScanButton(new QPushButton("扫描"))
  ,m_model(new QStringListModel)
  ,mResultView(new QListView)

{
    initUi();
    mResultView->setModel(m_model);
    setFixedSize(500,500);
    connect(mScanButton, &QPushButton::clicked, this, &ServerScan::onScanStart);
}

void ServerScan::onScanStart()
{
    QString server = mServerPrefix->text();
    QString port = mServerPort->text();
    m_model->setStringList(QStringList());
    QStringList result;
    result << QString("server:" + server) << "port:" + port;
    ScanWorkerThread *m_ScanWorker = new ScanWorkerThread[255];
    for (int i = 1; i < 255; i++) {
//        m_ScanWorker[i].start();
        m_ScanWorker[i].setWorkParam(QString(server+"."+QString::number(i)), port.toInt());
        connect(m_ScanWorker+i, SIGNAL(onConnected(QString)), this, SLOT(onConnected(QString)));
//        m_ScanWorker = new ScanWorkerThread(QString(server+"."+QString::number(i)), port.toInt());
        m_ScanWorker[i].start();
    }
}

void ServerScan::onConnected(QString result)
{
    m_model->setStringList(m_model->stringList() << result);
}

void ServerScan::initUi()
{
    mServerPrefix->setPlaceholderText("e.g: 0.0.0.0");
    mServerPort->setPlaceholderText("e.g: 80");

    QRegExpValidator *ipValidator = new QRegExpValidator;
    ipValidator->setRegExp(QRegExp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)"));
    mServerPrefix->setMaxLength(15);
    mServerPrefix->setValidator(ipValidator);

    QIntValidator *intValidator = new QIntValidator;
    intValidator->setRange(1,65535);
    mServerPort->setMaxLength(5);
    mServerPort->setValidator(intValidator);


    QGridLayout *serverParamLayout = new QGridLayout;
    serverParamLayout->addWidget(mServerPrefixLab, 0,0,1,1);
    serverParamLayout->addWidget(mServerPrefix, 0,1,1,2);
    serverParamLayout->addWidget(mServerPortLab, 1,0,1,1);
    serverParamLayout->addWidget(mServerPort, 1,1,1,2);
//    serverParamLayout->addWidget(mScanButton,2,0,1,3);

    QHBoxLayout *adjustParamLayout = new QHBoxLayout;
    adjustParamLayout->addLayout(serverParamLayout);

    QGroupBox *serverSettingBox = new QGroupBox("参数设置");
    serverSettingBox->setLayout(adjustParamLayout);


    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(serverSettingBox);
    mainLayout->addWidget(mScanButton);
    mainLayout->addWidget(mResultView);
//    mainLayout->addStretch();

    setLayout(mainLayout);
}

