#ifndef DBUSUTIL_H
#define DBUSUTIL_H

#include <QObject>

class DBusUtil : public QObject
{
    Q_OBJECT
    explicit DBusUtil(QObject *parent = nullptr);
public:

    static void showFileLocation(QString filename);

signals:

};

#endif // DBUSUTIL_H
