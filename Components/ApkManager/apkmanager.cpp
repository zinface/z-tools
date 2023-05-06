#include "apkmanager.h"
#include "apkinfopage.h"
#include "apkinstallpage.h"
#include "splashwindow.h"

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
#include <QCommandLineParser>
#include <QDebug>
#include <QThread>
#include <QTimer>

ApkManager::ApkManager(QWidget *parent) : QWidget(parent)
,m_centralLayout(new QStackedLayout)
,splash(new SplashWindow)
,infoPage(new ApkInfoPage)
,installPage(new ApkInstallPage)
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
    m_centralLayout->addWidget(splash);
    m_centralLayout->addWidget(infoPage);
    m_centralLayout->addWidget(installPage);


    connect(splash, &SplashWindow::done, this, [=](){
        m_centralLayout->setCurrentWidget(infoPage);
    });

    connect(infoPage, &ApkInfoPage::onInstall, this, [=](){
        m_centralLayout->setCurrentWidget(installPage);
    });



    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
    setLayout(m_centralLayout);
    setFixedSize(440,300);
}

void ApkManager::chooseApk(QString apkPath)
{
    this->m_centralLayout->setCurrentWidget(splash);
    splash->start();
    this->apkPath = apkPath;
    qDebug() << QString("chooseApk: %1").arg(apkPath);
    QThread *delayThread = new QThread(this);
    connect(delayThread, &QThread::started, [=](){
        this->infoPage->setApk(this->apkPath);
        this->installPage->setApk(this->apkPath);
        delayThread->exit(0);
    });
    connect(delayThread, &QThread::finished, delayThread, &QThread::deleteLater);

    QTimer *timer = new QTimer;
    timer->setInterval(200);
    connect(timer, SIGNAL(timeout()), delayThread, SLOT(start()));
    timer->start();
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

void ApkManager::switchPrevPage()
{
    int nextIndex = m_centralLayout->currentIndex() - 1;
    if (nextIndex == 1){
        m_centralLayout->setCurrentIndex(0);
    } else {
        m_centralLayout->setCurrentIndex(nextIndex);
    }
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
        chooseApk(fileList[0]);

        out << QString("!! --> %1\n").arg(fileList[0]);
//        m_centralLayout->setCurrentIndex(m_centralLayout->count()-1);
    }
}

void ApkManager::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Escape:
            switchPrevPage();
        break;
    default:;
    }
}