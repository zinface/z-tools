#include "adbmanager.h"
#include "adbpkgview.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>

#include <AdbUtils/adbutils.h>

AdbManager::AdbManager(QWidget *parent) : QWidget(parent)
  ,adbutils(new AdbUtils)
{
    createAdbInfomation();
    createAdbPackageLayout();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(adbGroupBox);
    mainLayout->addWidget(adbPackageGroupBox);
//    mainLayout->addStretch();
    this->setLayout(mainLayout);

    connect(refresh, &QPushButton::clicked, this, &AdbManager::refreshDevices);
    connect(deviceComb, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AdbManager::setDeviceInfomation);
    connect(m_packageTagComb, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &AdbManager::setPackageMgtag);

    this->refresh->click();
    setFixedSize(500,480);
}

void AdbManager::refreshDevices()
{
    deviceComb->clear();

    QStringList devices = adbutils->GetDevices();
    foreach(const QString& device, devices) {
        deviceComb->addItem(device, device);
    }
}

void AdbManager::setDeviceInfomation(int index)
{
    QString item = deviceComb->itemText(index);
    deviceBrand->setText(adbutils->GetBrand(item));
    deviceModel->setText(adbutils->GetModel(item));
    deviceVersion->setText("Android "+adbutils->GetAndroidVersion(item));
    devicePhysical->setText(adbutils->GetAndroidWmSize(item));
    m_currentDevice = deviceComb->itemText(index);
    m_packageTagComb->setCurrentIndex(0);
    setPackageMgtag(0);
}

void AdbManager::setPackageMgtag(int index)
{
    QVariant tag = m_packageTagComb->itemData(index);
    switch (tag.toInt()) {
        case AdbUtils::ONLY_ENABLED:  m_AdbPkgView->setData(AdbUtils::GetAndroidPackages(m_currentDevice,"-e","") ) ;break;
        case AdbUtils::ONLY_DISABLED: m_AdbPkgView->setData(AdbUtils::GetAndroidPackages(m_currentDevice, "-d","")) ;break;
        case AdbUtils::ONLY_SYSTEM:   m_AdbPkgView->setData(AdbUtils::GetAndroidPackages(m_currentDevice,"-s","") ) ;break;
        case AdbUtils::ONLY_THRID:    m_AdbPkgView->setData(AdbUtils::GetAndroidPackages(m_currentDevice,"-3","") ) ;break;
    }
}

void AdbManager::createAdbInfomation()
{
    adbGroupBox = new QGroupBox("Android");
    adbDeviceLabel = new QLabel("选择设备：");
    adbBrandLabel = new QLabel("厂商：");
    adbModelLabel = new QLabel("型号：");
    adbVersionLabel = new QLabel("版本：");
    adbPhysicalLabel = new QLabel("屏幕：");

    deviceComb = new QComboBox;
    deviceBrand = new QLabel("null");
    deviceModel = new QLabel("null");
    deviceVersion = new QLabel("null");
    devicePhysical = new QLabel("null");

    refresh = new QPushButton("刷新");

    QGridLayout *android = new QGridLayout;
    android->addWidget(adbDeviceLabel, 0,0,1,1);
    android->addWidget(deviceComb, 0,1,1,2);
    android->addWidget(refresh, 0,3,1,1);
    android->addWidget(adbBrandLabel, 1,0,1,1);
    android->addWidget(deviceBrand, 1,1,1,3);
    android->addWidget(adbModelLabel, 2,0,1,1);
    android->addWidget(deviceModel, 2,1,1,3);
    android->addWidget(adbVersionLabel, 3,0,1,1);
    android->addWidget(deviceVersion, 3,1,1,3);
    android->addWidget(adbPhysicalLabel,4,0,1,1);
    android->addWidget(devicePhysical,4,1,1,3);
    adbGroupBox->setLayout(android);

}

void AdbManager::createAdbPackageLayout()
{
    adbPackageGroupBox = new QGroupBox("PackageManager");

    m_packageTagLabel = new QLabel("软件包分类：");

    m_packageTagComb = new QComboBox();
    m_packageTagComb->addItem("已启用软件包",AdbUtils::ONLY_ENABLED);
    m_packageTagComb->addItem("已停用软件包",AdbUtils::ONLY_DISABLED);
    m_packageTagComb->addItem("系统软件",AdbUtils::ONLY_SYSTEM);
    m_packageTagComb->addItem("非系统软件",AdbUtils::ONLY_THRID);
    //    m_packageTagComb->addItem("已安装软件包",AdbUtils::ONLY_INSTALLER);
    //    m_packageTagComb->addItem("已卸载软件包",AdbUtils::ONLY_UNINSTALLER);

    m_AdbPkgView = new AdbPkgView;

    QGridLayout *packageControl = new QGridLayout;
    packageControl->addWidget(m_packageTagLabel, 0,0,1,1);
    packageControl->addWidget(m_packageTagComb, 0,1,1,2);

    QVBoxLayout *packageLayout = new QVBoxLayout;
    packageLayout->addLayout(packageControl);
    packageLayout->addWidget(m_AdbPkgView);

    adbPackageGroupBox->setLayout(packageLayout);

}
