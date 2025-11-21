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
    void installPage();
    void installAdb();
    void installUengine();

private:
    Aapt *m_aapt;
    QLabel *m_apkIcon;
    QLabel *m_apkName;
    QLabel *m_apkPackage;
    QLabel *m_apkVersion;
    QLabel *m_apkDescription;

    QString hotKey = "安装:ctrl+i | 运行:ctrl+r | 信息:ctrl+l";

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif  //!__APKINFOPAGE__H__
