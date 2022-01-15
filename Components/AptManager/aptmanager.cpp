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
#include <QTimer>

#include <AptUtils/aptutils.h>
#include <qnamespace.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qwindowdefs.h>

#include <QThread>

AptManager::AptManager(QWidget *parent) : QWidget(parent)
  ,m_statusBar(new QLabel)
{

    /******* 架构类别 和 结果过滤控制 *******/
    QLabel *packageCategoryLabel = new QLabel("软件包分类:");

    QComboBox *m_packageArchCategory = new QComboBox(this);
    m_packageArchCategory->addItem("未指定", PackageViewModel::ANY_ARCH);
    m_packageArchCategory->addItem("i386", PackageViewModel::ONLY_I386);
    m_packageArchCategory->addItem("amd64", PackageViewModel::ONLY_AMD64);
    m_packageArchCategory->addItem("arm64", PackageViewModel::ONLY_ARM64);

    QComboBox *packageInstalledCategory = new QComboBox(this);
    packageInstalledCategory->addItem("全部", PackageViewModel::ALL);
    packageInstalledCategory->addItem("已安装", PackageViewModel::ONLY_INSTALLER);
    packageInstalledCategory->addItem("未安装 ", PackageViewModel::ONLY_UNINSTALLER);
    packageInstalledCategory->addItem("可更新", PackageViewModel::ONLY_UPGRADLEABLE);


    /******** 搜索输入框 和 搜索按钮 ******/
    QLabel *packageSearchLabel = new QLabel("软件包名称:", this);
    QLineEdit *packageSearchEdit = new QLineEdit(this);
    QPushButton *packageSearchButton = new QPushButton("搜索", this);


    /******* 搜索控制框布局 ******/
    QGridLayout *searchControlLayout = new QGridLayout;
    // searchControlLayout->addLayout(resultCategoryLayout,0,0,1,4);

    searchControlLayout->addWidget(packageCategoryLabel, 0,0,1,1);
    searchControlLayout->addWidget(m_packageArchCategory, 0,1,1,1);
    searchControlLayout->addWidget(packageInstalledCategory, 0,2,1,2);
    searchControlLayout->addWidget(packageSearchLabel, 1,0,1,1);
    searchControlLayout->addWidget(packageSearchEdit, 1,1,1,2);
    searchControlLayout->addWidget(packageSearchButton, 1,3,1,1);

    QGroupBox *searchControlBox = new QGroupBox("Search Package");
    searchControlBox->setLayout(searchControlLayout);

    /******* 结果展示Widget ******/
    QGroupBox *packageResultBox = new QGroupBox("Result");

    QVBoxLayout *resultLayout = new QVBoxLayout;
    PackageView *m_packageView = new PackageView;

    resultLayout->addWidget(m_packageView);
    packageResultBox->setLayout(resultLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(searchControlBox);
    mainLayout->addWidget(packageResultBox);
    mainLayout->addWidget(m_statusBar);

    setLayout(mainLayout);

    setTabOrder(packageSearchEdit, m_packageArchCategory);
    setTabOrder(m_packageArchCategory, packageInstalledCategory);

    m_statusBar->setText(QString("本地已安装: %1 可更新: %2 镜像源: %3")
                                     .arg(mAptUtil.GetInstalledPackagesCount())
                                     .arg(mAptUtil.GetUpgradablePackagesCount())
                                     .arg(mAptUtil.GetMirrorsPackagesCount()));

    
    // 处理改变架构选择项的部分
    connect(m_packageArchCategory, &QComboBox::currentTextChanged, [=]{
        emit m_packageView->setArchCategory(m_packageArchCategory->currentData().toInt());
    });
    // 处理改变全部、未安装、已安装选择项部分
    connect(packageInstalledCategory, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), [=](int index){
        emit m_packageView->setInstallCategory(packageInstalledCategory->currentData().toInt());
    });
    // 处理实时搜索
    connect(packageSearchEdit, static_cast<void (QLineEdit::*)(const QString &)>(&QLineEdit::textChanged), [=](const QString &text){
        emit m_packageView->setPackageName(text);
    });
    // 处理点击按钮
    connect(packageSearchButton, static_cast<void (QPushButton::*)(bool)>(&QPushButton::clicked), [=](bool checked){
        emit m_packageView->setPackageName(packageSearchEdit->text());
    });

    setFixedSize(580, 500);
    setContentsMargins(0,0,0,0);
    
    m_packageView->setPackages(mAptUtil.GetPackageList());
    AptUtils *hAptUtil = new AptUtils;

    connect(this, &AptManager::packageChange, this, &AptManager::onPackageChange);   
    // 使用一个线程来检测是否有最新数据
    QThread *thread = new QThread(this);
    connect(thread, &QThread::started, [=]{
        QTimer *timer = new QTimer(this);
        timer->setInterval(500);
        timer->start();
        connect(timer, &QTimer::timeout, [=]{
            timer->stop();
            
            QTextStream(stdout) << "Check!\n";
                hAptUtil->reload();
                if (hAptUtil->GetInstalledPackagesCount() != mAptUtil.GetInstalledPackagesCount()) {
                    emit packageChange(m_packageView);
                }
            QTextStream(stdout) << "Finish!\n";

            timer->start();
        });
    });
    thread->start();
}

void AptManager::onPackageChange(PackageView *m_packageView) {
    mAptUtil.reload();
    m_packageView->setPackages(mAptUtil.GetPackageList());
};


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


