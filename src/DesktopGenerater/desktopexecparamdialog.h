#ifndef DESKTOPEXECPARAMDIALOG_H
#define DESKTOPEXECPARAMDIALOG_H

#include <QDialog>

class QComboBox;
class QPushButton;
class QLabel;

class DesktopExecParamDialog : public QDialog
{
    Q_OBJECT
public:
    explicit DesktopExecParamDialog(QWidget *parent = nullptr);
    QString getParam();

private slots:
    void onMessageContent();
    void onParamSelected();

private:
    void initUi();
    QComboBox *paramComb;
    QPushButton *paramChooseBtn;
    QLabel *paramMessageLabel;
    QLabel *paramMessageContent;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // DESKTOPEXECPARAMDIALOG_H
