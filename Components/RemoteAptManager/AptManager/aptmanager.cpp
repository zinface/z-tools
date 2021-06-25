#include "aptmanager.h"
#include "packageview.h"
#include "packageviewmodel.h"

#include <QComboBox>
#include <QDebug>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <remoteaptmanager.h>

AptManager::AptManager(QWidget *parent) : QWidget(parent)
  ,m_packageCategoryLab(new QLabel("软件包分类:"))
  ,m_packageArchCategory(new QComboBox)
  ,m_packageCategory(new QComboBox)
  ,mSearchLab(new QLabel("软件包名称:"))
  ,mSearchEdit(new QLineEdit)
  ,mSearchBtn(new QPushButton("搜索"))
  ,m_packageView(new PackageView)
  ,m_statusBar(new QLabel)
  ,workhost("localhost")
  ,workport(8888)
  ,timer(new QTimer())
  ,remoteLocal(QString("0"))
  ,remoteMirrors(QString("0"))
{
    createAptManager();
    onSearchPackage();

    connect(mSearchBtn, &QPushButton::clicked, this, &AptManager::onSearchPackage);
    connect(mSearchEdit, &QLineEdit::textChanged, this, &AptManager::onPackageChange);
    connect(m_packageCategory, &QComboBox::currentTextChanged, this, &AptManager::onCategoryChange);
    connect(m_packageArchCategory, &QComboBox::currentTextChanged, this, &AptManager::onArchCategoryChange);

    setFixedSize(580, 500);

    connect(timer,&QTimer::timeout, this, &AptManager::onTimerout);
    connect(&manager, &InfomationManager::onReply, this, &AptManager::onReply);
    connect(&manager, &InfomationManager::onReplyRaw, this, &AptManager::onReplyRaw);
    connect(&manager, &InfomationManager::connected, this, &AptManager::onConnected);
    connect(&manager, &InfomationManager::ClientSocketUnConnected, this, &AptManager::onUnConnected);

//    void registerAction(int action = -1, bool raw = false);
    manager.registerAction(RemoteAptManager::FetchPackageList, true);
    manager.registerAction(RemoteAptManager::RemoteLocalPackageCount);
    manager.registerAction(RemoteAptManager::RemoteMirrorsPackageCount);
}

bool AptManager::setWorkHost(QString host)
{
    this->workhost = host;
    return true;
}

bool AptManager::setWorkPort(int port)
{
    this->workport = port;
    return true;
}

bool AptManager::start()
{
    manager.setManagerTask(this->workhost, this->workport, SessionManager::CLIENT);
    this->show();
    return true;
}

void AptManager::onSearchPackage()
{
    onPackageChange();
    m_packageView->setPackages(plist);
}

void AptManager::onPackageChange()
{
    emit m_packageView->setPackageName(mSearchEdit->text());
}

void AptManager::onCategoryChange()
{
    emit m_packageView->setInstallCategory(m_packageCategory->currentData().toInt());
}

void AptManager::onArchCategoryChange()
{
    emit m_packageView->setArchCategory(m_packageArchCategory->currentData().toInt());
}

void AptManager::onConnected()
{
    manager.broadCaseAction(RemoteAptManager::FetchPackageList);
    timer->setInterval(1000);
    timer->start();
    onTimerout();
}

void AptManager::onUnConnected()
{
    //    exit(0);
}

void AptManager::onReply(int action, QString msg)
{
    if (action == RemoteAptManager::FetchPackageList) {
//        QTextStream(stdout) << QString("FetchPackageList - %1\n").arg(msg.length());
//        if (!msg.isEmpty() && msg.contains(";")) {
//            foreach(QString package, msg.split(";")) {
//                PackageInfo *info = new PackageInfo;
//                info->setName(msg);
//                this->m_packageView->appendPackage(info);
//                QTextStream(stdout) << QString("%1\n").arg(msg);
//            }
//        }
    } else if (action == RemoteAptManager::Name) {

    } else if (action == RemoteAptManager::Version) {

    } else if (action == RemoteAptManager::Arch) {

    } else if (action == RemoteAptManager::Installed) {

    } else if (action == RemoteAptManager::RemoteLocalPackageCount) {
        this->remoteLocal = msg;
        updateStatus();
    } else if (action == RemoteAptManager::RemoteMirrorsPackageCount) {
        this->remoteMirrors = msg;
        updateStatus();
    }
}

void AptManager::onReplyRaw(qint8 action, qint64 length, QByteArray &data)
{
    if (action == RemoteAptManager::FetchPackageList) {
        QTextStream(stdout) << QString("FetchPackageList - %1\n").arg(length);
        QString msg = QString::fromLocal8Bit(data);
        QTextStream(stdout) << QString("FetchPackageList - %1\n").arg(msg.length());
        if (!msg.isEmpty() && msg.contains(";")) {
            foreach(QString package, msg.split(";")) {
                PackageInfo *info = new PackageInfo;
                QStringList packageInfo = package.split(":");
                info->setName(packageInfo[0]);
                info->setVersion(packageInfo[1]);
                info->setArchitecture(packageInfo[2]);
                bool installed = packageInfo[3] == "true"?true:false;
                info->setInstalled(installed);
                plist << info;
//                this->m_packageView->appendPackage(info);
            }
        }
        this->m_packageView->setPackages(plist);
    }
}
//    QApt::PackageList plist = m_backend->availablePackages();

//#if 0
//    qDebug() << m_backend->nativeArchitecture();
////    "amd64"
//    foreach (const QString& item, m_backend->origins()) {
//        qDebug() << item;
//    }
////    "Linux Deepin"
////    "Uos"
////    "typora.io"
////    "com.github.linux-surface.linux-surface"
//    qDebug() << "---------------------------";
//    qDebug() << m_backend->originLabel("Uos");
////    "Uos_eagle"
//    qDebug() << "---------------------------";
//    foreach (const QString& item, m_backend->originLabels()) {
//        qDebug() << item;
//    }
////    "Uos_eagle"
////    "linux-surface"
////    "Typora"
////    "Deepin"
//    qDebug() << "---------------------------";
//    foreach (const QString& item, m_backend->originsForHost("localhost")) {
//        qDebug() << item;
//    }

//    qDebug() << m_backend->isBroken(); // false
//    qDebug() << m_backend->installSize(); // 0

//    qDebug() << "---------------------------";
//    qDebug() << m_backend->availablePackages().size();

//    foreach (QApt::Package* item, m_backend->availablePackages()) {
//        qDebug() << item->name() << item->architecture();
//    }
//#else
////    foreach (QApt::Package* item, m_backend->availablePackages()) {
//////        qDebug() << item->name() << item->architecture();
////        m_packageView->appendPackage(*item);
////    }

//#endif


//    QDebug
//    connect(mSearchButton, &QPushButton::clicked, this, &SearchPackage::onSearchPackage);
//    connect(mSearchLine, &QLineEdit::textChanged, this, &SearchPackage::onSearchPackage);
//}


void AptManager::createAptManager()
{
    m_statusBar->setText("............");

    m_packageArchCategory->addItem("未指定", PackageViewModel::ANY_ARCH);
    m_packageArchCategory->addItem("i386", PackageViewModel::ONLY_I386);
    m_packageArchCategory->addItem("amd64", PackageViewModel::ONLY_AMD64);
    m_packageArchCategory->addItem("arm64", PackageViewModel::ONLY_ARM64);


    m_packageCategory->addItem("全部", PackageViewModel::ALL);
    m_packageCategory->addItem("已安装", PackageViewModel::ONLY_INSTALLER);
    m_packageCategory->addItem("未安装 ", PackageViewModel::ONLY_UNINSTALLER);


    QGroupBox *searchBox = new QGroupBox("Search Package");

    QGridLayout *searchLayout = new QGridLayout;

    QGridLayout *resultCategoryLayout = new QGridLayout;
    resultCategoryLayout->addWidget(m_packageCategoryLab, 0,0,1,1);
    resultCategoryLayout->addWidget(m_packageArchCategory, 0,1,1,1);
    resultCategoryLayout->addWidget(m_packageCategory, 0,2,1,2);

    searchLayout->addLayout(resultCategoryLayout,0,0,1,4);

    searchLayout->addWidget(mSearchLab, 1,0,1,1);
    searchLayout->addWidget(mSearchEdit, 1,1,1,2);
    searchLayout->addWidget(mSearchBtn, 1,3,1,1);


//    QHBoxLayout *searchLayout = new QHBoxLayout;
//    searchLayout->addWidget(mSearchEdit);
//    searchLayout->addWidget(mSearchBtn);
    searchBox->setLayout(searchLayout);

    QGroupBox *resultBox = new QGroupBox("Result");

    QVBoxLayout *resultLayout = new QVBoxLayout;

//    QHBoxLayout *resultCategoryLayout = new QHBoxLayout;
//    resultCategoryLayout->addWidget(m_packageCategoryLab,1);
//    resultCategoryLayout->addWidget(m_packageCategory,2,Qt::AlignRight);

//    resultLayout->addLayout(resultCategoryLayout);
    resultLayout->addWidget(m_packageView);
    resultBox->setLayout(resultLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(searchBox);
    mainLayout->addWidget(resultBox);
    mainLayout->addWidget(m_statusBar);

    setLayout(mainLayout);

    updateStatus();
}

void AptManager::updateStatus()
{
    m_statusBar->setText(QString("本地已安装：%1 镜像源：%2")
                         .arg(remoteLocal)
                         .arg(remoteMirrors));
}

void AptManager::onTimerout()
{
    manager.broadCaseAction(RemoteAptManager::RemoteMirrorsPackageCount);
    manager.broadCaseAction(RemoteAptManager::RemoteLocalPackageCount);
}



