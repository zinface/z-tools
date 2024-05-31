
#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QFileInfoList>
#include <QWidget>
#include <qlistwidget.h>

QT_BEGIN_NAMESPACE
class QLineEdit;
class QListWidget;
class QLabel;
QT_END_NAMESPACE

class ImageView : public QWidget
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = nullptr);
    ~ImageView();

    void initUi();
    QFileInfoList loadFileInfos(QString path);

private slots:
    void onChangePath();
    void onSelectImageItem(QListWidgetItem *item);
    void onResize();

private:
    QLineEdit *m_dirpath_lineedit;
    QListWidget *m_image_list;
    QLabel *m_image_label;
    QString m_currentpaht;
};

#endif // IMAGEVIEW_H

