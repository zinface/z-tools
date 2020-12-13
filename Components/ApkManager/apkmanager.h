#ifndef APKMANAGER_H
#define APKMANAGER_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QTextEdit;

class ApkManager : public QWidget
{
    Q_OBJECT
public:
    explicit ApkManager(QWidget *parent = nullptr);

private:
    void initUi();
    QLabel *m_apkNameLabel;
    QLabel *m_apkVersionLabel;
    QLabel *m_apkPackageLabel;

    QLabel *m_apkName;
    QLabel *m_apkVersion;
    QLabel *m_apkPackage;

private:
    QString currentApkPath;
    QLineEdit *apkPathLine;
    QTextEdit *log;

    QString targetPath;
    void onChoosedApkFile();

private slots:
    void onApkFileChoose();
};

#endif // APKMANAGER_H
