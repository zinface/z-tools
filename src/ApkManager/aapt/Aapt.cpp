#include "Aapt.h"
#include "qapplication.h"
#include <QProcess>
#include <QInputDialog>
#include <qfileinfo.h>
#include <QTextStream>
#include <QFileDialog>
#include <QMessageBox>
#include <QSvgRenderer>
#include <QPixmap>
#include <QPainter>

#define AAPT_VERIONS_KEY "Android Asset Packaging Tool"

Aapt::Aapt()
    : tmpProgram("python3")
    , tmpShell("/tmp/apk-view.sh")
    , tmpAppIcon("/tmp/apk-view-icon.png")
{

    whichOkSetAapt({
#if defined(Q_OS_LINUX)
        "aapt",
#endif
#if defined(Q_OS_LINUX) && defined(Q_PROCESSOR_X86_64)
        "/opt/zTools/Components/BaseApkView/assets/tools/build-tools/35.0.0/aapt",
#endif
    });

#ifdef DEBUG_LINUX_AAPT
    this->aapt = DEBUG_LINUX_AAPT;
#endif

}

bool checkVersion(QString program) {
    QProcess process;
    process.start(program, QStringList() << "--version");
    process.waitForFinished();
    return process.readAllStandardOutput().contains(AAPT_VERIONS_KEY);
}

void Aapt::checkCommandsAapt() {
    if (aapt.isEmpty() || !whichOk(aapt)) {
        QMessageBox::StandardButton button =  QMessageBox::question(nullptr, "未配置 aapt 程序", "是否指定 aapt 程序位置?");
        if (button == QMessageBox::No) {
            return;
        }
        QString filePath = QFileDialog::getOpenFileName(nullptr, "aapt 程序");
        if (!filePath.isEmpty()) {
            aapt = filePath;
            checkCommandsAapt();
        }
        // 留白
    }
}

bool Aapt::checkApk(QString &apk) {
    
    QStringList out;

    QString command = aapt.trimmed();
    QStringList arguments{"dump", "badging", apk};
    out << command << " " << arguments.join(" ") << QString("\n");
    // out.flush();

    QProcess process;
    process.start(command, arguments);
    process.waitForFinished();
    if (process.exitCode() != 0) return false;

    // QByteArray out = process.readAllStandardOutput().trimmed();
    QStringList lineArray = QString::fromLocal8Bit(process.readAllStandardOutput()).trimmed().split("\n");

    QString zh, en;
    bool iconGenerated = false;
    foreach(QString line, lineArray) {
        if (line.startsWith("package: ")) {
            QString packageName = line.split(' ').at(1).split("\'").at(1);
            QString versionCode = line.split(' ').at(2).split("\'").at(1);
            QString versionName = line.split(' ').at(3).split("\'").at(1);

            out << QString("search packageName: %1\n").arg(packageName);
            out << QString("search versionCode: %1\n").arg(versionCode);
            out << QString("search versionName: %1\n").arg(versionName);

            apkPackage = packageName;
            // apkVersion = versionCode;
            apkVersion = versionName;
            
            // sscanf(const char *__restrict __s, const char *__restrict __format, ...)
        }
        if (line.startsWith("application-label-zh:")) {
            QStringList words = line.split("\'");
            if (words.size()  ==  3) {
                zh = words[1];
            }
            // log->append("search apkName(zh): " + name);
            out << QString("search apkName(zh): %1\n").arg(zh);
        }
        if (line.startsWith("application-label:")) {
            QStringList words = line.split("\'");
            if (words.size()  ==  3) {
                en = words[1];
            }
            // log->append("search apkName(en): " + name);
            out << QString("search apkName(en): %1\n").arg(en);
        }
        if (line.startsWith("application-icon") && !iconGenerated) {
            QString iconPath = line.split("\'").at(1);
            if (isVectorIcon(iconPath)) {
                // extractVectorIcon(apk, iconPath);
                createPlaceholderIcon();
            } else {
                extractPngIcon(apk, iconPath);
            }
            iconGenerated = true;
        }
    }

    if(!zh.isEmpty()) {
        apkName = (zh);
    } else {
        apkName = (en);
    }

    apkLog = (out << QString("Name: %1\n").arg(apkName)) .join("");


    return true;
}

QString Aapt::dump_badging(const QString &apkfile)
{
    QString command = aapt.trimmed();
    QStringList arguments{"dump", "badging", apkfile};

    QProcess process;
    process.start(command, arguments);
    process.waitForFinished();

    return process.readAllStandardOutput();
}

bool Aapt::whichOk(const QString &program)
{
    QProcess process;
    process.start(QString("which"), {program});
    process.waitForFinished();

    return process.exitCode() == 0;
}

bool Aapt::whichOkSetAapt(const QString &program)
{
    bool ok = false;
    if ((ok = whichOk(program))) {
        aapt = program;
    }
    return ok;
}

bool Aapt::whichOkSetAapt(const QStringList &programs)
{
    bool ok = false;
    foreach (auto program, programs) {
        if ((ok = whichOkSetAapt(program))) {
            break;
        }
    }
    return ok;
}

QString Aapt::python_read_apk_icon_script(const QString &apk, const QString &iconPath)
{
    QString script = R"(
#!/usr/bin/python3
import zipfile
zip = zipfile.ZipFile('%1')
with open('%3', 'wb') as file:
    file.write(zip.read('%2'))
    file.flush()
)";
    return script.arg(apk).arg(iconPath).arg(tmpAppIcon);
}

QString Aapt::python_read_apk_file_script(const QString &apk, const QString &filePath)
{
    QString script = R"(
#!/usr/bin/python3
import zipfile
import sys
zip = zipfile.ZipFile('%1')
sys.stdout.buffer.write(zip.read('%2'))
)";
    return script.arg(apk).arg(filePath);
}

bool Aapt::isVectorIcon(const QString iconPath)
{
    return iconPath.endsWith(".xml");
}

QByteArray Aapt::extractApkFile(const QString &apk, const QString &filePath)
{
    QByteArray script_content = python_read_apk_file_script(apk, filePath).toLocal8Bit();
    QFile sh(tmpShell);
    if (sh.open(QIODevice::WriteOnly)) {
        sh.write(script_content);
        sh.flush();
        sh.close();
    }

    QProcess extractProcess;
    extractProcess.start(tmpProgram, {tmpShell});
    extractProcess.waitForFinished();

    return extractProcess.readAllStandardOutput();
}

void Aapt::extractPngIcon(const QString &apk, const QString &iconPath)
{
    QByteArray script_content = python_read_apk_icon_script(apk, iconPath).toLocal8Bit();
    QFile script(tmpShell);
    if (script.open(QIODevice::WriteOnly)) {
        script.write(script_content);
        script.flush();
        script.close();
    }

    QProcess generatorIcon;
    generatorIcon.start(tmpProgram, {tmpShell});
    generatorIcon.waitForFinished();
}

void Aapt::extractVectorIcon(const QString &apk, const QString &iconPath)
{
    QByteArray binaryData = extractApkFile(apk, iconPath);

    QPixmap pixmap(QSize(64,64));
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    QSvgRenderer renderer;
    if (renderer.load(binaryData)) {
        renderer.render(&painter);
    }

    pixmap.save(tmpAppIcon);
}

void Aapt::createPlaceholderIcon()
{
    QSize size(64,64);
    QPixmap pixmap(size);
    pixmap.fill(QColor(200, 200, 200));

    QPainter painter(&pixmap);
    painter.setPen(Qt::darkGray);
    painter.drawRect(0, 0, size.width()-1, size.height()-1);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "No Icon");

    pixmap.save(tmpAppIcon);
}

// 

// package: name='com.tencent.mobileqq' versionCode='2266' versionName='8.8.38' platformBuildVersionName=''
// install-location:'auto'
// sdkVersion:'21'
// targetSdkVersion:'26'
// application-label:'QQ'

// application-label:'QQ'
// application-icon-120:'r/b/icon.png'
// application-icon-160:'r/b/icon.png'
// application-icon-240:'r/b/icon.png'
// application-icon-320:'r/b/icon.png'
// application-icon-480:'r/b/icon.png'
// application-icon-640:'r/b/icon.png'
// application-icon-65534:'r/b/icon.png'
// application-icon-65535:'r/b/icon.png'
// application: label='QQ' icon='r/b/icon.png'

// package: name='com.microsoft.office.word' versionCode='2005742579' versionName='16.0.19127.20128' compileSdkVersion='35' compileSdkVersionCodename='15'                                                                                                                                                              install-location:'auto'
// sdkVersion:'29'
// targetSdkVersion:'34'

// application-icon-120:'res/jT.xml'
// application-icon-160:'res/jT.xml'
// application-icon-240:'res/jT.xml'
// application-icon-320:'res/jT.xml'
// application-icon-480:'res/jT.xml'
// application-icon-640:'res/jT.xml'
// application-icon-65534:'res/jT.xml'
// application: label='Microsoft Excel' icon='res/jT.xml'

