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

    QGroupBox *uengine_groupbox = new QGroupBox("UEngine");
    QHBoxLayout *uengine_groupbox_layout = new QHBoxLayout(uengine_groupbox);

    uengine_install_button = new QPushButton("安装到 UEngine");
    uengine_groupbox_layout->addWidget(uengine_install_button);

    QGroupBox *adb_groupgbox = new QGroupBox("Adb");
    QHBoxLayout *adb_groupgbox_layout = new QHBoxLayout(adb_groupgbox);
    QPushButton *adb_install_button = new QPushButton("adb install");
    adb_groupgbox_layout->addWidget(adb_install_button);

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
    mainLayout->addWidget(uengine_groupbox);
    mainLayout->addWidget(adb_groupgbox);
    mainLayout->addLayout(centralLayout);
    mainLayout->addStretch();

    if (UEngine().checkCommandUEngine()) {
        uengine_install_button->setEnabled(true);
    } else {
        uengine_install_button->setEnabled(false);
        uengine_install_button->setText("未安装 uengine");
    }

    connect(uengine_install_button, &QPushButton::clicked, this, &ApkInstallPage::slot_install_uengine);
    connect(adb_install_button, &QPushButton::clicked, this, &ApkInstallPage::slot_install_adb);
    connect(uengine, &UEngine::logChanged, this, &ApkInstallPage::onInstallLog);
    connect(uengine, &UEngine::finished, this, &ApkInstallPage::onInstalled);

    setFixedSize(440,300);
}

void ApkInstallPage::setApk(QString apkPath) {
    this->logLabel->clear();
    this->apkPath = apkPath;
}

void ApkInstallPage::slot_install_uengine()
{
    uengine_install_button->setText("安装中...");
    uengine_install_button->setEnabled(false);


    progressLabel->setMovie(movie);
    progressLabel->show();
    movie->start();


    logLabel->clear();
    emit uengine->logChanged("--- 准备安装 APK---\n");

    uengine->doAsyncInstall(apkPath);
}

void ApkInstallPage::slot_install_adb()
{
//    QProcess process;
//    process.start("adb", QStringList() << "install" << apkPath);
//    process.waitForFinished();
    QProcess *process = new QProcess(this);
    connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit && exitCode == 0) {
            QMessageBox::information(this, "Success", "APK installed successfully.", QMessageBox::Close);
        } else {
            QMessageBox::warning(this, "Error", "Failed to install APK.", QMessageBox::Close);
        }
        process->deleteLater();
    });
    process->start("adb", QStringList() << "install" << apkPath);
}

void ApkInstallPage::onInstallLog(QString log)
{
    logLabel->setText(logLabel->text() + log);
    QTextStream(stdout) << log;
}

void ApkInstallPage::onInstalled(int exitCode)
{
    uengine_install_button->setText("安装到 UEngine");
    uengine_install_button->setEnabled(true);

    progressLabel->hide();
    movie->stop();

    logLabel->setText(uengine->logs.join(""));
}

