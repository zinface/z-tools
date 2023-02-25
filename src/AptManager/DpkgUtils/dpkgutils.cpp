#include "dpkgutils.h"

#include <CommandUtil/CommandUtil.h>

DpkgUtils::DpkgUtils(QObject *parent) : QObject(parent)
  , s_dpkg("dpkg")
  , s_dpkgQuery("dpkg-query")
  , s_dpkgShlibdeps("dpkg-shlibdeps")
{

}

void DpkgUtils::QueryListFiles(QString &packageName, QString &architecture)
{
    arguments.clear();

    command = s_dpkgQuery;
    arguments << "--listfiles";

    if (architecture.isEmpty()) {
        arguments << packageName;
    } else {
        arguments << QString("%1:%2").arg(packageName).arg(architecture);
    }
}

void DpkgUtils::QueryListFiles(QLatin1String &_packageName, QLatin1String &_architecture)
{
    QString packageName = _packageName;
    QString packageArch = _architecture;
    QueryListFiles(packageName, packageArch);
}

void DpkgUtils::run()
{
    QString result = CommandUtil::RunCommand(command, arguments);
    emit finished(result);

}
