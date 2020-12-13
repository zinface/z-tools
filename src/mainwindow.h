#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QVBoxLayout;

class SpeedWidget;
QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QVBoxLayout *m_mainLayout;
    QTabWidget *m_tabWidget;
};

#endif // MAINWINDOW_H
