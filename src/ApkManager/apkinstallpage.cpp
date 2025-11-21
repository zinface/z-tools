#include "apkinstallpage.h"
#include "utils/UEngine.h"

#include <QApplication>
#include <QGroupBox>
#include <QMessageBox>
#include <QMovie>
#include <QProgressDialog>
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


    adb_install_button = new QPushButton("adb install");
    uengine_install_button = new QPushButton("安装到 UEngine");

    // adb
    QGroupBox *adb_box = new QGroupBox("Adb");
    QHBoxLayout *adb_layout = new QHBoxLayout(adb_box);
    adb_layout->addWidget(adb_install_button);

    // uegnine
    QGroupBox *uengine_box = new QGroupBox("UEngine");
    QHBoxLayout *uengine_layout = new QHBoxLayout(uengine_box);
    uengine_layout->addWidget(uengine_install_button);

    // Panel
    QVBoxLayout *panelLayout = new QVBoxLayout(this);
    panelLayout->addWidget(adb_box);
    panelLayout->addWidget(uengine_box);
    panelLayout->addSpacing(10);
    panelLayout->addWidget(progressLabel);
    panelLayout->setAlignment(progressLabel, Qt::AlignmentFlag::AlignHCenter);
    panelLayout->addWidget(logLabel);
    panelLayout->setAlignment(logLabel, Qt::AlignmentFlag::AlignHCenter | Qt::AlignmentFlag::AlignLeft);
    panelLayout->addStretch();
    panelLayout->setSpacing(0);

    bool ok = adbOk();
    if (!ok) {
        adb_install_button->setText("未安装 adb");
        adb_install_button->setEnabled(ok);
    }

    ok = uengineOk();
    if (!ok) {
        uengine_install_button->setText("未安装 uengine");
        uengine_install_button->setEnabled(ok);
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

bool ApkInstallPage::adbOk()
{
    return true;
}

bool ApkInstallPage::uengineOk()
{
    return UEngine().checkCommandUEngine();
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
            QMessageBox::warning(this, "Error", "Failed to install APK.\n" + process->readAllStandardError(), QMessageBox::Close);
        }
        process->deleteLater();
    });

    // 安装时处理非标准 .apk 文件结尾(例如 apk.1 结尾)
    if (apkPath.endsWith(".apk")) {
        process->start("adb", QStringList() << "install" << apkPath);
    } else {
        QFileInfo fileInfo(apkPath);
        QString tempApkPath = tempDir.path() + "/" + fileInfo.baseName() + ".apk";
        copyFileWithProgress(apkPath, tempApkPath);
        process->start("adb", QStringList() << "install" << tempApkPath);
    }

    QProgressDialog *installProgressDialog = new QProgressDialog("正在安装...", "取消", 0, 100);
    installProgressDialog->setWindowTitle("安装应用");
    installProgressDialog->setWindowModality(Qt::WindowModal);
    installProgressDialog->setMinimumDuration(0); // 立即显示

    connect(process, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
        installProgressDialog->deleteLater();
    });
    connect(installProgressDialog, &QProgressDialog::canceled, process, &QProcess::terminate);

    installProgressDialog->setValue(10); // 立即显示

    installProgressDialog->show();

    QApplication::processEvents();
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

QString ApkInstallPage::copyFileWithProgress(const QString &sourcePath, const QString &destPath, QWidget *parent)
{
    QFile sourceFile(sourcePath);
    QFile destFile(destPath);

    QFileInfo fileInfo(sourcePath);
    qint64 fileSize = fileInfo.size();

    QProgressDialog progressDialog("正在复制文件...", "取消", 0, 100, parent);
    progressDialog.setWindowTitle("文件复制");
    progressDialog.setWindowModality(Qt::WindowModal);
    progressDialog.setMinimumDuration(0); // 立即显示

    if (!sourceFile.open(QIODevice::ReadOnly)) {
        qWarning() << "无法打开源文件:" << sourcePath;
        return QString();
    }

    if (!destFile.open(QIODevice::WriteOnly)) {
        qWarning() << "无法创建目标文件:" << destPath;
        sourceFile.close();
        return QString();
    }

    const qint64 bufferSize = 8192; // 8KB缓冲区
    char buffer[bufferSize];
    qint64 totalBytesRead = 0;

    while (!sourceFile.atEnd()) {
        if (progressDialog.wasCanceled()) {
            break;
        }

        qint64 bytesRead = sourceFile.read(buffer, bufferSize);
        if (bytesRead == -1) {
            qWarning() << "读取文件错误";
            break;
        }

        qint64 bytesWritten = destFile.write(buffer, bytesRead);
        if (bytesWritten != bytesRead) {
            qWarning() << "写入文件错误";
            break;
        }

        totalBytesRead += bytesRead;

        // 更新进度
        int progress = fileSize > 0 ? static_cast<int>((totalBytesRead * 100) / fileSize) : 0;
        progressDialog.setValue(progress);

        // 处理事件，保持UI响应
        QApplication::processEvents();
    }

    sourceFile.close();
    destFile.close();

    if (progressDialog.wasCanceled()) {
        destFile.remove(); // 删除不完整的文件
        return QString();
    }

    if (totalBytesRead == fileSize) {
        progressDialog.setValue(100);
        return destPath;
    } else {
        destFile.remove(); // 删除不完整的文件
        return QString();
    }
}

