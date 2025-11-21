#ifndef SPLASHPAGE_H
#define SPLASHPAGE_H

#include <QLabel>
#include <QMovie>
#include <QTimer>
#include <QWidget>

class QVBoxLayout;
class SplashPage : public QWidget
{
    Q_OBJECT
public:
    explicit SplashPage(QWidget *parent = nullptr);
    void initUI();

    void setGif(const QString &gif);

    void start(int interval = 300);

signals:
    void done();

private:
    QMovie *m_gifMovie;
    QLabel *m_gifPanel;
    QTimer *m_gifTimer;

    QVBoxLayout *m_pageLayout;
};

#endif // SPLASHPAGE_H
