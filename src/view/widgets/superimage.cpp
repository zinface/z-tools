#include "superimage.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include <wheeleater.h>
#include <mouseeater.h>

#include <utils/ScreenUtils.h>

SuperImage::SuperImage(QWidget *parent)
    : QWidget{parent}
    , m_ratio(1.0)
{
    animation = new QPropertyAnimation(this, "");
    animation->stop();
    animation->setStartValue(0.1);
    animation->setEndValue(1.0);
    animation->setDuration(400);

    new MouseEater([this](QMouseEvent *event){
        static QPoint mousePoint;
        static bool mousePressed = false;

        if (event->type() == QMouseEvent::Wheel) {
            auto wheel = (QWheelEvent *)event;
            QPoint degress;
            int step = 0;

            if (m_animate) {
                m_animate = false;
                animation->stop();
                m_ratio = 1.0;
            }

            degress = wheel->angleDelta();
            if (degress.isNull() == false)
            {
                step = degress.y();
            }
            m_ratio = step > 0 ? m_ratio + 0.08 : m_ratio - 0.08;
            m_ratio = m_ratio < 0.1 ? 0.1 : m_ratio;
            update();
        }

        if (event->type() == QMouseEvent::MouseButtonPress){
            mousePressed = true;
            mousePoint = event->globalPos() - m_pos;
        } else if (event->type() == QMouseEvent::MouseButtonRelease) {
            mousePressed = false;
        } else if (event->type() == QMouseEvent::MouseMove) {
            if (mousePressed && (event->buttons() & Qt::LeftButton)) {
                m_pos = event->globalPos() - mousePoint;
                update();
            }
        }
        return false;
    }, this);
}

void SuperImage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.save();

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);


    if (m_first) {
        m_first = false;
        m_animate = true;

        connect(animation, &QPropertyAnimation::valueChanged, this, [this](const QVariant &value){
            double v = value.toDouble();
            m_ratio = v;
            update();
        });
        connect(animation, &QPropertyAnimation::finished, this, [this](){
            m_animate = false;
            update();
        });
        animation->start();
        return;
    }

//    if (pixmap.width() > render_size.width() || pixmap.height() > render_size.height())
//        pixmap = pixmap.scaled(render_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    }
    //        static QSize anisize = event->rect().size();
    //        static QPixmap anipixmap = pixmap;
    //        static qreal aniratio = ratio;
    //        anipixmap.setDevicePixelRatio(aniratio);
    //        rect.moveCenter(event->rect().center());
    //        m_pos = rect.topLeft();

    //        connect(&animation, &QPropertyAnimation::valueChanged, this, [this](const QVariant &value){
    //            double v = value.toDouble();
    //            QSize render = anisize * v;
    //            if (anipixmap.width() < render.width() && anipixmap.height() < render.height())
    //            {
    //                anipixmap = anipixmap;
    //            } else {
    //                anipixmap = anipixmap.scaled(render, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    //            }
    //            update();
    //        });


    qreal ratio = ScreenUtils::ratio(this);
    QPixmap pixmap = m_pixmap;
    pixmap.setDevicePixelRatio(ratio);

    QRect event_rect = event->rect();
    QSize event_size = event->rect().size();

    if (m_animate) {
        // 绘制一个符合控件比例大小的图
        pixmap = pixmap.scaled(event_size, Qt::KeepAspectRatio, Qt::FastTransformation);
        QRect render_rect = pixmap.rect();
        QSize render_size = pixmap.rect().size();
        // 记录位置
        render_rect.moveCenter(event->rect().center());
        m_pos = render_rect.topLeft();
        // 用于模糊动画，绘制一个比目标小的图时，这个图将会异常错位
        pixmap = pixmap.scaled(render_size * m_ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(render_rect, pixmap);
    } else {
        // 绘制一个符合控件比例大小的图
        pixmap = pixmap.scaled(event_size * m_ratio, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QRect render_rect = pixmap.rect();
        QSize render_size = pixmap.rect().size();
        // 只是渲染时大小有变
        render_rect.setSize(render_size);
        if (m_resize) {
            m_resize = false;
            render_rect.moveCenter(event_rect.center());
        } else {
            render_rect.moveTo(m_pos);
        }
        painter.drawPixmap(render_rect, pixmap);
    }

    /**
     * 稳定的一种
     */
//    render_rect = event->rect();
//    render_size = event->rect().size() * m_ratio;
//    pixmap = pixmap.scaled(render_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//    render_rect.setSize(pixmap.size());
//    if (m_animate) {
    //        render_rect.moveCenter(event->rect().center());

    QPen green(QColor("#008000"));
    green.setWidth(4);
    painter.setPen(green);
    painter.drawRect(event->rect());

}

void SuperImage::resizeEvent(QResizeEvent *event)
{
    m_resize = true;
}

void SuperImage::setPixmap(const QPixmap &newPixmap)
{
    m_pixmap = newPixmap;
    m_first = true;
    update();
}
