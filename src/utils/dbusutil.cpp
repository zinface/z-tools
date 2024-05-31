#include "dbusutil.h"

#include <QDBusInterface>

DBusUtil::DBusUtil(QObject *parent)
    : QObject{parent}
{

}

void DBusUtil::showFileLocation(QString filename)
{
    QDBusInterface interface("org.freedesktop.FileManager1", "/org/freedesktop/FileManager1", "org.freedesktop.FileManager1");
    if (interface.isValid())
        interface.call("ShowItems", QStringList() << filename, "");
}
