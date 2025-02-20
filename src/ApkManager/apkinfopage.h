#ifndef __APKINFOPAGE__H__
#define __APKINFOPAGE__H__

#include <QWidget>
#include "aapt/Aapt.h"

class QLabel;

class ApkInfoPage : public QWidget {
    Q_OBJECT
public:
    explicit ApkInfoPage(QWidget *parent = nullptr);

    void setApk(QString &apk);

    void setAapt(Aapt *newAapt);

signals:
    void onInstall();

private:
    Aapt *m_aapt;
    QLabel *m_apkIcon;
    QLabel *m_apkName;
    QLabel *m_apkPackage;
    QLabel *m_apkVersion;
    QLabel *m_apkDescription;
};

#endif  //!__APKINFOPAGE__H__
