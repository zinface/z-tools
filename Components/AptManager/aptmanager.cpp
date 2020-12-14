#include "aptmanager.h"
#include "packageview.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

#include <AptUtils/aptutils.h>

AptManager::AptManager(QWidget *parent) : QWidget(parent)
  ,mSearchLine(new QLineEdit)
  ,mSearchButton(new QPushButton("搜索"))
  ,m_packageView(new PackageView)
{
    initUi();
    onSearchPackage();

    setFixedSize(580, 400);
}

void AptManager::onSearchPackage()
{

    if(!mSearchLine->text().isEmpty()) {
        m_packageView->setPackages(AptUtils::GetPackageListContians(m_backend, mSearchLine->text()));
    } else {
        m_packageView->setPackages(AptUtils::GetPackageList(m_backend));
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


void AptManager::initUi()
{
    m_backend = new QApt::Backend;
    m_backend->init();
    m_backend->reloadCache();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(mSearchLine);
    searchLayout->addWidget(mSearchButton);
    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(m_packageView);

    m_packageStatusBarLabel = new QLabel();
    m_packageStatusBarLabel->setText("............");
    mainLayout->addWidget(m_packageStatusBarLabel);

    setLayout(mainLayout);


    m_packageStatusBarLabel->setText(QString("本地已安装：%1 镜像源：%2")
                                     .arg(AptUtils::GetInstalledPackagesCount(m_backend))
                                     .arg(AptUtils::GetMirrorsPackagesCount(m_backend)));

    connect(mSearchButton, &QPushButton::clicked, this, &AptManager::onSearchPackage);
    connect(mSearchLine, &QLineEdit::textChanged, this, &AptManager::onSearchPackage);
}



