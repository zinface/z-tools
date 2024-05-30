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

    enum DpkgCommandType {
        Normal,
        DpkgSearch,
        DpkgQueryListFiles
    };

    void Search(QString &v);

    void QueryListFiles(QString &packageName, QString &architecture);
    void QueryListFiles(QLatin1String &_packageName, QLatin1String &_architecture);
    // void QueryListFiles(QApt::Package &_package);

signals:
    void finished(QString text);
    void searchFinished(QString key, QString text);

public slots:

    // QRunnable interface
public:
    void run() override;

private:
    QString command;
    QStringList arguments;


private:
    QString s_dpkg;             // dpkg
                                    // -S,--search
    QString s_dpkgQuery;        // dpkg-query
                                    // --listfiles <package>[:architecture]
    QString s_dpkgShlibdeps;    // dpkg-shlibdeps


private:
    DpkgCommandType m_dpkgCommandType;

    // --search key
    QString m_dpkg_search_key;
};

#endif // DPKGUTILS_H
