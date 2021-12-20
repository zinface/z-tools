#ifndef DIRUTILS_H
#define DIRUTILS_H

#include <QObject>

class DirUtils : public QObject
{
    Q_OBJECT
public:
    explicit DirUtils(QObject *parent = nullptr);

    enum DTYPE {
        DA,
        DB,
    };

    QString directoryLevel(QString name, int level = 0, DTYPE type = DA);
    void directoryRecursion(QString path, int action, QTcpSocket *c);

private:
    inline bool isDirEmpty(QDir dir);
    void senderDir(QDir dir,int level, int action, QTcpSocket *c);
    void printDir(QString path, DTYPE dt, int level, int action, QTcpSocket *c);
    void senderFile(QString path, DTYPE dt, int level, int action, QTcpSocket *c);
    void printCurrnetDir(QString path, DTYPE dt, int level,int action, QTcpSocket *c) {
//        if (level != 0) {
            QByteArray local8Bit = directoryLevel(QFileInfo(path).baseName(), level-1, dt).toLocal8Bit();
            manager.broadCaseAction(c, action, local8Bit.length(), local8Bit);
//        }
    };


    void printDirectory(QFileInfo info, QString prefix, QString section, bool end);
    void printFile(QFileInfo info, QString prefix, QString section);
    void printTree(QDir dir, QString prefix);
    void directoryRecursion2(QString path);
signals:

};

#endif // DIRUTILS_H
