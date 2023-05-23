#ifndef APKMANAGER_H
#define APKMANAGER_H

#include <QWidget>
#include <QDropEvent>

class QLabel;
class QLineEdit;
class QTextEdit;
class QStackedLayout;
class SplashWindow;
class ApkInfoPage;
class ApkInstallPage;

class ApkManager : public QWidget
{
    Q_OBJECT
public:
    explicit ApkManager(QWidget *parent = nullptr);
    void chooseApk(QString apkPath);


private:
    void initUi();
    void switchPrevPage();

    QStackedLayout *m_centralLayout;
    SplashWindow *splash;
    ApkInfoPage *infoPage;
    ApkInstallPage *installPage;

private:
    QString apkPath;

    // QWidget interface
protected:
    /*virtual */ void dragEnterEvent(QDragEnterEvent *event);
    // /*virtual */ void dragMoveEvent(QDragMoveEvent *event);
    // /*virtual */ void dragLeaveEvent(QDragLeaveEvent *event);
    /*virtual */ void dropEvent(QDropEvent *event);

    void keyPressEvent(QKeyEvent *event);

};

#endif // APKMANAGER_H
