#ifndef ADBPKGVIEW_H
#define ADBPKGVIEW_H

#include <QListView>
#include <QStringListModel>
#include <QWidget>

class AdbPkgView : public QListView
{
    Q_OBJECT
public:
    explicit AdbPkgView(QWidget *parent = nullptr);
    void appendData(const QString& item);
    void setData(QStringList items);

public slots:
    
    void enable_pkg(QString pkg);
    void disable_pkg(QString pkg);

private:
    QStringList m_data;
    QStringListModel *m_model;
};

#endif // ADBPKGVIEW_H
