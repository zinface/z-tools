#ifndef APKICONSHOWMAINWINDOW_H
#define APKICONSHOWMAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class ApkIconShowMainWindow;
}

class ApkIconShowMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ApkIconShowMainWindow(QWidget *parent = nullptr);
    ~ApkIconShowMainWindow();

    void createPlaceholderIcon();

private:
    Ui::ApkIconShowMainWindow *ui;
    QString tmpAppIcon = "";

};

#endif // APKICONSHOWMAINWINDOW_H
