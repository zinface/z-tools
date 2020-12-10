#include "desktopextendedgroupbox.h"

DesktopExtendedGroupBox::DesktopExtendedGroupBox(const QString &title, QWidget *parent /*= nullptr*/, State state /*= STATE_NORMAL*/) : QGroupBox (title, parent)
{
    setObjectName("DesktopExtendedGroupBox");
    setCheckable(true);
    state_ = state;
    if (state_ == STATE_NORMAL)
    {
        //隐藏垂直边框
        setFlat(true);
        setChecked(false);
    }
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onChecked(bool)));
    setStyleSheet("QGroupBox::indicator {width: 15px;height: 15px;}"
                  "QGroupBox::indicator:checked {image: url(:/images/uncheck.png)}"
                  "QGroupBox::indicator:unchecked {image: url(:/images/check.png)}");
    setFocusPolicy(Qt::NoFocus);
}

void DesktopExtendedGroupBox::addWidget(QWidget *widget)
{
    if (widget != nullptr) {
        if (state_ == STATE_NORMAL) {
            widget->setVisible(false);
        }
        children_.push_back(widget);
    }
}

void DesktopExtendedGroupBox::onChecked(bool checked)
{
    if(checked) {
        setFlat(false);
        for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
            (*iter)->setVisible(true);
        }
        state_ = STATE_EXPAND;
    } else {
         setFlat(true);
        for (auto iter = children_.begin(); iter != children_.end(); ++iter) {
            (*iter)->setVisible(false);
        }
        state_ = STATE_NORMAL;
    }

    emit onChanged();
}

DesktopExtendedGroupBox::State DesktopExtendedGroupBox::getState() const
{
    return state_;
}
