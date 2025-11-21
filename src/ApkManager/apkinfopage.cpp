#include "apkinfopage.h"
#include "apkinstallpage.h"
#include "qdebug.h"

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
#include <QPainter>

ApkInfoPage::ApkInfoPage(QWidget *parent) : QWidget(parent) 
  ,m_apkIcon(new QLabel)
  ,m_apkName(new QLabel)
  ,m_apkPackage(new QLabel)
  ,m_apkVersion(new QLabel)
  ,m_apkDescription(new QLabel)
{

    // visible infomations
    m_apkIcon->setText("icon");
    m_apkIcon->setFixedSize(64,64);
    m_apkName->setAlignment(Qt::AlignBottom | Qt::AlignLeft);
    m_apkPackage->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
    m_apkVersion->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_apkDescription->setWordWrap(true);

    // detail
    QLabel *apkName = new QLabel;
    apkName->setText("应用名称:");
    apkName->setAlignment(Qt::AlignBottom | Qt::AlignLeft);

    QLabel *apkPackage = new QLabel;
    apkPackage->setText("应用包名:");
    apkPackage->setAlignment(Qt::AlignCenter | Qt::AlignLeft);

    QLabel *apkVersion = new QLabel;
    apkVersion->setText("应用版本:");
    apkVersion->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    QGridLayout *detailLayout = new QGridLayout();
    detailLayout->setMargin(0);
    detailLayout->setSpacing(0);
    detailLayout->setVerticalSpacing(5);
    detailLayout->addWidget(apkName, 0, 0);
    detailLayout->addWidget(m_apkName, 0, 1);
    detailLayout->addWidget(apkPackage, 1, 0);
    detailLayout->addWidget(m_apkPackage, 1, 1);
    detailLayout->addWidget(apkVersion, 2, 0);
    detailLayout->addWidget(m_apkVersion, 2, 1);

    // icon
    QHBoxLayout *iconInfoLayout = new QHBoxLayout;
    iconInfoLayout->addStretch();
    iconInfoLayout->addWidget(m_apkIcon);
    iconInfoLayout->addLayout(detailLayout);
    iconInfoLayout->addStretch();
    iconInfoLayout->setSpacing(10);
    iconInfoLayout->setContentsMargins(0,0,0,0);
    iconInfoLayout->setAlignment(m_apkIcon, Qt::AlignmentFlag::AlignJustify);

    // install
    QPushButton *installAdbBtn = new QPushButton("安装到 ADB");
    connect(installAdbBtn, &QPushButton::clicked, this, [=](){
        emit installAdb();
    });
    installAdbBtn->setStyleSheet("padding: 2px;");

    QPushButton *installUEngineBtn = new QPushButton("安装到 Uengine");
    connect(installUEngineBtn, &QPushButton::clicked, this, [=](){
        emit installUengine();
    });
    installUEngineBtn->setStyleSheet("padding: 2px;");

    QPushButton *installBtn = new QPushButton("安装到...");
    connect(installBtn, &QPushButton::clicked, this, [=](){
        emit installPage();
    });
    installBtn->setStyleSheet("padding: 2px;");

    QHBoxLayout *installLayout = new QHBoxLayout;
    // installLayout->addWidget(installAdbBtn);
    // installLayout->addWidget(installUEngineBtn);
    installLayout->addSpacerItem(new QSpacerItem(0,0));
    installLayout->addWidget(installBtn);
    installLayout->addSpacerItem(new QSpacerItem(0,0));
    installLayout->setStretch(0, 1);
    installLayout->setStretch(1, 3);
    installLayout->setStretch(2, 1);
    detailLayout->addLayout(installLayout, 3, 0, 1, 2);

    // Panel
    QVBoxLayout *panelLayout = new QVBoxLayout;
    panelLayout->addStretch();
    panelLayout->addLayout(iconInfoLayout);
    panelLayout->addStretch();
    panelLayout->addWidget(m_apkDescription);
    panelLayout->setMargin(0);
    panelLayout->setContentsMargins(50, 20, 50, 20);

    setLayout(panelLayout);

//    setFixedSize(440,300);
}

void ApkInfoPage::setApk(QString &apk) {
    if (m_aapt->checkApk(apk)) {
        if (!QIcon(m_aapt->tmpAppIcon).isNull()) {
            m_apkIcon->setPixmap(QIcon(m_aapt->tmpAppIcon).pixmap(m_apkIcon->size()));
        }
        m_apkName->setText(m_aapt->apkName);
        m_apkPackage->setText(QString("'%1'").arg(m_aapt->apkPackage));
        m_apkVersion->setText(m_aapt->apkVersion);
        // m_logText->setText(apkLog);
        m_apkDescription->setText(m_aapt->apkLog);
    }
}

void ApkInfoPage::setAapt(Aapt *newAapt)
{
    m_aapt = newAapt;
}

void ApkInfoPage::paintEvent(QPaintEvent *event)
{
    // [1] 绘制面板
    QWidget::paintEvent(event);

    // [2] 绘制快捷键提示
    QPainter painter(this);
    QRectF frect = painter.boundingRect(event->rect(), hotKey);
    painter.drawText(event->rect().bottomRight() - frect.bottomRight(), hotKey);
}

