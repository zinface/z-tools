#include "aptreceivercommands.h"
#include "aptsendercommands.h"

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QTextStream>
#include <QStyleFactory>

#include <AptCommands/AptUtils/aptutils.h>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACOS
    qApp->setStyle(QStyleFactory::create("fusion"));
#endif
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("AptCommands");
    app.setApplicationVersion("v0.1");

    // Common Param
    QCommandLineOption workType(QStringList() << "t" << "type", "指定本程序运行模式(默认: Local)", "Server|Client|Local", "Server");
    QCommandLineOption workHost(QStringList() << "s" << "host", "Client模式指定要连接的主机地址(默认: localhost)","localhost");
    QCommandLineOption workPort(QStringList() << "p" << "port", "指定工作端口(默认: 8888)","8888");

    QCommandLineParser parser;
    parser.setApplicationDescription("系统信息GUI命令行版.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions(QList<QCommandLineOption>() << workType
                      << workHost
                      << workPort);

    parser.process(app);

    AptUtils utils;
    AptSenderCommands sender;
    AptReceiverCommands receiver;


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
        QTextStream(stdout) << QString("本地已安装：%1 镜像源：%2\n")
                               .arg(utils.GetInstalledPackagesCount())
                               .arg(utils.GetMirrorsPackagesCount());
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
        return app.exec();
    }
}
