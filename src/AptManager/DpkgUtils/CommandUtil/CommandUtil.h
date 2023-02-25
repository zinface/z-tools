#ifndef __CommandUtil__H__
#define __CommandUtil__H__

#include <QObject>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE
class CommandUtil
{
public:
    static QString RunCommand(const QString command, const QStringList &params);
};

#endif  //!__CommandUtil__H__