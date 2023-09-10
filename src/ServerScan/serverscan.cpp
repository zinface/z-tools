#include "serverscan.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QHostInfo>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QNetworkInterface>
#include <QPushButton>
#include <QRegExpValidator>
#include <QStringListModel>
#include <QTcpSocket>
#include <QThread>
#include <QMenu>
#include <QApplication>
#include <QClipboard>

ServerScan::ServerScan(QWidget *parent) : QWidget(parent)
  ,scanProgressBar(new QProgressBar)
  ,m_remoteStart(new QLineEdit(""))
  ,m_remoteEnd(new QLineEdit)
  ,m_switchFeture(new QComboBox)
  ,m_remoteStartLab(new QLabel("起始地址:"))
  ,m_remoteEndLab(new QLabel("结束地址:"))
  ,mServerPortLab(new QLabel("端口:"))
  ,threadNumLab(new QLabel("线程数量:"))
  ,mServerPort(new QLineEdit(""))
  ,mScanButton(new QPushButton("扫描"))
  ,mHostinfo(new QLabel())
{
    initUI();
//    initRegExp();

    QRegExp rxIp("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    QRegExp rxPort("[\\d,-]{0,}");
    QRegExpValidator v(rxIp);
    QRegExpValidator v1(rxPort);

    m_remoteStart->setValidator(new QRegExpValidator(rxIp, this));
    m_remoteEnd->setValidator(new QRegExpValidator(rxIp, this));
    mServerPort->setValidator(new QRegExpValidator(rxPort, this));

    pool.setMaxTaskTogether(50);
    threadNumBox.setRange(1,999);

    QList<QHostAddress> localAllAddresses = QNetworkInterface::allAddresses();
//    localAllAddresses.first();
    foreach(QHostAddress address, localAllAddresses){
        QString ipAddr = address.toString();
        int pos = 0;
        // use the first IP address
        if (v.validate(ipAddr,pos) == QRegExpValidator::Acceptable){
            mHostinfo->setText(mHostinfo->text() + address.toString() + " - ");
        }
    }
    connect(m_remoteStart, &QLineEdit::textChanged,this, &ServerScan::remoteStartChanged);
    connect(mScanButton, &QPushButton::clicked, this, &ServerScan::onScanStart);
    connect(&pool, &ScanWorkerPool::onTaskThreadChanged, this, &ServerScan::onScanThreadChanged);
    connect(&threadNumBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ServerScan::onScanThreadNumChanged);

    setFixedSize(500,500);
}

void ServerScan::onScanStart()
{
    m_resuleView.clear();
    QList<QString> servers;
    QList<int> ports;
    QString startIp = m_remoteStart->text();
    QString endIp = m_remoteEnd->text();
    QString port = mServerPort->text();

    while(1) {
        if (port.endsWith(",") || port.endsWith("-")) {
            port.remove(port.length()-1,1);
        } else if (port.startsWith(",") || port.startsWith("-")) {
            port.remove(0,1);
        } else {
            break;
        }
    }

    mServerPort->setText(port);

    QStringList localPortSplit = port.split(",");
    foreach (QString local, localPortSplit) {
        if (local.contains("-")) {
             QStringList rangePort = local.split("-");
             if (rangePort.count() >= 2) {
                 if(rangePort[0].toInt() > 65535 || rangePort[1].toInt() >65535) {
                     continue;
                 }

                 if (rangePort[0].toInt() > rangePort[1].toInt()) {
                     ports.append(rangeIntList(rangePort[1].toInt(), rangePort[0].toInt()));
                 }

                 if (rangePort[0].toInt() < rangePort[1].toInt()) {
                     ports.append(rangeIntList(rangePort[0].toInt(), rangePort[1].toInt()));
                 }
             }
        } else {
            if(local.toInt() > 0 && local.toInt() <= 65535) {
                ports.append(local.toInt());
            }
        }
    }

    QStringList localStartIpSplit = startIp.split(".");
    QStringList localEndIpSplit = endIp.split(".");

    // 如果起始和结束地址都为该格式： aaa.bbb.ccc.ddd
    if (localStartIpSplit.count() == 4 && localEndIpSplit.count() == 4) {
        QString prefixIp = QString("%1.%2.%3").arg(localStartIpSplit[0]).arg(localStartIpSplit[1]).arg(localStartIpSplit[2]);

        QString beginGroupIp = localStartIpSplit[2];
        QString endGroupIp = localEndIpSplit[2];

        if (beginGroupIp.compare(endGroupIp,Qt::CaseSensitive) != 0) {
            QString beginIp = localStartIpSplit[3];
            QString endIp = localEndIpSplit[3];

            for (int g = beginGroupIp.toInt(); g<endGroupIp.toInt(); g++) {
                if (g == beginGroupIp.toInt()) {
                    for(int i = beginIp.toInt(); i <= 254; i++) {
                        servers.append(QString("%1.%2").arg(QString("%1.%2.%3").arg(localStartIpSplit[0]).arg(localStartIpSplit[1]).arg(g)).arg(i));
                    }
                } else if (g == endGroupIp.toInt()) {
                    for(int i = 1; i <= endIp.toInt(); i++) {
                        servers.append(QString("%1.%2").arg(QString("%1.%2.%3").arg(localStartIpSplit[0]).arg(localStartIpSplit[1]).arg(g)).arg(i));
                    }
                } else {
                    for(int i = 1; i <= 254; i++) {
                        servers.append(QString("%1.%2").arg(QString("%1.%2.%3").arg(localStartIpSplit[0]).arg(localStartIpSplit[1]).arg(g)).arg(i));
                    }
                }
            }
        } else {
            QString beginIp = localStartIpSplit[3];
            QString endIp = localEndIpSplit[3];

            for(int i = beginIp.toInt(); i <= endIp.toInt(); i++) {
                servers.append(QString("%1.%2").arg(prefixIp).arg(i));
            }
        }
    } else if(localStartIpSplit.count() == 4){
        QString prefixIp = QString("%1.%2.%3").arg(localStartIpSplit[0]).arg(localStartIpSplit[1]).arg(localStartIpSplit[2]);
        QString beginIp = localStartIpSplit[3];

        for(int i = beginIp.toInt(); i <= beginIp.toInt(); i++) {
            servers.append(QString("%1.%2").arg(prefixIp).arg(i));
        }
    }

    if (startIp == "" && endIp == "") {
        servers.clear();

        foreach(QHostAddress address, QNetworkInterface::allAddresses()){
            QString ipAddr = address.toString();
            QTextStream(stdout) << QString("我的 IP 地址: %1\n").arg(ipAddr);
            auto localSplit = ipAddr.split(".");
            if (localSplit.count() == 4) {
                QString prefixIp = QString("%1.%2.%3").arg(localSplit[0]).arg(localSplit[1]).arg(localSplit[2]);
                for (int i = 1; i < 254; ++i) {
                    servers.append(QString("%1.%2").arg(prefixIp).arg(i));
                }
            }
        }
    }

    QStringList result;
    foreach (QString server, servers) {
        foreach (int port, ports){
            ScanWorkerThread *localScanWorkerThread = new ScanWorkerThread(server, port);
            connect(localScanWorkerThread, SIGNAL(onConnected(QString, int, QString)), this, SLOT(onConnected(QString, int, QString)));
            pool.addThreadTask(localScanWorkerThread);
        }
    }

    scanProgressBar->setMaximum(pool.count());
    pool.doStart();
}

void ServerScan::onConnected(QString host, int port, QString protocal)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setData(Qt::DisplayRole, QString("%1:%2 %3").arg(host).arg(port).arg(protocal));
    item->setData(Qt::UserRole, host);
    item->setData(Qt::UserRole+1, port);
    item->setData(Qt::UserRole+2, protocal);
    m_resuleView.addItem(item);
}

void ServerScan::onScanThreadChanged()
{
    scanProgressBar->setValue(scanProgressBar->maximum() - pool.count());
}

void ServerScan::onScanThreadNumChanged(int num)
{
    pool.setMaxTaskTogether(num);
}

void ServerScan::remoteStartChanged()
{
    if (m_remoteStart->text().startsWith(m_remoteEnd->text())){
        m_remoteEnd->setText(m_remoteStart->text());
    }
}

void ServerScan::initUI()
{
    m_remoteStart->setPlaceholderText("0.0.0.0");
    m_remoteEnd->setPlaceholderText("0.0.0.0");
    mServerPort->setPlaceholderText("22,80,8080-8090");

    scanProgressBar->setMaximum(100);

    QGridLayout *serverParamLayout = new QGridLayout;
    serverParamLayout->addWidget(m_remoteStartLab, 0,0,1,1);
    serverParamLayout->addWidget(m_remoteStart, 0,1,1,1);
    serverParamLayout->addWidget(m_remoteEndLab,0,2,1,1);
    serverParamLayout->addWidget(m_remoteEnd,0,3,1,1);
    serverParamLayout->addWidget(mServerPortLab, 1,0,1,1);
    serverParamLayout->addWidget(mServerPort, 1,1,1,1);
    serverParamLayout->addWidget(threadNumLab, 1,2,1,1);
    serverParamLayout->addWidget(&threadNumBox, 1,3,1,1);

    QHBoxLayout *adjustParamLayout = new QHBoxLayout;
    adjustParamLayout->addLayout(serverParamLayout);

    QGroupBox *serverSettingBox = new QGroupBox("参数设置");
    serverSettingBox->setLayout(adjustParamLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(serverSettingBox);
    mainLayout->addWidget(mScanButton);\
    mainLayout->addWidget(&m_resuleView);
    mainLayout->addWidget(scanProgressBar);
    mainLayout->addWidget(mHostinfo);

    mHostinfo->setTextInteractionFlags(Qt::TextSelectableByMouse);

// [1 自定义右键菜单]
    m_resuleView.setContextMenuPolicy(Qt::CustomContextMenu);
    m_resuleView_menu = new QMenu();

    m_resuleView_menu->addAction("复制", [this](){
        QClipboard *clipboard = qApp->clipboard();
        auto item = m_resuleView.currentItem();
        QString text = item->text();
        clipboard->setText(text);
    });
    m_resuleView_menu->addAction("复制(IP)", [this](){
        QClipboard *clipboard = qApp->clipboard();
        auto item = m_resuleView.currentItem();
        QString text = QString("%1")
                            .arg(item->data(Qt::UserRole).toString());
        clipboard->setText(text);
    });
    m_resuleView_menu->addAction("复制(IP:PORT)", [this](){
        QClipboard *clipboard = qApp->clipboard();
        auto item = m_resuleView.currentItem();
        QString text = QString("%1:%2")
                            .arg(item->data(Qt::UserRole).toString())
                            .arg(item->data(Qt::UserRole+1).toInt());
        clipboard->setText(text);
    });
    connect(&m_resuleView, &QListWidget::customContextMenuRequested, [this](const QPoint &pos){
        if (m_resuleView.selectedItems().count() > 0) {
            m_resuleView_menu->popup(QCursor::pos());
        }
    });
// [1]

    setLayout(mainLayout);
}

QList<int> ServerScan::rangeIntList(int x, int y)
{
    QList<int> ports;
    for (int i = x; i<y; i++) {
        ports.append(i);
    }
    return ports;
}

