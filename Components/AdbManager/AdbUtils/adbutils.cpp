#include "adbutils.h"

#include <QDebug>
#include <QProcess>
#include <QStringList>

void AdbUtils::StopServer()
{
    RunCommand("kill-server");
}

//List of devices attached
//192.168.43.1:9999       device
QStringList AdbUtils::GetDevices()
{
    QStringList devices;
    QString content = RunCommand("devices");
    if (!content.isEmpty()) {
        content.replace(QRegExp("List of devices attached\n"), "");
//        content.replace("List of devices attached", "");
         QStringList splits = content.split("\n");
         foreach(const QString& item, splits) {
             QStringList devs = item.split("\t");
             if (devs.length() == 2 && devs[1] == "device") {
                 devices.append(devs[0]);
             }
         }
    }
    return devices;
}

QString AdbUtils::GetBrand(const QString &deviceId)
{
    QString content = nullptr;
    if (deviceId == nullptr) {
        content = RunCommand("shell getprop ro.product.brand");
    } else {
        content = RunCommand("-s " + deviceId + " shell getprop ro.product.brand");
    }
    return content;
}

QString AdbUtils::GetModel(const QString &deviceId)
{
    QString content = nullptr;
    if (deviceId == nullptr) {
        content = RunCommand("shell getprop ro.product.model");
    } else {
        content = RunCommand("-s " + deviceId + " shell getprop ro.product.model");
    }
    return content;
}

QString AdbUtils::GetManufacturer(const QString &deviceId)
{
    QString content = nullptr;
    if (deviceId == nullptr) {
        content = RunCommand("shell getprop ro.product.manufacturer");
    } else {
        content = RunCommand("-s " + deviceId + " shell getprop ro.product.manufacturer");
    }
    return content;
}

QString AdbUtils::GetAndroidApiLevel(const QString &deviceId)
{
//    ro.build.version.sdk
    QString content = nullptr;
    if (deviceId == nullptr) {
        content = RunCommand("shell getprop ro.build.version.sdk");
    } else {
        content = RunCommand("-s " + deviceId + " shell getprop ro.build.version.sdk");
    }
    return content;
}


QString AdbUtils::GetAndroidVersion(const QString &deviceId)
{
//    ro.build.version.release
    QString content = nullptr;
    if (deviceId == nullptr) {
        content = RunCommand("shell getprop ro.build.version.release");
    } else {
        content = RunCommand("-s " + deviceId + " shell getprop ro.build.version.release");
    }
    return content;
}

QString AdbUtils::GetAndroidWmSize(const QString &deviceId)
{
//    adb shell wm size
//    Physical size: 1080x2340
    QString content = nullptr;
    if (deviceId == nullptr) {
        content = RunCommand("shell wm size");
    } else {
        content = RunCommand("-s " + deviceId + " shell wm size");
    }
    if (!content.isEmpty()) {
        if(content.startsWith("Physical size:")) {
            QStringList result = content.split(" ");
            if(result.size()>0)
                return result.at(result.size()-1);
        }
    }
    return "null";

}

//查看应用列表
//adb shell pm list packages [-f] [-d] [-e] [-s] [-3] [-i] [-u] [--user USER_ID] [FILTER]
//-f	显示应用关联的 apk 文件
//-d	只显示 disabled 的应用
//-e	只显示 enabled 的应用
//-s	只显示系统应用
//-3	只显示第三方应用
//-i	显示应用的 installer
//-u	包含已卸载应用
//<FILTER>	包名包含 <FILTER> 字符串
QStringList AdbUtils::GetAndroidPackages(const QString& deviceId, const QString& tag, const QString& keyword)
{
    QString m_tag = tag != nullptr ? tag : "";
    QString m_keyword = keyword != nullptr? keyword : "";
    QString content = nullptr;
    if (deviceId == nullptr) {
        content = RunCommand("shell pm list packages " + m_tag + " " + m_keyword);
    } else {
        content = RunCommand("-s " + deviceId + " shell pm list packages " + m_tag + " " + m_keyword);
    }
    QStringList packages;
    if(!content.isEmpty()) {
        foreach(const QString item, content.split("\n")){
            if(!item.isEmpty())
                packages.append(item);
        }
    }
    qDebug() << packages;
    return packages;
}

QString AdbUtils::RunCommand(const QString &commandLine)
{
    QProcess process;
    process.start("adb " + commandLine);
    process.waitForFinished();
    QByteArray stdo = process.readAllStandardOutput();
    QString result = QString::fromLocal8Bit(stdo).trimmed();
    return result;
}

/*
int main(int argc, char *argv[])
{

//    qDebug() << AdbUtils().RunCommand("kill-server");

//    QString str("List of devices attached\n");
//    qDebug() << str;
//    str.replace(QRegExp("List of devices attached(\\s)\n"), "");
//    qDebug() << str;

    QString deviceId("b700eacb");
    qDebug() << "Brand: " << AdbUtils::GetBrand(nullptr);
    qDebug() << "Model: " << AdbUtils::GetModel(nullptr);

    qDebug() << "Api: " << AdbUtils::GetAndroidApiLevel(nullptr);
    qDebug() << "Android: " << AdbUtils::GetAndroidVersion(nullptr);

    QStringList devices;
    foreach(const QString& item, AdbUtils().GetDevices()) {
        devices.append(item +"("+AdbUtils().GetModel(item)+")");
    }
    qDebug() << devices;

    return 0;
}

*/


