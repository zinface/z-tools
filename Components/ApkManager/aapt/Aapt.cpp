#include "Aapt.h"
#include <QProcess>
#include <QInputDialog>
#include <qfileinfo.h>
#include <QTextStream>

void Aapt::checkCommandsAapt() {
    if (aapt=="") {
        aapt = "aapt";
    }
    QProcess process;
    process.start(QString("which"), QStringList() << aapt);
    process.waitForFinished();
    if (process.exitCode() != 0) {
        QInputDialog dialog;
        dialog.setLabelText("指定aapt程序位置");
        if (dialog.exec() == QDialog::Accepted) {
            aapt = dialog.textValue();
            checkCommandsAapt();
        }
    }
}

bool Aapt::checkApk(QString &apk) {
    
    QStringList out;

    QString command = aapt.trimmed();
    QStringList args = QStringList() << "dump"<< "badging" << apk;
    out << command << " " << args.join(" ") << QString("\n");
    // out.flush();

    QProcess process;
    process.start(command, args);
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
            QString python = "python3";
            QString tmpShell = "/tmp/apk-view.sh";
            QFile sh(tmpShell);
            if (sh.open(QIODevice::WriteOnly)) {
                sh.write(QString(
                    "#!/usr/bin/python3\n"
                    "import zipfile\n"
                    "zip = zipfile.ZipFile('%1')\n"
                    "file = open('%2', 'wb')\n"
                    "file.write(zip.read('%3'))\n"
                    "file.flush()\n"
                    "file.close()\n")
                    .arg(apk).arg(apkIcon).arg(iconPath).toLocal8Bit());
                sh.flush();
                sh.close();
            }
            QProcess generatorIcon;
            generatorIcon.start(python, QStringList() << tmpShell);
            generatorIcon.waitForFinished();

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


