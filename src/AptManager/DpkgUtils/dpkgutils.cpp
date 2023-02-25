#include "dpkgutils.h"

#include <CommandUtil/CommandUtil.h>

DpkgUtils::DpkgUtils(QObject *parent) : QObject(parent)
  , s_dpkg("dpkg")
  , s_dpkgQuery("dpkg-query")
  , s_dpkgShlibdeps("dpkg-shlibdeps")
  , m_dpkgCommandType(Normal)
{

}

void DpkgUtils::Search(QString &v)
{
    arguments.clear();

    command = s_dpkg;
    arguments << "--search" << v;

    m_dpkgCommandType = DpkgSearch;
    m_dpkg_search_key = v;
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
    if (m_dpkgCommandType == DpkgSearch) {
        emit searchFinished(m_dpkg_search_key, result);
    }
    emit finished(result);

}
