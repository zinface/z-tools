#ifndef SERVERSCAN_H
#define SERVERSCAN_H

#include "scanworkerthread.h"

#include <QMutex>
#include <QStringListModel>
#include <QThread>
#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QListView;

static QMutex mutex;

class ServerScan : public QWidget
{
    Q_OBJECT
public:
    explicit ServerScan(QWidget *parent = nullptr);

private slots:
    void onScanStart();
    void onConnected(QString result);

private:
    void initUi();
    QLabel *mServerPrefixLab;
    QLineEdit *mServerPrefix;
    QLabel *mServerPortLab;
    QLineEdit *mServerPort;
    QPushButton *mScanButton;
    QStringListModel *m_model;
    QListView *mResultView;

//    ScanWorkerThread *m_ScanWorker;
};

#endif // SERVERSCAN_H
