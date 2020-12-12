#ifndef TEMPLATEWIDGET_H
#define TEMPLATEWIDGET_H

#include <QWidget>

class TemplateWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TemplateWidget(QWidget *parent = nullptr);
    explicit TemplateWidget(QString index, QWidget *parent = nullptr);
signals:

public slots:
};

#endif // TEMPLATEWIDGET_H
