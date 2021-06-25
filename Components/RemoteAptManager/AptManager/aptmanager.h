#ifndef SEARCHPACKAGE_H
#define SEARCHPACKAGE_H

#include <QTimer>
#include <QWidget>
#include <infomationmanager.h>
#include <packageinfo.h>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QPushButton;
class QLabel;
class QTableWidget;
class QComboBox;

class PackageView;
QT_END_NAMESPACE



class AptManager : public QWidget
{
    Q_OBJECT
public:
    explicit AptManager(QWidget *parent = nullptr);
    bool setWorkHost(QString host);
    bool setWorkPort(int port);

    bool start();

signals:

public slots:
    void onSearchPackage();
    void onPackageChange();
    void onCategoryChange();
    void onArchCategoryChange();

    void onConnected();
    void onUnConnected();
    void onReply(int action, QString msg);
    void onReplyRaw(qint8 action, qint64 length, QByteArray &data);


private:
    /******* 类别控制 *******/
    QLabel *m_packageCategoryLab;
    QComboBox *m_packageArchCategory;
    QComboBox *m_packageCategory;

private:
    /******** 搜索框 和 按钮 ******/
    QLabel *mSearchLab;
    QLineEdit *mSearchEdit;
    QPushButton *mSearchBtn;

private:
    /******* 结果展示Widget ******/
    PackageView *m_packageView;

private:
    QLabel *m_statusBar;

private:
    void createAptManager();
//    AptUtils mAptUtil;
    InfomationManager manager;

    QString workhost;
    int workport;

    QTimer *timer;

// RemoteInfo
private:
    QString remoteLocal;
    QString remoteMirrors;
    QList<PackageInfo*> plist;

private slots:
    void updateStatus();
    void onTimerout();
};

#endif // SEARCHPACKAGE_H
