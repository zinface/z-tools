#include "apkhelper.h"

#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

ApkHelper::ApkHelper()
{

}

ApkHelper::ApkInfo ApkHelper::parseApkInfo(Aapt *aapt, const QString &apkfile)
{
    ApkInfo info;

    QString output = aapt->dump_badging(apkfile);
    if (output.isEmpty()) {
        return info;
    }

    // 使用正则表达式解析输出
    QRegularExpression regex;
    QRegularExpressionMatch match;

    // 解析包名
    regex.setPattern(R"(package: name='([^']+)')");
    match = regex.match(output);
    if (match.hasMatch()) {
        info.packageName = match.captured(1);
    }

    // 解析版本名称
    regex.setPattern(R"(versionName='([^']*)')");
    match = regex.match(output);
    if (match.hasMatch()) {
        info.versionName = match.captured(1);
    }

    // 解析版本代码
    regex.setPattern(R"(versionCode='([^']*)')");
    match = regex.match(output);
    if (match.hasMatch()) {
        info.versionCode = match.captured(1);
    }

    // 解析应用名称
    regex.setPattern(R"(application-label:'([^']*)')");
    match = regex.match(output);
    if (match.hasMatch()) {
        info.appName = match.captured(1);
    }

    // 解析主Activity - 首先查找launchable-activity
    regex.setPattern(R"(launchable-activity: name='([^']+)')");
    match = regex.match(output);
    if (match.hasMatch()) {
        info.mainActivity = match.captured(1);
    } else {
        // 如果没有明确的主Activity，查找包含LAUNCHER的activity
        QStringList lines = output.split('\n');
        for (const QString &line : lines) {
            if (line.contains("android.intent.action.MAIN") &&
                line.contains("android.intent.category.LAUNCHER")) {
                regex.setPattern(R"(name='([^']+)')");
                match = regex.match(line);
                if (match.hasMatch()) {
                    info.mainActivity = match.captured(1);
                    break;
                }
            }
        }
    }

    qDebug() << "Parsed APK Info:";
    qDebug() << "Package:" << info.packageName;
    qDebug() << "Main Activity:" << info.mainActivity;
    qDebug() << "Version Name:" << info.versionName;
    qDebug() << "Version Code:" << info.versionCode;
    qDebug() << "App Name:" << info.appName;

    return info;
}
