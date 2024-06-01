#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QCommandLineParser>
#include <QUrl>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("图片浏览器 - v0.4.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("图片浏览器");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("dir", "要加载的目录路径", "[dir]");
    
    QCommandLineOption versionOption(QStringList() << "v" << "version",
                                    QCoreApplication::translate("main", "显示版本信息"));
    parser.addOption(versionOption);
    parser.process(a);

    const QStringList args = parser.positionalArguments();

    MainWindow wm;

    if (!args.isEmpty()) {
        QUrl url(args.first());
        QString localPath = url.toLocalFile();
        wm.loaderDirectory(localPath);
    }
    wm.show();

    return a.exec();
}
