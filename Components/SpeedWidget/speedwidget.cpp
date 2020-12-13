#include "speedwidget.h"

#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>

#include <infoUtils/infoutils.h>

SpeedWidget::SpeedWidget(QWidget *parent) : QWidget(parent)
  ,m_Sensitive(infoUtils::Default)
  ,m_uploadLabel(new QLabel("↑:"))
  ,m_downloadLabel(new QLabel("↓:"))
  ,m_cpuLabel(new QLabel("MEM:"))
  ,m_memLabel(new QLabel("CPU:"))
  ,m_uploadValue(new QLabel("0.00 B/s"))
  ,m_downloadVal(new QLabel("0.00 B/s"))
  ,m_cpuVal(new QLabel("0.0 %"))
  ,m_memVal(new QLabel("0.0 %"))
  ,m_uptime(new QLabel(""))
  ,m_DecimalsNum(0)
  ,m_down(0)
  ,m_upload(0)
  ,m_cpuAll(0)
  ,m_cpuFree(0)
  ,m_timer(new QTimer)
{

    initUi();

    m_timer->setInterval(1000);
    m_timer->start();

    connect(m_timer, &QTimer::timeout,this,&SpeedWidget::onTimeout);
}

void SpeedWidget::onTimeout()
{
    updateNet();
    updateCpu();
    updateMem();
    updateUptime();
}

void SpeedWidget::initUi()
{

    QGridLayout *uploadspeedGL = new QGridLayout;
    uploadspeedGL->addWidget(m_uploadLabel,0,0,1,1);
    uploadspeedGL->addWidget(m_uploadValue,0,1,1,1);
    uploadspeedGL->setAlignment(m_uploadValue,Qt::AlignLeft);

    QGridLayout *downloadspeedGL = new QGridLayout;
    downloadspeedGL->addWidget(m_downloadLabel,0,0,1,1);
    downloadspeedGL->addWidget(m_downloadVal,0,1,1,1);
    downloadspeedGL->setAlignment(m_downloadVal,Qt::AlignLeft);

    QGridLayout *cpuspeedGL = new QGridLayout;
    cpuspeedGL->addWidget(m_cpuLabel,0,0,1,1);
    cpuspeedGL->addWidget(m_cpuVal,0,1,1,1);
    cpuspeedGL->setAlignment(m_cpuVal,Qt::AlignLeft);

    QGridLayout *memspeedGL = new QGridLayout;
    memspeedGL->addWidget(m_memLabel,0,0,1,1);
    memspeedGL->addWidget(m_memVal,0,1,1,1);
    memspeedGL->setAlignment(m_memVal,Qt::AlignLeft);


    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addLayout(uploadspeedGL,0,0,1,1);
    mainLayout->addLayout(downloadspeedGL,1,0,1,1);
    mainLayout->addLayout(cpuspeedGL,0,1,1,1);
    mainLayout->addLayout(memspeedGL,1,1,1,1);
    mainLayout->setSpacing(10);

    QVBoxLayout *topLayout = new QVBoxLayout;
    topLayout->addLayout(mainLayout);
    topLayout->addWidget(m_uptime);
    topLayout->setAlignment(m_uptime, Qt::AlignVCenter | Qt::AlignCenter);

    setLayout(topLayout);
}

void SpeedWidget::updateNet()
{
    long down = 0;
    long upload = 0;
    double downRate = 0;
    double uploadRate = 0;
    infoUtils::RateUnit unit = infoUtils::RateByte;

    infoUtils::netRate(down, upload);
    downRate = infoUtils::autoRateUnits((down - m_down) / (m_timer->interval() / 1000), unit);
    QString downUnit = infoUtils::setRateUnitSensitive(unit, m_Sensitive);
    unit = infoUtils::RateByte;
    uploadRate = infoUtils::autoRateUnits((upload - m_upload) / (m_timer->interval() / 1000), unit);
    QString uploadUnit = infoUtils::setRateUnitSensitive(unit, m_Sensitive);
    m_downloadVal->setText(QString("%1").arg(downRate, 0, 'f', m_DecimalsNum, QLatin1Char(' ')) + downUnit);
    m_uploadValue->setText(QString("%1").arg(uploadRate, 0, 'f', m_DecimalsNum, QLatin1Char(' ')) + uploadUnit);

    m_down = down;
    m_upload = upload;
}

void SpeedWidget::updateCpu()
{
    long cpuAll = 0;
    long cpuFree = 0;
    infoUtils::cpuRate(cpuAll, cpuFree);
    m_cpuVal->setText(QString("%1%").arg((((cpuAll - m_cpuAll) - (cpuFree - m_cpuFree)) * 100.0 / (cpuAll - m_cpuAll)), 0, 'f', m_DecimalsNum, QLatin1Char(' ')));

    m_cpuAll = cpuAll;
    m_cpuFree = cpuFree;
}

void SpeedWidget::updateMem()
{
    long memory = 0;
    long memoryAll = 0;
    long swap = 0;
    long swapAll = 0;

    infoUtils::memoryRate(memory, memoryAll, swap, swapAll);
    m_memVal->setText(QString("%1%").arg(memory * 100.0 / memoryAll, 0, 'f', m_DecimalsNum, QLatin1Char(' ')));
}

void SpeedWidget::updateUptime()
{
    double run,idle;
    infoUtils::uptime(run,idle);
    int time = qFloor(run);
    int ss = time % 60;
    int MM = (time % 3600) / 60;
    int hh = (time % 86400) / 3600;
    int dd = time / 86400;

    //Argument Formats
    // f ->  format as [-]9.9
    QString runTime = QString(tr("系统已运行: %1天, %2:%3:%4"))
            .arg(dd, 0, 'f', 0, QLatin1Char(' '))
            .arg(hh, 2, 'f', 0, QLatin1Char('0'))
            .arg(MM, 2, 'f', 0, QLatin1Char('0'))
            .arg(ss, 2, 'f', 0, QLatin1Char('0'));

    m_uptime->setText(runTime);
}
