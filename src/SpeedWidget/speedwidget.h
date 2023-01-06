#ifndef SPEEDWIDGET_H
#define SPEEDWIDGET_H

#include <QObject>
#include <QWidget>

#include <infoUtils/infoutils.h>


QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class SpeedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SpeedWidget(QWidget *parent = nullptr);


public slots:
    void onTimeout();

private:
    void initUi();
    infoUtils::Sensitive m_Sensitive;

    QLabel *m_uploadLabel;
    QLabel *m_downloadLabel;
    QLabel *m_cpuLabel;
    QLabel *m_memLabel;

    QLabel *m_uploadValue;
    QLabel *m_downloadVal;
    QLabel *m_cpuVal;
    QLabel *m_memVal;

    QLabel *m_uptime;

    void updateNet();
    void updateCpu();
    void updateMem();
    void updateUptime();

    int m_DecimalsNum;
    long m_down;
    long m_upload;
    long m_cpuAll;
    long m_cpuFree;

    QTimer *m_timer;


};

#endif // SPEEDWIDGET_H
