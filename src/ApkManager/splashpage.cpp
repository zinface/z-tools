#include "splashpage.h"

#include <QVBoxLayout>

SplashPage::SplashPage(QWidget *parent)
    : QWidget(parent)
{
    initUI();

    m_gifTimer = new QTimer(this);
    m_gifTimer->setInterval(300);

    connect(m_gifTimer, &QTimer::timeout, this, &SplashPage::done);
    connect(m_gifTimer, &QTimer::timeout, m_gifMovie, &QMovie::stop);
    connect(m_gifTimer, &QTimer::timeout, m_gifTimer, &QTimer::stop);

}

void SplashPage::initUI()
{
    m_gifMovie = new QMovie("://resources/loading.gif");

    m_gifPanel = new QLabel(this);
    m_gifPanel->setMovie(m_gifMovie);

    m_pageLayout = new QVBoxLayout(this);
    m_pageLayout->addStretch();
    m_pageLayout->addWidget(m_gifPanel);
    m_pageLayout->setAlignment(m_gifPanel, Qt::AlignmentFlag::AlignHCenter);
    m_pageLayout->addStretch();
}

void SplashPage::setGif(const QString &gif)
{
    m_gifMovie->setFileName(gif);
}

void SplashPage::start(int interval)
{    
    m_gifMovie->start();

    if (m_gifTimer->isActive())
        m_gifTimer->stop();

    m_gifTimer->setInterval(interval);
    m_gifTimer->start();
}
