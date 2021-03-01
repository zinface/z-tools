#include "choosedockercontentfrom.h"

#include <QSizePolicy>
#include <QVBoxLayout>

ChooseDockerContentFrom::ChooseDockerContentFrom(QWidget *parent) : QDialog(parent)
{
    initUi();

    connect(confirmBtn,&QPushButton::clicked, this, &ChooseDockerContentFrom::onConfirmChoose);
}

void ChooseDockerContentFrom::setShowAction(QString dialogTitle, QString actionTitle, QString placeholderText, QString actionCommand, QString actionDescripton)
{
    setWindowTitle(dialogTitle);
    actionLineTitle->setText(actionTitle);
    actionLineEdit->setPlaceholderText(placeholderText);

    actionCommandContent->setText(actionCommand);
    actionDescriptionContent->setText(actionDescripton);
}

void ChooseDockerContentFrom::onConfirmChoose()
{
    accept();
}

void ChooseDockerContentFrom::initUi()
{
    initContentTips();

    actionLineTitle = new QLabel();
    actionLineEdit = new QLineEdit();
    actionLineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    confirmBtn = new QPushButton("确认");

    actionCommand = new QLabel("动作指令：");
    actionCommandContent = new QLabel();
    actionDescription = new QLabel("描述：");
    actionDescriptionContent = new QLabel();

    QHBoxLayout *chooseContentLayout = new QHBoxLayout;
    chooseContentLayout->addWidget(actionLineTitle);
    chooseContentLayout->addWidget(actionLineEdit);
    chooseContentLayout->addWidget(confirmBtn);
    actionLineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


    QGridLayout *tipDescriptionLayout = new QGridLayout;
    tipDescriptionLayout->addWidget(actionCommand,1,0,1,1,Qt::AlignRight | Qt::AlignBottom);
    tipDescriptionLayout->addWidget(actionCommandContent,1,1,1,1,Qt::AlignLeft | Qt::AlignBottom);
    tipDescriptionLayout->addWidget(actionDescription,2,0,1,1,Qt::AlignRight | Qt::AlignTop);
    tipDescriptionLayout->addWidget(actionDescriptionContent,2,1,1,1,Qt::AlignLeft | Qt::AlignTop);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(chooseContentLayout);
    mainLayout->addLayout(tipDescriptionLayout);

    setLayout(mainLayout);
    layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void ChooseDockerContentFrom::initContentTips()
{

}
