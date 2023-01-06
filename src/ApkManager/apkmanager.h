#ifndef APKMANAGER_H
#define APKMANAGER_H

#include <QWidget>
#include <QDropEvent>



class QLabel;
class QLineEdit;
class QTextEdit;
class QStackedLayout;
class ApkInfoPage;

class ApkManager : public QWidget
{
    Q_OBJECT
public:
    explicit ApkManager(QWidget *parent = nullptr);

private:
    void initUi();


    QStackedLayout *m_centralLayout;
    ApkInfoPage *page;

private:
    // QString currentApkPath;
    // QLineEdit *apkPathLine;
    QTextEdit *m_logText;

    // QString targetPath;
    // void onChoosedApkFile();

    // void checkCommandsAapt();


private slots:
    void onApkFileChoose();
    // void onDropApkFile();

    // QWidget interface
protected:
    /*virtual */ void dragEnterEvent(QDragEnterEvent *event);
    // /*virtual */ void dragMoveEvent(QDragMoveEvent *event);
    // /*virtual */ void dragLeaveEvent(QDragLeaveEvent *event);
    /*virtual */ void dropEvent(QDropEvent *event);

    void keyPressEvent(QKeyEvent *event);

};

#endif // APKMANAGER_H
