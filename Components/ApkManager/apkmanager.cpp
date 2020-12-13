#include "apkmanager.h"

#include <QDir>
#include <QFileDialog>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>

ApkManager::ApkManager(QWidget *parent) : QWidget(parent)
  ,m_apkNameLabel(new QLabel("应用名称:"))
  ,m_apkVersionLabel(new QLabel("应用版本:"))
  ,m_apkPackageLabel(new QLabel("应用包名:"))
  ,m_apkName(new QLabel("null"))
  ,m_apkVersion(new QLabel("null"))
  ,m_apkPackage(new QLabel("null"))
  ,currentApkPath(QString(""))
  ,log(new QTextEdit)
{
    initUi();

    setFixedSize(500,480);
}

void ApkManager::initUi()
{
    QLabel *apkPathLabel = new QLabel("apk路径:");
    apkPathLine = new QLineEdit;
    apkPathLine->setReadOnly(true);
    QPushButton *apkFileChoose = new QPushButton("选择apk文件");


    QHBoxLayout *apkFileChooseLayout = new QHBoxLayout;
    apkFileChooseLayout->addWidget(apkPathLine);
    apkFileChooseLayout->addWidget(apkFileChoose);

    QGroupBox *apkGroupBox = new QGroupBox("Apk Infomation");
    QGridLayout *apkinfomationLayout = new QGridLayout;
    apkinfomationLayout->addWidget(apkPathLabel, 0,0,1,1);
    apkinfomationLayout->addLayout(apkFileChooseLayout, 0,1,1,2);

    apkinfomationLayout->addWidget(m_apkNameLabel, 1,0,1,1);
    apkinfomationLayout->addWidget(m_apkName, 1,1,1,2);
    apkinfomationLayout->addWidget(m_apkVersionLabel, 2,0,1,1);
    apkinfomationLayout->addWidget(m_apkVersion, 2,1,1,2);
    apkinfomationLayout->addWidget(m_apkPackageLabel, 3,0,1,1);
    apkinfomationLayout->addWidget(m_apkPackage, 3,1,1,2);
    apkGroupBox->setLayout(apkinfomationLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(apkGroupBox);
    mainLayout->addWidget(log);
    mainLayout->addStretch();

    setLayout(mainLayout);

    connect(apkFileChoose, &QPushButton::clicked, this, &ApkManager::onApkFileChoose);
}

void ApkManager::onChoosedApkFile()
{
    log->clear();
    QFile file(currentApkPath);

    log->append("tmppath: "+QDir::tempPath());
    if(file.exists()) {
        log->append("find file path: " + currentApkPath);
        log->append("find file: " + QFileInfo(file).fileName());
//        file.copy(QDir::tempPath()+"/"+QFileInfo(file).fileName());
    }
    QProcess process;
    log->append("working dir: " + QFileInfo(file).dir().path());
    process.setWorkingDirectory(QFileInfo(file).dir().path());
//    currentApkPath.replace(' ',"\\ ");
//    process.start("aapt dump badging " + QDir::tempPath()+"/"+QFileInfo(file).fileName());
//    process.start("aapt dump badging " + currentApkPath);
    process.start("aapt", QStringList() << "dump" << "badging" << QFileInfo(file).fileName());
    process.waitForFinished();
    targetPath = QFileInfo(file).dir().path();
    QByteArray stde = process.readAllStandardError();
    log->append("exit :" + QString::number(process.exitCode()));
    log->append("..........................");
    log->append(QString::fromLocal8Bit(stde).trimmed());

    QByteArray stdo = process.readAllStandardOutput();
    QString result = QString::fromLocal8Bit(stdo).trimmed();
    QStringList lines = result.split('\n');

//    log->append(result);

    QString zh;
    QString en;

    foreach(QString line, lines) {
// package: name='cn.androidfun.apkfetch' versionCode='20' versionName='2.5.1' platformBuildVersionName=''
// application-label-zh:'提取应用APK安装包'
        if(line.startsWith("package: ")) {
            QString packagename = line.split(' ').at(1).split("\'").at(1);
            QString versionCode = line.split(' ').at(2).split("\'").at(1);
            QString versionName = line.split(' ').at(3).split("\'").at(1);
            m_apkVersion->setText(versionName);
            m_apkPackage->setText(packagename);
            log->append("search packagename: " + packagename);
            log->append("search versionName: " + versionName);
        }
        if(line.startsWith("application-label-zh:")) {
            QString name = line.split("\'").at(1);
            zh = name;
            log->append("search apkName(zh): " + name);
        }
        if(line.startsWith("application-label:")) {
            QString name = line.split("\'").at(1);
            en = name;
            log->append("search apkName(en): " + name);
        }
    }
    if(!zh.isEmpty()) {
        m_apkName->setText(zh);
    } else {
        m_apkName->setText(en);
    }
}

void ApkManager::onApkFileChoose()
{
    if(targetPath.isEmpty()) {
        targetPath=QDir::homePath();
    }
    QString filePath = QFileDialog::getOpenFileName(this, "选择apk文件",targetPath,"apk 文件 (*.apk)");
    if (!filePath.endsWith(".apk")) {
        return;
    }
    currentApkPath = filePath;
    apkPathLine->setText(filePath);
    onChoosedApkFile();
}
