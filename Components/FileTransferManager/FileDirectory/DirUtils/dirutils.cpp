#include "dirutils.h"

DirUtils::DirUtils(QObject *parent) : QObject(parent)
{

}

QString DirUtils::directoryLevel(QString name, int level, DirUtils::DTYPE type)
{
    QString parent = "│   ";
    QString temp = "";
    for (int i = 0; i < level; i++) {
        temp.append(parent);
    }
    switch (type) {
    case DA:return QString("%1%2%3").arg(temp).arg("├── ").arg(name);
    case DB:return QString("%1%2%3").arg(temp).arg("└── ").arg(name);
    default:
        return QString();
        ;
    }
}

void DirUtils::directoryRecursion(QString path, int action, QTcpSocket *c)
{
    QDir dir(path);

    QByteArray local8Bit = QString(path).toLocal8Bit();
    manager.broadCaseAction(c, action, local8Bit.length(), local8Bit);
    if (!isDirEmpty(dir)) senderDir(dir, 0, action, c);
}

bool DirUtils::isDirEmpty(QDir dir)
{
    return dir.entryList().length() == 0;
}

void DirUtils::senderDir(QDir dir, int level, int action, QTcpSocket *c)
{
    int x = 0;
    QStringList entryList = dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);

    foreach(QString filename, entryList) {
        DTYPE dt = ++x == entryList.length()?DB :DA;
        QFileInfo info(dir.absoluteFilePath(filename));

        info.isDir() ? printDir(info.filePath(), dt, level+1, action, c)
        : senderFile(filename, dt, level, action, c);
    }
}

void DirUtils::printDir(QString path, DTYPE dt, int level, int action, QTcpSocket *c)
{
    QDir dir(path);
    // 打印当前目录
    printCurrnetDir(path, dt, level, action, c);

    // 文件数量计数
    senderDir(dir, level, action, c);
}

void DirUtils::senderFile(QString path, DTYPE dt, int level, int action, QTcpSocket *c)
{
    QDir dir(path);

    QByteArray local8Bit = directoryLevel(path, level, dt).toLocal8Bit();
    manager.broadCaseAction(c, action, local8Bit.length(), local8Bit);
}

void DirUtils::printDirectory(QFileInfo info, QString prefix, QString section, bool end)
{
    QStringList childEntrys = info.dir().entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    QTextStream out(stdout); out << parent << prefix << info.fileName() << "\n";flush(out);
    QString section = childEntrys.length() <= 1 || end? "   " : "|   ";
    printTree(QDir(info.filePath()), QString("%1%2")
                                          .arg(parent)
                                          .arg(section));
}

void DirUtils::printFile(QFileInfo info, QString prefix, QString section)
{
    QTextStream out(stdout);
    out << parent << prefix << info.fileName() << "\n";
    flush(out);
}

void DirUtils::printTree(QDir dir, QString prefix)
{
    QStringList entrys = dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
    int x = 0;

    foreach (QString entry, entrys) {
        x++;

        QFileInfo info(dir.absoluteFilePath(entry));
        QString prefix = (x == entrys.length()) ? "└── " : "├── ";
        if (info.isDir()) {
            QStringList childEntrys = info.dir().entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
            QTextStream out(stdout); out << parent << prefix << info.fileName() << "\n";flush(out);
            QString section = childEntrys.length() <= 1 || x == childEntrys.length()? "   " : "|   ";
            printTree(dir.absoluteFilePath(entry), QString("%1%2")
                                                  .arg(parent)
                                                  .arg(section));

//            printDirectory(info, parent, prefix, x == entry.length());
        } else {
            QTextStream out(stdout);
            out << parent << prefix << info.fileName() << "\n";
            flush(out);
            printFile(info,parent,prefix);
        }
    }
}

void DirUtils::directoryRecursion2(QString path)
{
    QDir curdir(path);
    printTree(curdir, "");
}
