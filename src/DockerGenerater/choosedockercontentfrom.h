#ifndef CHOOSEDOCKERCONTENTFROM_H
#define CHOOSEDOCKERCONTENTFROM_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>

class ChooseDockerContentFrom : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseDockerContentFrom(QWidget *parent = nullptr);
    void setShowAction(QString dialogTitle, QString actionTitle, QString placeholderText, QString actionCommand, QString actionDescripton);
    QString GetActionValue(){ return actionLineEdit->text(); };
private slots:
    void onConfirmChoose();

private:
    void initUi();
    void initContentTips();

private:
    QLabel *actionLineTitle;
    QLineEdit *actionLineEdit;
    QLabel *actionCommand;
    QLabel *actionCommandContent;
    QLabel *actionDescription;
    QLabel *actionDescriptionContent;

    QPushButton *confirmBtn;

};

#endif // CHOOSEDOCKERCONTENTFROM_H
