#ifndef NETCARDMANAGER_H
#define NETCARDMANAGER_H

#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <tins/network_interface.h>


using namespace Tins;

class NetcardManager : public QWidget
{
    Q_OBJECT
public:
    explicit NetcardManager(QWidget *parent = nullptr);

private slots:
    void refreshInterfaces();
    void setIfaceInfomation(int index);

private:
    QGroupBox *ifaceGroupBox;
    QComboBox *ifaceComb;
    QLabel *ifaceLab;

    QLabel *ifaceHWLab;
    QLabel *ifaceIPv4Lab;
    QLabel *ifaceIPv6Lab;
    QLabel *ifaceNetMaskLab;
    QLabel *ifaceBroadcastLab;
    QLabel *ifaceIndexLab;
    QLabel *ifaceStatusLab;

    QLabel *ifaceHW;
    QLabel *ifaceIPv4;
    QLabel *ifaceIPv6;
    QLabel *ifaceNetMask;
    QLabel *ifaceBroadcast;
    QLabel *ifaceIndex;
    QLabel *ifaceStatus;

    QPushButton *refresh;

    NetworkInterface *tni;

    QMap<QString, QMap<QString, QString>> mapInfo;

    void createNetworkInfomation();
};


#endif // NETCARDMANAGER_H
