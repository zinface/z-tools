#ifndef __APKINFOPAGE__H__
#define __APKINFOPAGE__H__

#include <QWidget>
#include "aapt/Aapt.h"

class QLabel;

class ApkInfoPage : public QWidget, Aapt {
    Q_OBJECT
public:
    explicit ApkInfoPage(QWidget *parent = nullptr);

    void setApk(QString &apk);

signals:
    void onInstall();

private:
    QLabel *m_apkIcon;
    QLabel *m_apkName;
    QLabel *m_apkPackage;
    QLabel *m_apkVersion;
    QLabel *m_apkDescription;
};

#endif  //!__APKINFOPAGE__H__
