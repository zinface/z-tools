#ifndef __AAPT__H__
#define __AAPT__H__

#include <QString>

class Aapt {

public:
    explicit Aapt();
    void checkCommandsAapt();
    bool checkApk(QString &apk);

    QString dump_badging(const QString &apkfile);

    bool whichOk(const QString &program);
    bool whichOkSetAapt(const QString &program);
    bool whichOkSetAapt(const QStringList &programs);

    QString aapt = "" ;

    QString apkPackage;
    QString apkVersion;
    QString apkName = "";
    QString tmpAppIcon = "";

    QString apkLog;

    QString tmpProgram = "";
    QString tmpShell = "";

private:
    QString python_read_apk_icon_script(const QString &apk, const QString &iconPath);
    QString python_read_apk_file_script(const QString &apk, const QString &filePath);

    bool isVectorIcon(const QString iconPath);

    QByteArray extractApkFile(const QString &apk, const QString &filePath);
    void extractPngIcon(const QString &apk, const QString &iconPath);
    void extractVectorIcon(const QString &apk, const QString &iconPath);

    void createPlaceholderIcon();
};

#endif  //!__AAPT__H__
