#include "apkmanager.h"
#include "apkhelper.h"
#include "apkinfopage.h"
#include "apkinstallpage.h"
#include "splashpage.h"

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
#include <QtConcurrent>
#include <QScrollArea>
#include <QMessageBox>

ApkManager::ApkManager(QWidget *parent) : QWidget(parent)
    , m_centralLayout(new QStackedLayout)
    , splashPage(new SplashPage)
    , infoPage(new ApkInfoPage)
    , installPage(new ApkInstallPage)
    , m_aapt(new Aapt)
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
    m_centralLayout->addWidget(splashPage);
    m_centralLayout->addWidget(infoPage);
    m_centralLayout->addWidget(installPage);


    connect(splashPage, &SplashPage::done, this, [=](){
        m_centralLayout->setCurrentWidget(infoPage);
    });

    connect(infoPage, &ApkInfoPage::installPage, this, [=](){
        m_centralLayout->setCurrentWidget(installPage);
    });
    connect(infoPage, &ApkInfoPage::installAdb, this, [=](){
        installPage->slot_install_adb();
    });
    connect(infoPage, &ApkInfoPage::installUengine, this, [=](){
        installPage->slot_install_uengine();
    });

    infoPage->setAapt(m_aapt);

    setAcceptDrops(true);
    setFocusPolicy(Qt::StrongFocus);
    setLayout(m_centralLayout);
    setFixedSize(440,300);
}

/**
 * @brief  chooseApk 选中一个文件路径
 * @note   
 * @param  apkPath: 
 * @retval None
 */
void ApkManager::chooseApk(QString apkPath)
{
    m_aapt->checkCommandsAapt();
    // 当前是一个加载界面
    this->m_centralLayout->setCurrentWidget(splashPage);
    // 并且启动加载动画
    splashPage->start();

    this->apkPath = apkPath;
    qDebug() << QString("chooseApk: %1").arg(apkPath);

    // 使用一个线程来处理任务，这个过程将会持续动画
    // 但也为了每次都在分析得非常快，所以加了一个 QTimer 来启动线程
    // fix: 使用 concurrent
//    QThread *delayThread = new QThread(this);
//    connect(delayThread, &QThread::started, [=](){
//        this->infoPage->setApk(this->apkPath);
//        this->installPage->setApk(this->apkPath);
//        delayThread->exit(0);
//    });
//    connect(delayThread, &QThread::finished, delayThread, &QThread::deleteLater);

//    QTimer *timer = new QTimer;
//    timer->setInterval(200);
//    connect(timer, SIGNAL(timeout()), delayThread, SLOT(start()));
//    timer->start();

    QtConcurrent::run([this](){
        this->infoPage->setApk(this->apkPath);
        this->installPage->setApk(this->apkPath);
    });
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

/**
 * @brief  switchPrevPage 界面回退
 * @note   
 * @retval None
 */
void ApkManager::switchPrevPage()
{
    // 界面顺序从 QStackedLayout 中加载，如果
    // int nextIndex = m_centralLayout->currentIndex() - 1;
    // 如果是 1 == Splash 界面时，将直接显示主界面，否则显示前一个界面
    // if (nextIndex == 1){
    //     m_centralLayout->setCurrentIndex(0);
    // } else {
    //     m_centralLayout->setCurrentIndex(nextIndex);
    // }

// new code logic
    int nextIndex = m_centralLayout->currentIndex();
    nextIndex = nextIndex == 2 ? 0 :nextIndex -1;
    m_centralLayout->setCurrentIndex(nextIndex);
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

    // return event->accept();   

    QStringList fileList;
    for (auto &url : mime->urls())
    {
        if (url.isLocalFile()) {
            QFileInfo info (url.toLocalFile());
            qDebug().noquote() << "-->" << url.toLocalFile();
            if (info.isFile() && info.suffix() == "apk") {
                fileList << url.toLocalFile();
            }
        }
    }

    if (fileList.size() != 0) {
        chooseApk(fileList[0]);

        qDebug().noquote() << "!! -->" << fileList[0];
//        m_centralLayout->setCurrentIndex(m_centralLayout->count()-1);
    }
}

void ApkManager::keyPressEvent(QKeyEvent *event) {
    // 在按下了 Esc 键时，回退到上一个界面
    if (event->key() == Qt::Key_Escape) {
        switchPrevPage();
    }

    // [Ctrl +l] 显示信息
    if (m_centralLayout->currentWidget() == infoPage
        && ((event->modifiers() | Qt::Key_Control) && (event->key() == Qt::Key_L))) {

        QWidget *widget = new QWidget();
        QLabel *label = new QLabel(m_aapt->dump_badging(apkPath));
        QVBoxLayout *layout = new QVBoxLayout(widget);
        label->setTextInteractionFlags(Qt::TextSelectableByMouse);
        layout->addWidget(label);

        auto area = new QScrollArea;
        area->setWidget(widget);
        area->show();

        ApkHelper::ApkInfo info = ApkHelper::parseApkInfo(m_aapt, apkPath);
        area->setWindowTitle(info.packageName + "/" +info.mainActivity);
    }

    // [Ctrl + i] 安装程序
    if (m_centralLayout->currentWidget() == infoPage
        && ((event->modifiers() | Qt::Key_Control) && (event->key() == Qt::Key_I))) {
        // # 1
        // ApkHelper::ApkInfo info = ApkHelper::parseApkInfo(m_aapt, apkPath);
        // QProcess::startDetached("adb", {"install", apkPath});

        // # 2
        installPage->slot_install_adb();
    }

    // [Ctrl + r] 启动程序
    if (m_centralLayout->currentWidget() == infoPage
        && ((event->modifiers() | Qt::Key_Control) && (event->key() == Qt::Key_R))) {
        ApkHelper::ApkInfo info = ApkHelper::parseApkInfo(m_aapt, apkPath);

        if (info.mainActivity.isEmpty()) {
            QMessageBox::warning(this, "无法启动", "未检索到应用的 MainActivity");
            return;
        }

        // # 1
        // QProcess::startDetached("adb", {"shell", "am", "start", "-n", info.packageName+"/"+info.mainActivity});

        // # 2
        QProcess process;
        QString command = QString("am start -n %1/%2")
                         .arg(info.packageName)
                         .arg(info.mainActivity);

        process.setProcessChannelMode(QProcess::MergedChannels); // 合并标准输出和错误输出
        process.start("adb", {"shell", command});

        if (process.waitForFinished(5000)) {
            QString output = QString::fromLocal8Bit(process.readAll());
            if (output.contains("Error") ||  output.contains("exception", Qt::CaseInsensitive)
                || process.exitCode() != 0) {
                QMessageBox::warning(this, "启动失败", output);
            } else {
                QMessageBox::information(this, "成功", "应用启动成功");
            }
        } else {
            QMessageBox::warning(this, "错误", "启动命令执行超时");
        }
    }
}
