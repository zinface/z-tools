#ifndef DPKGUTILS_H
#define DPKGUTILS_H

#include <QObject>
#include <QRunnable>
//#include <QApt/Backend>
//using namespace QApt;

class DpkgUtils : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit DpkgUtils(QObject *parent = nullptr);

    void QueryListFiles(QString &packageName, QString &architecture);
    void QueryListFiles(QLatin1String &_packageName, QLatin1String &_architecture);
    // void QueryListFiles(QApt::Package &_package);

signals:
    void finished(QString text);

public slots:

    // QRunnable interface
public:
    void run() override;

private:
    QString command;
    QStringList arguments;


private:
    QString s_dpkg;             // dpkg
    QString s_dpkgQuery;        // dpkg-query
                                    // --listfiles <package>[:architecture]
    QString s_dpkgShlibdeps;    // dpkg-shlibdeps

};

#endif // DPKGUTILS_H
