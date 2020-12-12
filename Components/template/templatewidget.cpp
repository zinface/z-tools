#include "templatewidget.h"

#include <QIcon>
#include <QLabel>
#include <QLayout>

TemplateWidget::TemplateWidget(QWidget *parent) : QWidget(parent)
{

    QLabel *icon = new QLabel;
    //400,280
    QImage image("://img/icon-system-components.png");
    icon->setPixmap(QPixmap::fromImage(image));


    QBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(icon);
    setLayout(mainLayout);
    resize(400,280);
}

TemplateWidget::TemplateWidget(QString index, QWidget *parent)
{
    QLabel *icon = new QLabel;
    //400,280
    QImage image(QString("://img/icon-system-components%1.png").arg(index));
    icon->setPixmap(QPixmap::fromImage(image));


    QBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(icon);
    setLayout(mainLayout);
    resize(400,280);
}
