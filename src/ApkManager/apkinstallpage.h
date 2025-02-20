#ifndef APKINSTALLPAGE_H
#define APKINSTALLPAGE_H

#include <QDialog>
#include <QDropEvent>
#include <QLabel>
#include <QTextEdit>
#include <QWidget>

#include "utils/UEngine.h"
// class UEngine;

class ApkInstallPage : public QWidget
{
    Q_OBJECT
public:
    explicit ApkInstallPage(QWidget *parent = nullptr);
    void setApk(QString apkPath);

signals:

public slots:
    void slot_install_uengine();
    void slot_install_adb();
    void onInstallLog(QString log);
    void onInstalled(int exitCode);

private:
    QPushButton *uengine_install_button;

private:
    QString apkPath;
    UEngine *uengine;
    QLabel *progressLabel;
    QLabel *logLabel;
    QMovie *movie;

    // QWidget interface
//protected:
//    void keyPressEvent(QKeyEvent *event) override;
};




#endif // APKINSTALLPAGE_H
