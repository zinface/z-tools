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

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    QPixmap m_pixmap;
    bool m_first;
    double m_ratio;
    QPoint m_pos;

    QPropertyAnimation *animation;
    bool m_animate;
    bool m_resize;
};

#endif // SUPERIMAGE_H
