#ifndef __AAPT__H__
#define __AAPT__H__

#include <QString>

class Aapt {

public:
    void checkCommandsAapt();
    bool checkApk(QString &apk);

    QString aapt;

    QString apkPackage;
    QString apkVersion;
    QString apkIcon = "/tmp/apk-view-icon.png";
    QString apkName = "";

    QString apkLog;
};




#endif  //!__AAPT__H__