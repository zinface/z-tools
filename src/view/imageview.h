
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

namespace Ui
{
    class ImageView;
};
class ImageView : public QWidget
{
    Q_OBJECT
public:
    explicit ImageView(QWidget *parent = nullptr);
    ~ImageView();

    QFileInfoList loadFileInfos(QString path);

private slots:
    void on_e_dirpath_textChanged(const QString &arg1);
    void on_listWidget_itemEntered(QListWidgetItem *item);
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    Ui::ImageView *ui;
    QLineEdit *m_dirpath_lineedit;
    QListWidget *m_image_list;
    QLabel *m_image_label;
    QString m_currentpaht;
    QPixmap m_currentPic;
};

#endif // IMAGEVIEW_H

