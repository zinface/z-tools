#include "infosendercommands.h"

InfoSenderCommands::InfoSenderCommands(QObject *parent) : QObject(parent)
  ,_port(8888)
{
    connect(&manager, &InfomationManager::onRemoteFetch, this, &InfoSenderCommands::onRemoteFetch);
    manager.registerAction(LinuxInfomation::CPU);
    manager.registerAction(LinuxInfomation::MEM);
    manager.registerAction(LinuxInfomation::NETUP);
    manager.registerAction(LinuxInfomation::NETDOWN);
    manager.registerAction(LinuxInfomation::UPTIME);

}

bool InfoSenderCommands::setWorkPort(int port)
{
    this->_port = port;
}

void InfoSenderCommands::start()
{
    this->manager.setManagerTask("", this->_port, SessionManager::SERVER);
    info.netstart();
}

void InfoSenderCommands::onRemoteFetch(int action, QTcpSocket *c)
{
    if (action == LinuxInfomation::CPU) {
        manager.broadCaseAction(c, action, info.getCPU());
    } else if (action == LinuxInfomation::MEM) {
        manager.broadCaseAction(c, action, info.getMemory());
    } else if (action == LinuxInfomation::NETUP) {
        manager.broadCaseAction(c, action, info.getNetUpload());
    } else if (action == LinuxInfomation::NETDOWN) {
        manager.broadCaseAction(c, action, info.getNetDownload());
    } else if (action == LinuxInfomation::UPTIME){
        manager.broadCaseAction(c, action, info.getUptime());
    }
}
