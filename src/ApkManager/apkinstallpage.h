#ifndef APKINSTALLPAGE_H
#define APKINSTALLPAGE_H

#include <QDialog>
#include <QDropEvent>
#include <QLabel>
#include <QTemporaryDir>
#include <QTextEdit>
#include <QWidget>

#include "utils/UEngine.h"
// class UEngine;

class ApkInstallPage : public QWidget
{
    Q_OBJECT
public:
    friend class ApkInfoPage;
    explicit ApkInstallPage(QWidget *parent = nullptr);
    void setApk(QString apkPath);

    bool adbOk();
    bool uengineOk();

signals:

public slots:
    void slot_install_adb();
    void slot_install_uengine();
    void onInstallLog(QString log);
    void onInstalled(int exitCode);

private:
    QPushButton *adb_install_button;
    QPushButton *uengine_install_button;

private:
    QString apkPath;
    UEngine *uengine;
    QLabel *progressLabel;
    QLabel *logLabel;
    QMovie *movie;

    QTemporaryDir tempDir;


private:
    QString copyFileWithProgress(const QString& sourcePath, const QString& destPath, QWidget* parent = nullptr);
};




#endif // APKINSTALLPAGE_H
