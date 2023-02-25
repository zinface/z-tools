#include "CommandUtil.h"

#include <QProcess>
#include <QTextStream>

QString CommandUtil::RunCommand(const QString command, const QStringList &params) {
    QProcess process;
    process.start(command, params, QProcess::ReadOnly);
    process.waitForFinished();

    QByteArray stdo = process.readAllStandardOutput();
    // QTextStream out(stdout);
    // out << stdo.toStdString().data() <<  "\n";
    QString result = QString::fromLocal8Bit(stdo).trimmed();
    return result;
}

/** CustomeCommand */ 
// QString CustomeCommand(const QStringList &params);
// QString CustomeCommand(const QStringList &params) {
//     return CommandUtil::RunCommand("arch", params);
// }