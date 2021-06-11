#include "inforecivercommands.h"
#include "infosendercommands.h"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTextStream>
#include <linuxinfomation.h>


int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("InfoCommands");
    app.setApplicationVersion("v0.1");

    // Common Param
    QCommandLineOption showWidget(QStringList() << "wm", "图形化查看运行信息");

    QCommandLineOption showCpu(QStringList() << "cpu", "查看CPU信息");
    QCommandLineOption showMemory(QStringList() << "mem", "查看内存信息");
    QCommandLineOption showNetUpload(QStringList() << "netup", "查看上传信息");
    QCommandLineOption showNetDownload(QStringList() << "netdown", "查看下载信息");
    QCommandLineOption showUptime(QStringList() << "uptime", "查看运行信息");

    QCommandLineOption workType(QStringList() << "t" << "type", "指定本程序运行模式(默认: Local)", "Server|Client|Local", "Server");
    QCommandLineOption workHost(QStringList() << "s" << "host", "Client模式指定要连接的主机地址(默认: localhost)","localhost");
    QCommandLineOption workPort(QStringList() << "p" << "port", "指定工作端口(默认: 8888)","8888");


    // Gui Param

    QCommandLineParser parser;
    parser.setApplicationDescription("系统信息CLI命令行版.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions(QList<QCommandLineOption>() << workType
                      << workHost
                      << workPort
                      << showCpu
                      << showMemory
                      << showNetUpload
                      << showNetDownload
                      << showUptime
                      << showWidget);

    parser.process(app);

    LinuxInfomation info;
    InfoSenderCommands sender;
    InfoReciverCommands receiver;

    bool m = false;

    if (parser.isSet(workPort)) {
        QString port = parser.value(workPort);
        if (port.isEmpty()) goto _noport;
        bool isSuccess;
        int num = port.toInt(&isSuccess);
        if (!isSuccess) goto _noport;
        sender.setWorkPort(num);
        receiver.setWorkPort(num);
    } else {
    _noport:
        ;
    }

    if (parser.isSet(workType)) {
        QString localValue = parser.value(workType);
        if (!localValue.isEmpty()) {
            if (localValue == "Local") goto _local;
            if (localValue == "Server") goto _server;
            if (localValue == "Client") goto _client;
            goto _failwork;
        } else {
        _failwork:
            QTextStream(stdout) << QString("好家伙,你乱指定运行模式. \n\tNOTE: -t %1\n").arg(workType.description());
            return -1;
        }
    } else {
    _local:
        info.netstart();
        if (parser.isSet(showCpu)) {
            QTextStream(stdout) << QString("好家伙, CPU信息 %1.\n").arg(info.getCPU());
        }

        if (parser.isSet(showMemory)) {
            QTextStream(stdout) << QString("好家伙, 内存信息 %1.\n").arg(info.getMemory());
        }

        if (parser.isSet(showNetUpload)) {
            QTextStream(stdout) << QString("好家伙, 网络信息 %1.\n").arg(info.getNetUpload());
        }

        if (parser.isSet(showNetDownload)) {
            QTextStream(stdout) << QString("好家伙, 网络信息 %1.\n").arg(info.getNetDownload());
        }

        if (parser.isSet(showUptime)) {
            QTextStream(stdout) << QString("好家伙, 系统信息 %1.\n").arg(info.getUptime());
        }
        exit(0);
    _server:
        sender.start();
        return app.exec();

    _client:
        if (parser.isSet(workHost)) {
            QString host = parser.value(workHost);
            if (host.isEmpty()) goto _nohost;
            if(!receiver.setWorkHost(host)) {
                QTextStream(stdout) << QString("好家伙,指定主机不可用或未能连接成功\n");
                return -1;
            }
        } else {
        _nohost:
          receiver.setWorkHost("localhost");
        }

        if (parser.isSet(showCpu)) {
            receiver.setWorkMode("CPU");m = true;
        }

        if (parser.isSet(showMemory)) {
            receiver.setWorkMode("MEM");m = true;
        }

        if (parser.isSet(showNetUpload)) {
            receiver.setWorkMode("NETUP");m = true;
        }
        if (parser.isSet(showNetDownload)) {
            receiver.setWorkMode("NETDOWN");m = true;
        }

        if (parser.isSet(showUptime)) {
            receiver.setWorkMode("UPTIME");m = true;
        }
        receiver.start();
        return app.exec();
    }
}
