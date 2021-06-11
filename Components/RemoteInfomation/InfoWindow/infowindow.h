#ifndef INFOWINDOW_H
#define INFOWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>
#include <infomationmanager.h>
#include <linuxinfomation.h>

class InfoWindow : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWindow(QWidget *parent = nullptr);
    bool setWorkHost(QString host);
    bool setWorkPort(int port);

    bool start();

private slots:
    void onConnected();
    void onUnConnected();

    void onReplay(int action, QString msg);

private:
    InfomationManager manager;
    LinuxInfomation info;

    QString _host;
    int _port;


    QLabel *mCpuUsed;
    QLabel *mMemUsed;
    QLabel *mNetUpload;
    QLabel *mNetDownload;
    QLabel *mUptime;

    QTimer *timer;

    QString getCpuUsed();
    QString getMemUsed();
    QString getNetUploadDownload();
    QString getUptime();


private slots:
    void onTimerout();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
};

#endif // INFOWINDOW_H
