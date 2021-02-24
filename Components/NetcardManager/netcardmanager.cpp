#include "netcardmanager.h"

#include <QDebug>
#include <QGridLayout>

NetcardManager::NetcardManager(QWidget *parent) : QWidget(parent)
{
    createNetworkInfomation();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(ifaceGroupBox);
    mainLayout->addStretch();
    this->setLayout(mainLayout);

    connect(refresh, &QPushButton::clicked, this, &NetcardManager::refreshInterfaces);
    connect(ifaceComb, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NetcardManager::setIfaceInfomation);

    this->refresh->click();
    setFixedSize(500,480);
}

void NetcardManager::refreshInterfaces()
{
    this->ifaceComb->clear();
    this->mapInfo.clear();
    for (NetworkInterface &iface :  tni->all()) {
        NetworkInterface::Info info = iface.info();


        QMap<QString, QString> temp;
        std::string ipv6_string;
        QString ipv6_qstr;

        if (info.ipv6_addrs.empty()){
            ipv6_string = "(none)";
            ipv6_qstr = "(none)";
        } else {
            using std::ostringstream;
            QTextStream ts;
//            ostringstream oss;
            for (size_t i = 0; i < info.ipv6_addrs.size(); i++) {
                const NetworkInterface::IPv6Prefix& prefix = info.ipv6_addrs[i];
                if (i>0){
                    ipv6_qstr = ipv6_qstr + ", ";
                }
                ipv6_qstr = ipv6_qstr +  QString::fromStdString(prefix.address.to_string()) + "/" + QString::number(prefix.prefix_length);
            }
//            ipv6_qstr >> ipv6_qstr;
        }

        temp["HW"] = QString::fromStdString(info.hw_addr.to_string());
        temp["IP4"] = QString::fromStdString(info.ip_addr.to_string());
        temp["IP6"] = ipv6_qstr;
        temp["Netmask"] = QString::fromStdString(info.netmask.to_string());
        temp["Broadcast"] = QString::fromStdString(info.bcast_addr.to_string());
        temp["Status"] = QString::fromStdString(iface.is_up() ? "up" : "down");
        mapInfo[QString::fromStdString(iface.name())] = temp;
        ifaceComb->addItem(QString::fromStdString(iface.name()),QString::fromStdString(iface.name()));
    }
}

void NetcardManager::setIfaceInfomation(int index)
{
    QString item = ifaceComb->itemText(index);
    ifaceHW->setText(mapInfo[item]["HW"]);
    ifaceIPv4->setText(mapInfo[item]["IP4"]);
    ifaceIPv6->setText(mapInfo[item]["IP6"]);
    ifaceNetMask->setText(mapInfo[item]["Netmask"]);
    ifaceBroadcast->setText(mapInfo[item]["Broadcast"]);
//    ifaceIndex->setText(mapInfo[item][""]);
    ifaceStatus->setText(mapInfo[item]["Status"]);
}

void NetcardManager::createNetworkInfomation()
{
    ifaceGroupBox = new QGroupBox("Network Infomation");
    ifaceLab = new QLabel("选择设备:");
    ifaceHWLab = new QLabel("硬件:");
    ifaceIPv4Lab = new QLabel("IPV4:");
    ifaceIPv6Lab = new QLabel("IPV6:");
    ifaceNetMaskLab = new QLabel("网关:");
    ifaceBroadcastLab = new QLabel("广播:");
    ifaceIndexLab = new QLabel("序号:");
    ifaceStatusLab = new QLabel("状态:");

    ifaceComb = new QComboBox;
    ifaceHW = new QLabel("null");
    ifaceIPv4 = new QLabel("null");
    ifaceIPv6 = new QLabel("null");
    ifaceNetMask = new QLabel("null");
    ifaceBroadcast = new QLabel("null");
    ifaceIndex = new QLabel("null");
    ifaceStatus = new QLabel("null");

    refresh = new QPushButton("刷新");

    QGridLayout *network = new QGridLayout;

    network->addWidget(ifaceLab, 0,0,1,1);
    network->addWidget(ifaceComb,0,1,1,2);
    network->addWidget(refresh,0,3,1,1);
    network->addWidget(ifaceHWLab,1,0,1,1);
    network->addWidget(ifaceHW,1,1,1,1);
    network->addWidget(ifaceIPv4Lab,2,0,1,1);
    network->addWidget(ifaceIPv4,2,1,1,1);
    network->addWidget(ifaceIPv6Lab,3,0,1,1);
    network->addWidget(ifaceIPv6,3,1,1,1);
    network->addWidget(ifaceNetMaskLab,4,0,1,1);
    network->addWidget(ifaceNetMask,4,1,1,1);
    network->addWidget(ifaceBroadcastLab,5,0,1,1);
    network->addWidget(ifaceBroadcast,5,1,1,1);
    network->addWidget(ifaceStatusLab,6,0,1,1);
    network->addWidget(ifaceStatus,6,1,1,1);
    ifaceGroupBox->setLayout(network);

}


//docker0:
//   HW address:     02:42:39:0e:19:d6
//   IP address:     172.17.0.1
//   IPv6 addresses: (none)
//   Netmask:        255.255.0.0
//   Broadcast:      172.17.255.255
//   Iface index:    4
//   Status:         interface up

//enp1s0:
//   HW address:     8c:ec:4b:b3:1f:7c
//   IP address:     0.0.0.0
//   IPv6 addresses: (none)
//   Netmask:        0.0.0.0
//   Broadcast:      0.0.0.0
//   Iface index:    2
//   Status:         interface up

//lo:
//   HW address:     00:00:00:00:00:00
//   IP address:     127.0.0.1
//   IPv6 addresses: (none)
//   Netmask:        255.0.0.0
//   Broadcast:      0.0.0.0
//   Iface index:    1
//   Status:         interface up

//wlp2s0:
//   HW address:     0c:96:e6:16:7b:bd
//   IP address:     0.0.0.0
//   IPv6 addresses: (none)
//   Netmask:        0.0.0.0
//   Broadcast:      0.0.0.0
//   Iface index:    3
//   Status:         interface up

//wlxe84e06355627:
//   HW address:     e8:4e:06:35:56:27
//   IP address:     192.168.249.105
//   IPv6 addresses: fe80::8539:1dcf:f278:20b2/64
//   Netmask:        255.255.255.0
//   Broadcast:      192.168.249.255
//   Iface index:    8
//   Status:         interface up
