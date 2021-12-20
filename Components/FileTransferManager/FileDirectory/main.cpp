#include "directoryreceiver.h"
#include "directorysender.h"

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineOption workType(QStringList() << "t" << "type", "指定本程序运行模式(默认: Server)", "Server|Client|Any|Daemon", "Server");
    QCommandLineOption workDepth(QStringList() << "--depth", "workDepth", "0-10", "0");
    QCommandLineOption workDir(QStringList() << "d" << "directory", "指定被接收文件存储到该目录(Client默认为当前目录)\n",".");

    QCommandLineParser parser;
    parser.setApplicationDescription("目录传输CLI命令行版.");
    parser.addHelpOption();

    parser.addOptions(QList<QCommandLineOption>() << workType
                      << workDir
                      << workDepth);

    parser.process(app);

    DirectorySender sender;
    DirectoryReceiver receiver;

    struct OptionHandle{
        QCommandLineOption option;
        void (*handle)(void*);
    };

    OptionHandle options[] = {
        { workType, [](void*){}},
        { workDepth, [](void*){}},
    };


    if (parser.isSet(workDepth)) {
        QString localValue = parser.value(workDepth);
        if (!localValue.isEmpty()) {
            bool isSuccess;
            int num = localValue.toInt(&isSuccess);
            if (!isSuccess) goto _nodepthnumber;
            receiver.setDepth(num);
        } else {
        _nodepthnumber:
            QTextStream(stdout) << QString("好家伙,你乱指定运行模式. \n\tNOTE: -t %1\n").arg(workType.description());
        }
    } else {

    }

    if (parser.isSet(workType)) {
        QString localValue = parser.value(workType);
        if (!localValue.isEmpty()) {
            if (localValue == "Server") goto _sender;
            if (localValue == "Client") goto _client;
            goto _failwork;
        } else {
        _failwork:
            QTextStream(stdout) << QString("好家伙,你乱指定运行模式. \n\tNOTE: -t %1\n").arg(workType.description());
            return -1;
        }
    } else {
    _sender:
        if (parser.isSet(workDir)) {
            QString localdir = parser.value(workDir);
            if (localdir.isEmpty()) {
                QTextStream(stdout) << QString("好家伙,未指定存储目录,将不启用接收文件功能.\n");
                goto _noserverexistsdir;
            }
            if (!QDir(localdir).exists()) {
                QTextStream(stdout) << QString("好家伙,指定了不存在的目录(%1),将不启用接收文件功能\n").arg(localdir);
                goto _noserverexistsdir;
            }
//            d = true;
            QTextStream(stdout) << QString("好家伙,已指定共享目录: %1\n").arg(localdir);
            sender.setWorkDir(localdir);
        } else {
        _noserverexistsdir:
            sender.setWorkDir(".");
        }

        sender.start();
        return app.exec();

    _client:
        if (parser.isSet(workDir)) {
            QString localdir = parser.value(workDir);
            if (localdir.isEmpty()) {
                QTextStream(stdout) << QString("好家伙,未指定存储目录,将以当前目录为存储目录.\n");
                goto _noexistsdir;
            }
            if (!QDir(localdir).exists()) {
                QTextStream(stdout) << QString("好家伙,指定了不存在的目录(%1),将不启用接收文件功能\n").arg(localdir);
                return -1;
            }
//            d = true;
            QTextStream(stdout) << QString("好家伙,已指定存储目录: %1\n").arg(localdir);
            receiver.setWorkDir(localdir);
        } else {
        _noexistsdir:
            receiver.setWorkDir(".");
        }

        receiver.start();
        return app.exec();
    }
}
