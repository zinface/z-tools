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
#include <qboxlayout.h>
#include <qframe.h>
#include <QScrollArea>
#include <qnamespace.h>
#include <qobject.h>
#include <qobjectdefs.h>
#include <qwindowdefs.h>

#include <QThread>

QString numSize(qint64 size){
    if (size / 1000 / 1000 < 10){
        if (size / 1000 < 10){
            if (size < 100){
                return QString(QString::number(size)+"b");
            }
            return QString(QString::number(size/1000)+"Kb");
        }
        return QString(QString::number(size/1000)+"Kb");
    }
    return QString(QString::number(size/1000/1000)+"Mb");
}



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


    QHBoxLayout *layout = new QHBoxLayout(this);
	
// left
    QVBoxLayout *LeftLayout = new QVBoxLayout;   

// 1.1
    QGroupBox *leftBox = new QGroupBox("软件包查询");
    QVBoxLayout *leftBoxLayout = new QVBoxLayout(leftBox);
    leftBox->setFixedWidth(564);
    leftBoxLayout->addWidget(searchControlBox);
    leftBoxLayout->addWidget(packageResultBox);
    leftBoxLayout->addWidget(m_statusBar);
    LeftLayout->addWidget(leftBox);


// right
    QVBoxLayout *Rightlayout = new QVBoxLayout();

// 2.1
    QGroupBox *rightBox = new QGroupBox("信息展示");
    QVBoxLayout *rightBoxLayout = new QVBoxLayout(rightBox);
    Rightlayout->addWidget(rightBox);

    QLabel *packageLabel = new QLabel("Package:");
    QLineEdit *package = new QLineEdit();
    // package->setWordWrap(true);
    QLabel *versionLabel = new QLabel("Version:");
    QLineEdit *version = new QLineEdit();
    // version->setWordWrap(true);
    QLabel *priorityLabel = new QLabel("Priority:");
    QLineEdit *priority = new QLineEdit();
    // priority->setWordWrap(true);
    QLabel *sectionLabel = new QLabel("Section:");
    QLineEdit *section = new QLineEdit();
    // section->setWordWrap(true);
    QLabel *sourceLabel = new QLabel("Source:");
    QLineEdit *source = new QLineEdit();
    // source->setWordWrap(true);
    QLabel *maintainerLabel = new QLabel("Maintainer:");
    QLineEdit *maintainer = new QLineEdit();
    // maintainer->setWordWrap(true);
    QLabel *installedLabel = new QLabel("Installed-Size:");
    QLineEdit *installed = new QLineEdit();
    // installed->setWordWrap(true);
    QLabel *suggestsLabel = new QLabel("Suggests:");
    QLineEdit *suggests = new QLineEdit();
    // suggests->setWordWrap(true);
    QLabel *homepageLabel = new QLabel("Homepage:");
    QLineEdit *homepage = new QLineEdit();
    // homepage->setWordWrap(true);
    QLabel *descriptionLabel = new QLabel("Description:");
    QLineEdit *sdescription = new QLineEdit();
    // sdescription->setWordWrap(true);
    QLabel *ldescription = new QLabel(" Flatpak installs, manages and runs sandboxed desktop application bundles.\n See the flatpak package for a more comprehensive description.\n .\n This package contains reference documentation for libflatpak0.");
    ldescription->setWordWrap(true);

    QGridLayout *packageVerboseLayout = new QGridLayout();
    packageVerboseLayout->addWidget(packageLabel,     0, 0);
    packageVerboseLayout->addWidget(package,          0, 1);
    packageVerboseLayout->addWidget(versionLabel,     1, 0);
    packageVerboseLayout->addWidget(version,          1, 1);
    packageVerboseLayout->addWidget(sectionLabel,     2, 0);
    packageVerboseLayout->addWidget(section,          2, 1);
    packageVerboseLayout->addWidget(sourceLabel,      3, 0);
    packageVerboseLayout->addWidget(source,           3, 1);
    packageVerboseLayout->addWidget(maintainerLabel,  4, 0);
    packageVerboseLayout->addWidget(maintainer,       4, 1);
    packageVerboseLayout->addWidget(installedLabel,   5, 0);
    packageVerboseLayout->addWidget(installed,        5, 1);
    packageVerboseLayout->addWidget(suggestsLabel,    6, 0);
    packageVerboseLayout->addWidget(suggests,         6, 1);
    packageVerboseLayout->addWidget(homepageLabel,    7, 0);
    packageVerboseLayout->addWidget(homepage,         7, 1);
    packageVerboseLayout->addWidget(descriptionLabel, 8, 0);
    packageVerboseLayout->addWidget(sdescription,     8, 1);
    rightBoxLayout->addLayout(packageVerboseLayout);

    QFrame *sframe = new QFrame;
    QVBoxLayout *sframeLayout = new QVBoxLayout;
    sframe->setLayout(sframeLayout);

    QScrollArea *scrollWidget = new QScrollArea;
    scrollWidget->setWidget(sframe);
    scrollWidget->setWidgetResizable(true);
    sframeLayout->addWidget(ldescription);
    sframeLayout->addStretch();

    // QGroupBox *packageDescriptionGroup = new QGroupBox("description");
    rightBoxLayout->addWidget(scrollWidget);
    // rightBoxLayout->addStretch();/
    
    layout->addLayout(LeftLayout,1);
    layout->addLayout(Rightlayout,1);
    // setLayout(layout);

    resize(1155, 691);
    // setFixedSize(580, 500);
    setContentsMargins(0,0,0,0);


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

    connect(m_packageView, &PackageView::currentPackageChanged, [=](QApt::Package *_package){
        package->setText(_package->name());
        version->setText(_package->version());
        priority->setText(_package->priority());
        section->setText(_package->section());
        source->setText(_package->sourcePackage());
        maintainer->setText(QString("%1").arg(_package->maintainer()).replace("&lt;", "<"));
        // _package->depends().
        installed->setText(numSize(_package->currentInstalledSize()));
        if (_package->currentInstalledSize() < 0) {
            packageVerboseLayout->itemAt(10)->widget()->setHidden(true);
            packageVerboseLayout->itemAt(11)->widget()->setHidden(true);
        } else {
            packageVerboseLayout->itemAt(10)->widget()->setHidden(false);
            packageVerboseLayout->itemAt(11)->widget()->setHidden(false);
        }
        suggests->setText(_package->suggestsList().join(","));
        if (_package->suggestsList().size() == 0) {
            packageVerboseLayout->itemAt(12)->widget()->setHidden(true);
            packageVerboseLayout->itemAt(13)->widget()->setHidden(true);
        } else {
            packageVerboseLayout->itemAt(12)->widget()->setHidden(false);
            packageVerboseLayout->itemAt(13)->widget()->setHidden(false);
        }
        homepage->setText(_package->homepage());
        sdescription->setText(_package->shortDescription());
        ldescription->setText(_package->longDescription());
    });


    
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
    m_statusBar->setText(QString("本地已安装: %1 可更新: %2 镜像源: %3")
                                     .arg(mAptUtil.GetInstalledPackagesCount())
                                     .arg(mAptUtil.GetUpgradablePackagesCount())
                                     .arg(mAptUtil.GetMirrorsPackagesCount()));
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


