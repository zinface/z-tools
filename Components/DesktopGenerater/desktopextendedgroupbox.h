#ifndef DESKTOPEXTENDEDGROUPBOX_H
#define DESKTOPEXTENDEDGROUPBOX_H

#include <QGroupBox>
#include <QWidget>

class DesktopExtendedGroupBox : public QGroupBox
{
    Q_OBJECT

public:
    enum State
    {
        STATE_NORMAL,
        STATE_EXPAND,
    };

public:
    explicit DesktopExtendedGroupBox(const QString &title, QWidget *parent = nullptr, State state = STATE_NORMAL);

signals:
    void onChanged();

private Q_SLOTS:
    void onChecked(bool checked);

public:
    void addWidget(QWidget *widget);
    State getState() const;

private:
    QVector<QWidget*> children_;
    State state_;
};

#endif // DESKTOPEXTENDEDGROUPBOX_H
