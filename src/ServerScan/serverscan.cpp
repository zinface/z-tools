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
    , rxIP("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)")
    , rxPort("[\\d,-]{0,}")
    , scanProgressBar(new QProgressBar)
    , m_remoteStart(new QLineEdit(""))
    , m_remoteEnd(new QLineEdit)
    , m_switchFeture(new QComboBox)
    , m_remoteStartLab(new QLabel("起始地址:"))
    , m_remoteEndLab(new QLabel("结束地址:"))
    , m_serverPortLab(new QLabel("端口:"))
    , threadNumLab(new QLabel("线程数量:"))
    , m_serverPort(new QLineEdit(""))
    , m_scanButton(new QPushButton("扫描"))
    , mHostinfo(new QLabel())
{
    initUI();
    initLocalIpAddress();

    // 输入方式
    m_remoteStart->setValidator(new QRegExpValidator(rxIP, this));
    m_remoteEnd->setValidator(new QRegExpValidator(rxIP, this));
    m_serverPort->setValidator(new QRegExpValidator(rxPort, this));

    pool.setMaxTaskTogether(50);
    threadNumBox.setRange(1,999);


    connect(m_remoteStart, &QLineEdit::textChanged,this, &ServerScan::remoteStartChanged);
    connect(m_scanButton, &QPushButton::clicked, this, &ServerScan::onScanStart);
    connect(&pool, &ScanWorkerPool::onTaskThreadChanged, this, &ServerScan::onScanThreadChanged);
    connect(&threadNumBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &ServerScan::onScanThreadNumChanged);

    setFixedSize(500,500);
}

void ServerScan::initUI()
{
    m_remoteStart->setPlaceholderText("0.0.0.0");
    m_remoteEnd->setPlaceholderText("0.0.0.0");
    m_serverPort->setPlaceholderText("22,80,8080-8090");

    scanProgressBar->setMaximum(100);

    QGridLayout *serverParamLayout = new QGridLayout;
    serverParamLayout->addWidget(m_remoteStartLab, 0,0,1,1);
    serverParamLayout->addWidget(m_remoteStart, 0,1,1,1);
    serverParamLayout->addWidget(m_remoteEndLab,0,2,1,1);
    serverParamLayout->addWidget(m_remoteEnd,0,3,1,1);
    serverParamLayout->addWidget(m_serverPortLab, 1,0,1,1);
    serverParamLayout->addWidget(m_serverPort, 1,1,1,1);
    serverParamLayout->addWidget(threadNumLab, 1,2,1,1);
    serverParamLayout->addWidget(&threadNumBox, 1,3,1,1);

    QHBoxLayout *adjustParamLayout = new QHBoxLayout;
    adjustParamLayout->addLayout(serverParamLayout);

    QGroupBox *serverSettingBox = new QGroupBox("参数设置");
    serverSettingBox->setLayout(adjustParamLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(serverSettingBox);
    mainLayout->addWidget(m_scanButton);\
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

void ServerScan::initLocalIpAddress()
{
    QStringList localIpAddresses;

    QRegExpValidator v(rxIP);
    QRegExpValidator v1(rxPort);

    QList<QHostAddress> localAllAddresses = QNetworkInterface::allAddresses();
    foreach(QHostAddress address, localAllAddresses){
        QString ipAddress = address.toString();
        int pos = 0;

        // use the first IP address
        if (v.validate(ipAddress,pos) == QRegExpValidator::Acceptable){
            localIpAddresses.append(ipAddress);
        }
    }
    mHostinfo->setText(localIpAddresses.join(" - "));
}

QList<int> ServerScan::rangeIntList(int x, int y)
{
    QList<int> ports;
    for (int i = x; i<y; i++) {
        ports.append(i);
    }
    return ports;
}

void ServerScan::onScanStart()
{
    m_resuleView.clear();
    QList<QString> servers;
    QList<int> ports;
    QString startIp = m_remoteStart->text();
    QString endIp = m_remoteEnd->text();
    QString port = m_serverPort->text();

    // [1] 清理端口部分起始与结尾可能出现 的,和-
    while(1) {
        if (port.endsWith(",") || port.endsWith("-")) {
            port.chop(1);
        } else if (port.startsWith(",") || port.startsWith("-")) {
            port.remove(0,1);
        } else {
            break;
        }
    }

    m_serverPort->setText(port);

    // [2] 切分端口
    QStringList localPortSplit = port.split(",");
    foreach (QString local, localPortSplit) {
        // 1.重建包含 - 符号的部分为一个新的端口列表
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
             // 如果该端口符合范围将加入到列表中
            if(local.toInt() > 0 && local.toInt() <= 65535) {
                ports.append(local.toInt());
            }
        }
    }

    // [3] 切分 IP 地址
    QStringList localStartIpSplit = startIp.split(".");
    QStringList localEndIpSplit = endIp.split(".");

    //1.起始和结束地址都为该格式： aaa.bbb.ccc.ddd
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

            // ! 跳过生成 127.0.0.x 的主机
            if (ipAddr.startsWith("127.0.0")) continue;

            //
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

    // 标识出当前机器
    foreach(QHostAddress address, QNetworkInterface::allAddresses()){
        QString ipAddr = address.toString();
        if (host == ipAddr) {
            item->setData(Qt::DisplayRole, QString("%1:%2 %3 (本机)").arg(host).arg(port).arg(protocal));
        }
    }
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





