#ifndef DESKTOPCUSTOMECONTENT_H
#define DESKTOPCUSTOMECONTENT_H

#include <QDialog>
#include <QTextEdit>

class QTextEdit;
class QPushButton;
class QVBoxLayout;

class DesktopCustomeContent : public QTextEdit
{
    Q_OBJECT
public:
    explicit DesktopCustomeContent(QWidget *parent = nullptr);

private:
    int fitted_height;

private slots:
    void fitHeightToDocument();


    // QWidget interface
public:
    QSize sizeHint() const;

};

#endif // DESKTOPCUSTOMECONTENT_H
