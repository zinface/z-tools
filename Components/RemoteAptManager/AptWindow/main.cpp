#include <QApplication>
#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QStyleFactory>
#include <aptmanager.h>

int main(int argc, char *argv[])
{
#ifdef Q_OS_MACOS
    qApp->setStyle(QStyleFactory::create("fusion"));
#endif
    QApplication app(argc, argv);
    QApplication::setApplicationName("AptWindow");
    app.setApplicationVersion("v0.1");

    QCommandLineOption showWidget(QStringList() << "wm", "图形化查看运行信息");

    QCommandLineOption workHost(QStringList() << "s" << "host", "Client模式指定要连接的主机地址(默认: localhost)","localhost");
    QCommandLineOption workPort(QStringList() << "p" << "port", "指定工作端口(默认: 8888)","8888");

    QCommandLineParser parser;
    parser.setApplicationDescription("远程包管理信息GUI命令行版.");
    parser.addHelpOption();
    parser.addVersionOption();

    parser.addOptions(QList<QCommandLineOption>()
                      << workHost
                      << workPort);

    parser.process(app);

    AptManager manager;
    if (parser.isSet(workHost)) {
        QString host = parser.value(workHost);
        if (host.isEmpty()) goto _nohost;
        if(!manager.setWorkHost(host)) {
            QTextStream(stdout) << QString("好家伙,指定主机不可用或未能连接成功\n");
            return -1;
        }
    } else {
    _nohost:;
      manager.setWorkHost("localhost");
    }

    if (parser.isSet(workPort)) {
        QString port = parser.value(workPort);
        if (port.isEmpty()) goto _noport;
        bool isSuccess;
        int num = port.toInt(&isSuccess);
        if (!isSuccess) goto _noport;
        manager.setWorkPort(num);
    } else {
    _noport:
        ;
    }
    manager.start();
    return app.exec();
}
