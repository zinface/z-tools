#ifndef APKHELPER_H
#define APKHELPER_H

#include "Aapt.h"

#include <QObject>

class ApkHelper
{
public:
    ApkHelper();

    struct ApkInfo {
        QString packageName;
        QString mainActivity;
        QString versionName;
        QString versionCode;
        QString appName;
        bool isValid() const { return !packageName.isEmpty() && !mainActivity.isEmpty(); }
    };

    static ApkInfo parseApkInfo(Aapt *aapt,const QString &apkfile);
};

#endif // APKHELPER_H
