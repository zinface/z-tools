#include "inforecivercommands.h"

#include <linuxinfomation.h>


InfoReciverCommands::InfoReciverCommands(QObject *parent) : QObject(parent)
  ,_work("")
  ,_host("localhost")
  ,_port(8888)
{
    connect(&manager, &InfomationManager::connected, this, &InfoReciverCommands::onConnected);
    connect(&manager, &InfomationManager::ClientSocketUnConnected, this, &InfoReciverCommands::onUnConnected);
    connect(&manager, &InfomationManager::onReply, this, &InfoReciverCommands::onReplay);

    manager.registerAction(LinuxInfomation::CPU);
    manager.registerAction(LinuxInfomation::MEM);
    manager.registerAction(LinuxInfomation::NETUP);
    manager.registerAction(LinuxInfomation::NETDOWN);
    manager.registerAction(LinuxInfomation::UPTIME);
}

void InfoReciverCommands::setWorkMode(QString mode)
{
    this->_work = mode;
}

bool InfoReciverCommands::setWorkHost(QString host)
{
    this->_host = host;
    return true;
}

bool InfoReciverCommands::setWorkPort(int port)
{
    this->_port = port;
    return true;
}

void InfoReciverCommands::start()
{
    this->manager.setManagerTask(this->_host, this->_port, SessionManager::CLIENT);
}

void InfoReciverCommands::onConnected()
{
    if (this->_work == "CPU") {
        manager.broadCaseAction(LinuxInfomation::CPU);
    } else if (this->_work == "MEM") {
        manager.broadCaseAction(LinuxInfomation::MEM);
    } else if (this->_work == "NETUP") {
        manager.broadCaseAction(LinuxInfomation::NETUP);
    } else if (this->_work == "NETDOWN") {
        manager.broadCaseAction(LinuxInfomation::NETDOWN);
    } else if (this->_work == "UPTIME") {
        manager.broadCaseAction(LinuxInfomation::UPTIME);
    } else {
        QTextStream(stdout) << QString("好家伙，无工作项\n");
        exit(0);
    }
}

void InfoReciverCommands::onUnConnected()
{
    exit(0);
}

void InfoReciverCommands::onReplay(int action, QString msg)
{
    if (action == LinuxInfomation::CPU) {
        QTextStream(stdout) << QString("好家伙, CPU信息 %1.\n").arg(msg);
    } else if (action == LinuxInfomation::MEM) {
        QTextStream(stdout) << QString("好家伙, 内存信息 %1.\n").arg(msg);
    } else if (action == LinuxInfomation::NETUP) {
        QTextStream(stdout) << QString("好家伙, 上传信息 %1.\n").arg(msg);
    } else if (action == LinuxInfomation::NETDOWN) {
        QTextStream(stdout) << QString("好家伙, 下载信息 %1.\n").arg(msg);
    } else if (action == LinuxInfomation::UPTIME){
        QTextStream(stdout) << QString("好家伙, 系统信息 %1.\n").arg(msg);
    }
    exit(0);
}

