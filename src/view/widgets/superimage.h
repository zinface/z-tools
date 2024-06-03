#ifndef SUPERIMAGE_H
#define SUPERIMAGE_H

#include <QLabel>
#include <QPropertyAnimation>

class SuperImage : public QWidget
{
    Q_OBJECT
public:
    explicit SuperImage(QWidget *parent = nullptr);
    void setPixmap(const QPixmap &newPixmap);

signals:
    void messageEvent(const QString &message);

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    QPixmap m_pixmap;
    QPixmap m_pixmap_copy;
    QPropertyAnimation *animation;

    bool m_first;
    bool m_is_animate;
    bool m_is_move;
    bool m_is_wheel;
    bool m_is_resize;

    bool m_pre_is_move;
    bool m_pre_is_wheel;

    double m_ratio;
    QPoint m_pos;

    QPoint m_wheel_pos;
    QRect m_old_render_rect;
};

#endif // SUPERIMAGE_H
