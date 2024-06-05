
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


class LoaderManager;
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

    QFileInfoList loadFileInfos(QString path, int depth = 1);
    void setDefaultPath(const QString &path);

    void setWindowHandler(QWindow *newWindowHandler);

private slots:
    void on_e_dirpath_textChanged(const QString &arg1);
    void on_listWidget_itemEntered(QListWidgetItem *item);
    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_widget_customContextMenuRequested(const QPoint &pos);

    void on_sp_maxdepth_valueChanged(int arg1);

    void refresh();

    void on_e_find_textChanged(const QString &arg1);
    void filtering_items(const QString &content);

signals:
    void messageEvent(const QString &message);

private:
    Ui::ImageView *ui;
    QLineEdit *m_dirpath_lineedit;
    QListWidget *m_image_list;
    QLabel *m_image_label;
    QString m_currentpaht;
    QPixmap m_currentPic;

    QWindow *m_windowHandler;
    LoaderManager *manager;

    // QWidget interface
protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // IMAGEVIEW_H

