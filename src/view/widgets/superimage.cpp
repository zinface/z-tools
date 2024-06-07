#include "superimage.h"

#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>

#include <wheeleater.h>
#include <mouseeater.h>

#include <screenutil.h>

SuperImage::SuperImage(QWidget *parent)
    : QWidget{parent}
    , m_ratio(1.0)
{
    animation = new QPropertyAnimation(this, "");
    animation->stop();
    animation->setStartValue(0.1);
    animation->setEndValue(1.0);
    animation->setDuration(300);

    connect(animation, &QPropertyAnimation::valueChanged, this, [this](const QVariant & value)
    {
        double v = value.toDouble();
        m_ratio = v;
        update();
    });
    connect(animation, &QPropertyAnimation::finished, this, [this]()
    {
        m_is_animate = false;
        m_ratio = 1.0;
        update();
    });

    new MouseEater([this](QMouseEvent * event)
    {
        static QPoint mousePoint;
        static bool mousePressed = false;

        if (event->type() == QMouseEvent::MouseButtonPress)
        {
            mousePressed = true;
            mousePoint = event->globalPos() - m_pos;
        }
        else if (event->type() == QMouseEvent::MouseButtonRelease)
        {
            mousePressed = false;
        }
        else if (event->type() == QMouseEvent::MouseMove)
        {
            m_is_move = true;
            m_is_wheel = false;

            m_pre_is_move = true;
            m_pre_is_wheel = false;
            if (mousePressed && (event->buttons() & Qt::LeftButton))
            {
                m_pos = event->globalPos() - mousePoint;
                update();
            }
        }

        // 如果滚动是在动画过程中，则立即停止动画，并响应滚动操作
        if (event->type() == QMouseEvent::Wheel)
        {
            m_is_wheel = true;
            m_is_move = false;

            m_pre_is_wheel = true;
            m_pre_is_move = false;

            if (m_is_animate)
            {
                m_is_animate = false;
                animation->stop();
                m_ratio = 1.0;
            }

            int step = 0;
            auto wheel = (QWheelEvent *)event;
            m_wheel_pos = event->pos();

            if (wheel->angleDelta().isNull() == false)
            {
                step = wheel->angleDelta().y();
            }

            m_ratio = step > 0 ? m_ratio + 0.09 : m_ratio - 0.09;

            // 修正 m_ratio 不能小于 0.1
            m_ratio = m_ratio < 0.1 ? 0.1 : m_ratio;

            update();
        }

        return false;

    }, this);
}

void SuperImage::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setPen(Qt::NoPen);
    painter.setBrush(Qt::NoBrush);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 先绘制边框减少视觉上的视差
    painter.save();
    QPen green(QColor("#008000"));
    green.setWidth(4);
    painter.setPen(green);
    painter.drawRect(event->rect());
    painter.restore();

    // 再绘制剩下的图片
    qreal ratio = ScreenUtil::ratio(this);

    QRect event_rect = event->rect();
    QSize event_size = event->rect().size() - QSize(5, 5);

    if (m_is_animate)
    {
        // 绘制一个符合控件比例大小的图
        m_pixmap_copy = m_pixmap.scaled(event_size, Qt::KeepAspectRatio, Qt::FastTransformation);
        QRect render_rect = m_pixmap_copy.rect();
        QSize render_size = m_pixmap_copy.rect().size();

        // 记录位置
        render_rect.moveCenter(event->rect().center());
        m_pos = render_rect.topLeft();

        // 用于模糊动画，绘制一个比目标小的图时，这个图将会异常错位
        QSize render = render_size * m_ratio;
        m_pixmap_copy = m_pixmap_copy.scaled(render, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        painter.drawPixmap(render_rect, m_pixmap_copy);

        emit messageEvent(QString("加载中: %1%").arg(m_ratio * 100));
    }
    else
    {
        QSize render = event_size * m_ratio;
        // 绘制一个符合控件比例大小的图

        m_pixmap_copy = m_pixmap.scaled(render, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        QRect render_rect = m_pixmap_copy.rect();
        QSize render_size = m_pixmap_copy.rect().size();
        render_rect.moveCenter(event_rect.center());

        // 只是渲染时大小有变
        render_rect.setSize(render_size);

        if (m_is_move)
        {
            emit messageEvent(QString("移动: x:%1, y: %2").arg(m_pos.x()).arg(m_pos.y()));
            render_rect.moveTo(m_pos);
        }
        else if (m_is_wheel && (render_rect.width() < event->rect().width() || render_rect.height() < event->rect().height()))
        {
            emit messageEvent(QString("缩放: %1% (图片居中)").arg(100 * m_ratio));
            render_rect.moveCenter(event->rect().center());
            m_pos = render_rect.topLeft();
        }
        else if (m_is_wheel)
        {
            emit messageEvent(QString("缩放: %1%").arg(100 * m_ratio));
            // TODO: 计算位置

//            double sw = double(render_rect.width()) / double(m_old_render_rect.width());
//            double sh = double(render_rect.height()) / double(m_old_render_rect.height());

//            QPointF newpos;
//            QPointF oldpos = m_wheel_pos;

//            newpos.setX(oldpos.x() * sw);
//            newpos.setY(oldpos.y() * sh);

//            auto pos = newpos - oldpos;
        }
        else if (m_is_resize)
        {
            emit messageEvent(QString("视口调整: w:%1, h:%2").arg(event->rect().width()).arg(event->rect().height()));
            render_rect.moveCenter(event_rect.center());
        }
        else
        {
            render_rect.moveCenter(event_rect.center());
        }

        m_pos = render_rect.topLeft();

//        m_is_move = false;
//        m_is_wheel = false;
//        m_is_resize = false;
        m_old_render_rect = render_rect;

        painter.drawPixmap(render_rect, m_pixmap_copy);
    }
}

void SuperImage::resizeEvent(QResizeEvent *event)
{
    m_is_resize = true;
    QWidget::resizeEvent(event);
}

void SuperImage::setPixmap(const QPixmap &newPixmap)
{
    m_pixmap = newPixmap;
    m_pixmap_copy = m_pixmap.copy();

    // 新图进入，启动动画
    m_is_animate = true;
    animation->start();
}
