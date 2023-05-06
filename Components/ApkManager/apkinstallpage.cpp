#include "apkinstallpage.h"
#include "utils/UEngine.h"

#include <QGroupBox>
#include <QMessageBox>
#include <QMovie>
#include <QPushButton>
#include <QTextStream>
#include <QThread>
#include <QVBoxLayout>


ApkInstallPage::ApkInstallPage(QWidget *parent) : QWidget(parent)
  , uengine(new UEngine(parent))
  , progressLabel(new QLabel(this))
  , logLabel(new QLabel(this))
  , movie(new QMovie("://resources/loading.gif"))
{
    setWindowTitle("安装 apk");
    logLabel->setWordWrap(true);

    QGroupBox *uengineBox = new QGroupBox("UEngine");
    QHBoxLayout *uengineLayout = new QHBoxLayout(uengineBox);

    uengineBtn = new QPushButton("安装到 UEngine");
    uengineLayout->addWidget(uengineBtn);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addStretch();
    centralLayout->addWidget(progressLabel);
    centralLayout->setAlignment(progressLabel, Qt::AlignmentFlag::AlignHCenter);
    centralLayout->addWidget(logLabel);
    centralLayout->setAlignment(logLabel, Qt::AlignmentFlag::AlignHCenter | Qt::AlignmentFlag::AlignLeft);
    centralLayout->addStretch();
    centralLayout->setSpacing(0);
//    centralLayout->setContentsMargins(30, 20, 30, 20);
    centralLayout->setContentsMargins(0,10,0,0);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(uengineBox);
    mainLayout->addLayout(centralLayout);
    mainLayout->addStretch();

    if (UEngine().checkCommandUEngine()) {
        uengineBtn->setEnabled(true);
    } else {
        uengineBtn->setEnabled(false);
        uengineBtn->setText("未安装 uengine");
    }

    connect(uengineBtn, &QPushButton::clicked, this, &ApkInstallPage::onInstallToEngine);
    connect(uengine, &UEngine::logChanged, this, &ApkInstallPage::onInstallLog);
    connect(uengine, &UEngine::finished, this, &ApkInstallPage::onInstalled);

    setFixedSize(440,300);
}

void ApkInstallPage::setApk(QString apkPath) {
    this->logLabel->clear();
    this->apkPath = apkPath;
}

void ApkInstallPage::onInstallToEngine()
{
    uengineBtn->setText("安装中...");
    uengineBtn->setEnabled(false);


    progressLabel->setMovie(movie);
    progressLabel->show();
    movie->start();


    logLabel->clear();
    emit uengine->logChanged("--- 准备安装 APK---\n");

    uengine->doAsyncInstall(apkPath);
}

void ApkInstallPage::onInstallLog(QString log)
{
    logLabel->setText(logLabel->text() + log);
    QTextStream(stdout) << log;
}

void ApkInstallPage::onInstalled(int exitCode)
{
    uengineBtn->setText("安装到 UEngine");
    uengineBtn->setEnabled(true);

    progressLabel->hide();
    movie->stop();

    logLabel->setText(uengine->logs.join(""));
}

