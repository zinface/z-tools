#ifndef UENGINEUTILS_H
#define UENGINEUTILS_H

#include <QObject>
#include <QProcess>

class UEngine : public QObject
{
    Q_OBJECT
public:
    explicit UEngine(QObject *parent = nullptr);
    ~UEngine();

    enum InstallType {
        Sync,
        Async,
    };

    bool checkCommandUEngine();
    // 检查Android系统是否成功启动
    bool checkCommandReady();
    bool checkApkFile(QString apkFile);

    QStringList logs;

public slots:
    int doSyncInstall(QString apkPath);
    void doAsyncInstall(QString apkPath);
    int doInstall(InstallType type, QString apkPath);
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onFinished2(int exitCode, QProcess::ExitStatus exitStatus);

signals:
    void logChanged(QString log);
    void finished(int exitCode);

private slots:
    void storelog(QString log);

private:
    QProcess process;
    QProcess process2;
    QString cmd;
};



#endif // UENGINEUTILS_H
