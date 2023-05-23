#ifndef SPLASHWINDOW_H
#define SPLASHWINDOW_H

#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QWidget>

class SplashWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SplashWindow(QWidget *parent = nullptr);

    void start();

signals:
    void done();

private:
    QTimer *loading;
    QLabel *loadingLabel;
    QMovie *movie;
};

#endif // SPLASHWINDOW_H
