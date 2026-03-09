#ifndef SERVERSCAN_H
#define SERVERSCAN_H

#include "scanworkerpool.h"
#include "scanworkerthread.h"

#include <QComboBox>
#include <QHostInfo>
#include <QListWidget>
#include <QMutex>
#include <QProgressBar>
#include <QSpinBox>
#include <QStringListModel>
#include <QThread>
#include <QThreadPool>
#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;

static QMutex mutex;

class ServerScan : public QWidget
{
    Q_OBJECT
public:
    explicit ServerScan(QWidget *parent = nullptr);

private:
    QRegExp rxIP;
    QRegExp rxPort;
//    QRegExpValidator rv;
//    QRegExpValidator;
    void initUI();
    void initLocalIpAddress();
    QList<int> rangeIntList(int x, int y);

private slots:
    void onScanStart();
    void onConnected(QString host, int port, QString protocal);
    void onScanThreadChanged();
    void onScanThreadNumChanged(int);

private slots:
    void remoteStartChanged();

private:
    QProgressBar *scanProgressBar;
    ScanWorkerPool pool;

private:
    QLineEdit *m_remoteStart;
    QLineEdit *m_remoteEnd;
    QComboBox *m_switchFeture;
    QSpinBox threadNumBox;

private:
    QLabel *m_remoteStartLab;
    QLabel *m_remoteEndLab;
    QLabel *m_serverPortLab;
    QLabel *threadNumLab;

    QLineEdit *m_serverPort;
    QPushButton *m_scanButton;

    QListWidget m_resuleView;
    QMenu *m_resuleView_menu;

    QLabel *mHostinfo;

//    ScanWorkerThread *m_ScanWorker;
};

#endif // SERVERSCAN_H
