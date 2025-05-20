#include "matcher.h"
#include "view.h"
#include "window.h"
#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <qdebug.h>
#include <qglobal.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    Matcher matcher("org.ayatana.bamf", "/org/ayatana/bamf/matcher", QDBusConnection::sessionBus());
    auto paths = matcher.WindowPaths();
    paths.waitForFinished();
    
    if (paths.isValid()) {
        // qDebug() << paths.value();
        foreach  (auto path, paths.value()) {
            qDebug() << "--------------------";
            qDebug() << "Window:" << path;
            Window window("org.ayatana.bamf", path, QDBusConnection::sessionBus());
            auto pid = window.GetPid();
            pid.waitForFinished();
            QFileInfo file(QString("/proc/%1/exe").arg(pid));
            qDebug() << "PID:" << pid.value() << file.symLinkTarget();

            View view("org.ayatana.bamf", path, QDBusConnection::sessionBus());
            auto name = view.name();
            auto icon = view.icon();

            // name.waitForFinished();
            // qDebug() << "Name:" << name;
            
            auto parents = view.Parents();
            parents.waitForFinished();

            // qDebug() << "Parents:" << parents.value() << "," << "Count:" << parents.value().count();
            if (parents.value().count() > 0) {
                View parent("org.ayatana.bamf", parents.value().first(), QDBusConnection::sessionBus());
                auto parentName = parent.name();
                auto parentIcon = parent.icon();
                auto parentPath = parent.path();

                qDebug() << "Name:" << name;
                qDebug() << "icon:" << icon;
                qDebug() << "parentName:" << parentName;
                qDebug() << "parentIcon:" << parentIcon;
                qDebug() << "parentPath:" << parentPath;
            }
        }
    }

    return 0;
}
