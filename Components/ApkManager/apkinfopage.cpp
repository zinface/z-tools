#include "apkinfopage.h"
#include "apkinstallpage.h"

#include <QDir>
#include <QFileDialog>
#include <QGroupBox>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QMimeData>
#include <QTextStream>
#include <QApplication>

ApkInfoPage::ApkInfoPage(QWidget *parent) : QWidget(parent) 
  ,m_apkIcon(new QLabel)
  ,m_apkName(new QLabel)
  ,m_apkPackage(new QLabel)
  ,m_apkVersion(new QLabel)
  ,m_apkDescription(new QLabel)
{

    m_apkIcon->setText("icon");
    m_apkIcon->setFixedSize(64,64);
    m_apkName->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    m_apkPackage->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_apkVersion->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_apkDescription->setWordWrap(true);
    // m_logText.set


    QLabel *apkName = new QLabel;
    // apkName;
    apkName->setText("应用名称:");
    apkName->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    QLabel *apkPackage = new QLabel;
    // apkPackage;
    apkPackage->setText("应用包名:");
    apkPackage->setAlignment(Qt::AlignCenter | Qt::AlignLeft);

    QLabel *apkVersion = new QLabel;
    // apkVersion;
    apkVersion->setText("应用版本:");
    apkVersion->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QPushButton *installBtn = new QPushButton("安装到...");
    installBtn->setStyleSheet("border: none");
    connect(installBtn, &QPushButton::clicked, this, [=](){
        emit onInstall();
    });

    QGridLayout *itemInfoLayout = new QGridLayout;
    itemInfoLayout->addWidget(apkName, 0, 0);
    itemInfoLayout->addWidget(m_apkName, 0, 1);
    itemInfoLayout->addWidget(apkPackage, 1, 0);
    itemInfoLayout->addWidget(m_apkPackage, 1, 1);
    itemInfoLayout->addWidget(apkVersion, 2, 0);
    itemInfoLayout->addWidget(m_apkVersion, 2, 1);
    
    itemInfoLayout->addWidget(installBtn, 3, 0, 1, 2);
    
    itemInfoLayout->setSpacing(0);
    itemInfoLayout->setVerticalSpacing(5);
    itemInfoLayout->setMargin(0);

    QHBoxLayout *itemLayout = new QHBoxLayout;
    itemLayout->addStretch();
    itemLayout->addWidget(m_apkIcon);
    itemLayout->addLayout(itemInfoLayout);
    itemLayout->addStretch();
    itemLayout->setSpacing(10);
    itemLayout->setContentsMargins(0,0,0,0);
    itemLayout->setAlignment(m_apkIcon, Qt::AlignmentFlag::AlignJustify);

    QVBoxLayout *contentLayout = new QVBoxLayout;
    contentLayout->addStretch();
    contentLayout->addLayout(itemLayout);
    contentLayout->addSpacing(30);
    contentLayout->addStretch();
    contentLayout->addWidget(m_apkDescription);
    // contentLayout->addWidget(m_logText);
    // contentLayout->setSpacing(0);
    contentLayout->setMargin(0);

    QHBoxLayout *centralLayout = new QHBoxLayout;
    centralLayout->addStretch();
    centralLayout->addLayout(contentLayout);
    centralLayout->addStretch();
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(50, 20, 50, 20);

    setLayout(centralLayout);

//    setFixedSize(440,300);
}


void ApkInfoPage::setApk(QString &apk) {
    Aapt::checkCommandsAapt();
    if (checkApk(apk)) {
        m_apkIcon->setPixmap(QIcon(apkIcon).pixmap(m_apkIcon->size()));
        m_apkName->setText(apkName);
        m_apkPackage->setText(QString("'%1'").arg(apkPackage));
        m_apkVersion->setText(apkVersion);
        // m_logText->setText(apkLog);
        m_apkDescription->setText(apkLog);
    }
}
