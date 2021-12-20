#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("FileWindow");

    MainWindow wm;
    wm.show();

    return app.exec();
}

