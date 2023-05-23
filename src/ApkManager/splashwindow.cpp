#include "splashwindow.h"

#include <QVBoxLayout>

SplashWindow::SplashWindow(QWidget *parent) : QWidget(parent)
  ,loading(new QTimer)
  ,loadingLabel(new QLabel)
  ,movie(new QMovie("://resources/loading.gif"))
{
    loading->setInterval(300);
    loadingLabel->setMovie(movie);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addStretch();
    layout->addWidget(loadingLabel);
    layout->setAlignment(loadingLabel, Qt::AlignmentFlag::AlignHCenter);
    layout->addStretch();

    connect(loading, &QTimer::timeout, this, &SplashWindow::done);
    connect(loading, &QTimer::timeout, movie, &QMovie::stop);
    connect(loading, &QTimer::timeout, loading, &QTimer::stop);
}

void SplashWindow::start()
{
    movie->start();
    loading->start();
}
