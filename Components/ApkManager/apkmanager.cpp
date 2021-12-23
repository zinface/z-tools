#include "apkmanager.h"
#include "apkinfopage.h"

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
#include <QStackedLayout>
#include <QFrame>

ApkManager::ApkManager(QWidget *parent) : QWidget(parent)
,m_centralLayout(new QStackedLayout)
,page(new ApkInfoPage)
{
    // m_centralLayout->addWidget(QWidget *w);
    QLabel *iconImage = new QLabel;
    iconImage->setFixedSize(122,140);
    iconImage->setPixmap(QIcon(":/resources/apk1.png").pixmap(iconImage->size()));


    QLabel *dndTips = new QLabel;
    dndTips->setText("拖动APK文件到此处");
    dndTips->setAlignment(Qt::AlignHCenter);

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(iconImage);
    centralLayout->setAlignment(iconImage, Qt::AlignTop | Qt::AlignHCenter);
    centralLayout->addSpacing(20);
    centralLayout->addWidget(dndTips);
    centralLayout->setAlignment(dndTips, Qt::AlignCenter);
    centralLayout->addSpacing(0);
    centralLayout->setContentsMargins(0,20,0,15);


    QFrame *centerWidget = new QFrame;
    centerWidget->setLayout(centralLayout);

    m_centralLayout->addWidget(centerWidget);
    m_centralLayout->addWidget(page);

    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
    setLayout(m_centralLayout);
    setFixedSize(440,300);
}

void ApkManager::initUi()
{

// New Layout


// Old Layout
    // QLabel *apkPathLabel = new QLabel("apk路径:");
    // apkPathLine = new QLineEdit;
    // apkPathLine->setReadOnly(true);
    // apkPathLine->setAcceptDrops(true);
    // QPushButton *apkFileChoose = new QPushButton("选择apk文件");


    // QHBoxLayout *apkFileChooseLayout = new QHBoxLayout;
    // apkFileChooseLayout->addWidget(apkPathLine);
    // apkFileChooseLayout->addWidget(apkFileChoose);

    // QGroupBox *apkGroupBox = new QGroupBox("Apk Infomation");
    // QGridLayout *apkinfomationLayout = new QGridLayout;
    // apkinfomationLayout->addWidget(apkPathLabel, 0,0,1,1);
    // apkinfomationLayout->addLayout(apkFileChooseLayout, 0,1,1,2);

    // apkinfomationLayout->addWidget(m_apkNameLabel, 1,0,1,1);
    // apkinfomationLayout->addWidget(m_apkName, 1,1,1,2);
    // apkinfomationLayout->addWidget(m_apkVersionLabel, 2,0,1,1);
    // apkinfomationLayout->addWidget(m_apkVersion, 2,1,1,2);
    // apkinfomationLayout->addWidget(m_apkPackageLabel, 3,0,1,1);
    // apkinfomationLayout->addWidget(m_apkPackage, 3,1,1,2);
    // apkGroupBox->setLayout(apkinfomationLayout);

    // QVBoxLayout *mainLayout = new QVBoxLayout;
    // mainLayout->addWidget(apkGroupBox);
    // mainLayout->addWidget(log);
    // // mainLayout->addStretch();

    // setLayout(mainLayout);

    // connect(apkFileChoose, &QPushButton::clicked, this, &ApkManager::onApkFileChoose);


}

// void ApkManager::onChoosedApkFile()
// {
//     log->clear();
//     QFile file(currentApkPath);

//     log->append("tmppath: "+QDir::tempPath());
//     if(file.exists()) {
//         log->append("find file path: " + currentApkPath);
//         log->append("find file: " + QFileInfo(file).fileName());
// //        file.copy(QDir::tempPath()+"/"+QFileInfo(file).fileName());
//     }
//     QProcess process;
//     checkCommandsAapt();
//     log->append("working dir: " + QFileInfo(file).dir().path());
//     process.setWorkingDirectory(QFileInfo(file).dir().path());
// //    currentApkPath.replace(' ',"\\ ");
// //    process.start("aapt dump badging " + QDir::tempPath()+"/"+QFileInfo(file).fileName());
// //    process.start("aapt dump badging " + currentApkPath);
//     process.start(aapt.trimmed(), QStringList() << "dump" << "badging" << QFileInfo(file).fileName());
//     process.waitForFinished();
//     targetPath = QFileInfo(file).dir().path();
//     QByteArray stde = process.readAllStandardError();

//     log->append("exit :" + QString::number(process.exitCode()));
//     log->append("..........................");
//     log->append(QString::fromLocal8Bit(stde).trimmed());

//     QByteArray stdo = process.readAllStandardOutput();
//     QString result = QString::fromLocal8Bit(stdo).trimmed();
//     QStringList lines = result.split('\n');

// //    log->append(result);

//     QString zh;
//     QString en;

//     foreach(QString line, lines) {
// // package: name='cn.androidfun.apkfetch' versionCode='20' versionName='2.5.1' platformBuildVersionName=''
// // application-label-zh:'提取应用APK安装包'
//         if(line.startsWith("package: ")) {
//             QString packagename = line.split(' ').at(1).split("\'").at(1);
//             QString versionCode = line.split(' ').at(2).split("\'").at(1);
//             QString versionName = line.split(' ').at(3).split("\'").at(1);
//             m_apkVersion->setText(versionName);
//             m_apkPackage->setText(packagename);
//             log->append("search packagename: " + packagename);
//             log->append("search versionName: " + versionName);
//         }
//         if(line.startsWith("application-label-zh:")) {
//             QString name = line.split("\'").at(1);
//             zh = name;
            
//         }
//         if(line.startsWith("application-label:")) {
//             QString name = line.split("\'").at(1);
//             en = name;
            
//         }
//     }
//     if(!zh.isEmpty()) {
//         m_apkName->setText(zh);
//     } else {
//         m_apkName->setText(en);
//     }
// }


void ApkManager::onApkFileChoose()
{
    // if(targetPath.isEmpty()) {
    //     targetPath=QDir::homePath();
    // }
    // QString filePath = QFileDialog::getOpenFileName(this, "选择apk文件",targetPath,"apk 文件 (*.apk)");
    // if (!filePath.endsWith(".apk")) {
    //     return;
    // }
    // currentApkPath = filePath;
    // apkPathLine->setText(filePath);
    // onChoosedApkFile();
}

void ApkManager::dragEnterEvent(QDragEnterEvent *event) {
    // event->accept();
    auto * const mime = event->mimeData();
    if (!mime->hasUrls()) {
        return event->ignore();
    }
    return event->accept();    
}
// void dragMoveEvent(QDragMoveEvent *event);
// void dragLeaveEvent(QDragLeaveEvent *event);
void ApkManager::dropEvent(QDropEvent *event) {
     auto * const mime = event->mimeData();
    if (!mime->hasUrls()) {
        return event->ignore();
    }

    QTextStream out(stdout);

    // return event->accept();   

    QStringList fileList;
    for (auto &url : mime->urls())
    {
        if (url.isLocalFile()) {
            QFileInfo info (url.toLocalFile());
            out << QString("--> %1\n").arg(url.toLocalFile());
            if (info.isFile() && info.suffix() == "apk") {
                fileList << url.toLocalFile();
            }
        }
    }
    out.flush();

    if (fileList.size() != 0) {
        page->setApk(fileList[0]);


        out << QString("!! --> %1\n").arg(fileList[0]);
        m_centralLayout->setCurrentIndex(m_centralLayout->count()-1);
    }
}

void ApkManager::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape: m_centralLayout->setCurrentIndex(0);   break;
    default:;
    }
}