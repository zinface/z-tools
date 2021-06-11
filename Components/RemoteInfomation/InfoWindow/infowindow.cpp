#include "infowindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QLabel>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>
#include <QPoint>

InfoWindow::InfoWindow(QWidget *parent) : QWidget(parent)
  ,_host("localhost")
  ,_port(8888)
  ,mCpuUsed(new QLabel("(null)"))
  ,mMemUsed(new QLabel("(null)"))
  ,mNetUpload(new QLabel("(null)"))
  ,mNetDownload(new QLabel("(null)"))
  ,mUptime(new QLabel("(null)"))
  ,timer(new QTimer())

{
    //setAttribute(Qt::WA_TransparentForMouseEvents, true);
//    setAttribute(Qt::WA_TranslucentBackground, false);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_MacAlwaysShowToolWindow, true);

    setWindowFlags(Qt::X11BypassWindowManagerHint |Qt::MacWindowToolBarButtonHint| Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);

    QVBoxLayout *netLayout = new QVBoxLayout;
    netLayout->addWidget(mNetUpload);
    netLayout->setAlignment(mNetUpload, Qt::AlignRight | Qt::AlignTop);
    netLayout->addWidget(mNetDownload);
    netLayout->setAlignment(mNetDownload, Qt::AlignRight | Qt::AlignTop);

    QVBoxLayout *cpumemLayout = new QVBoxLayout;
    cpumemLayout->addWidget(mCpuUsed);
    cpumemLayout->setAlignment(mCpuUsed, Qt::AlignRight | Qt::AlignTop);
    cpumemLayout->addWidget(mMemUsed);
    cpumemLayout->setAlignment(mMemUsed, Qt::AlignRight | Qt::AlignTop);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addLayout(netLayout);
    topLayout->addLayout(cpumemLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(mUptime);
    mainLayout->setAlignment(mUptime, Qt::AlignRight | Qt::AlignTop);
    mainLayout->addStretch();
    QDesktopWidget *localDesktop = QApplication::desktop();
    setFixedSize(230,100);

    setLayout(mainLayout);

    move(localDesktop->width()-this->width(),0);

    connect(timer,&QTimer::timeout, this, &InfoWindow::onTimerout);
    connect(&manager, &InfomationManager::connected, this, &InfoWindow::onConnected);
    connect(&manager, &InfomationManager::ClientSocketUnConnected, this, &InfoWindow::onUnConnected);
    connect(&manager, &InfomationManager::onReply, this, &InfoWindow::onReplay);

    manager.registerAction(LinuxInfomation::CPU);
    manager.registerAction(LinuxInfomation::MEM);
    manager.registerAction(LinuxInfomation::NETUP);
    manager.registerAction(LinuxInfomation::NETDOWN);
    manager.registerAction(LinuxInfomation::UPTIME);
}

bool InfoWindow::setWorkHost(QString host)
{
    this->_host = host;
    return true;
}

bool InfoWindow::setWorkPort(int port)
{
    this->_port = port;
    return true;
}

bool InfoWindow::start()
{
    QTextStream(stdout) << QString("好家伙, 准备连接：%1: %2\n").arg(this->_host).arg(this->_port);
    manager.setManagerTask(this->_host, this->_port, SessionManager::CLIENT);
    this->show();
    return true;
}

void InfoWindow::onConnected()
{
    timer->setInterval(100);
    timer->start();
}

void InfoWindow::onUnConnected()
{
    exit(0);
}

void InfoWindow::onReplay(int action, QString msg)
{
    if (action == LinuxInfomation::CPU) {
        mCpuUsed->setText(msg);
    }
    if (action == LinuxInfomation::MEM) {
        mMemUsed->setText(msg);
    }
    if (action == LinuxInfomation::NETUP) {
        mNetUpload->setText(msg);
    }
    if (action == LinuxInfomation::NETDOWN) {
        mNetDownload->setText(msg);
    }
    if (action == LinuxInfomation::UPTIME) {
        mUptime->setText(msg);
    }
}

QPoint temp;
bool press = false;
bool db = false;
void InfoWindow::mousePressEvent(QMouseEvent *e){
    if (e->buttons() == Qt::LeftButton && e->type() == QMouseEvent::MouseButtonDblClick)
    {
        press = true;
        temp = e->globalPos() - this->pos();
        e->accept();
    }
}
void InfoWindow::mouseMoveEvent(QMouseEvent *e) {
    if (press && e->buttons() == Qt::LeftButton)
    {
        move(e->globalPos() - temp);
        e->accept();
    }
}
void InfoWindow::mouseReleaseEvent(QMouseEvent *e) {
    Q_UNUSED(e)
    press = false;
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
}

void InfoWindow::onTimerout()
{
    manager.broadCaseAction(LinuxInfomation::CPU);
    manager.broadCaseAction(LinuxInfomation::MEM);
    manager.broadCaseAction(LinuxInfomation::NETUP);
    manager.broadCaseAction(LinuxInfomation::NETDOWN);
    manager.broadCaseAction(LinuxInfomation::UPTIME);
}
