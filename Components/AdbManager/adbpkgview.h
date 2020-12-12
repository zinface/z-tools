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


private:
    QStringList m_data;
    QStringListModel *m_model;
};

#endif // ADBPKGVIEW_H
