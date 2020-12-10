#include "desktopexecparamdialog.h"

#include <QComboBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>

DesktopExecParamDialog::DesktopExecParamDialog(QWidget *parent) : QDialog(parent)
  ,paramComb(new QComboBox)
  ,paramChooseBtn(new QPushButton("确认"))
  ,paramMessageLabel(new QLabel("参数说明："))
  ,paramMessageContent(new QLabel("null"))
{
    setWindowTitle("可执行程序参数选择");
    initUi();
    onMessageContent();
    setFixedSize(400,200);

    connect(paramComb, &QComboBox::currentTextChanged, this, &DesktopExecParamDialog::onMessageContent);
//    connect(paramChooseBtn, &QPushButton::clicked, this, &DesktopExecParamDialog::onMessageContent);
    connect(paramChooseBtn, &QPushButton::clicked, this, &DesktopExecParamDialog::accept);
}

QString DesktopExecParamDialog::getParam()
{
    return paramComb->currentData().toString();
}

void DesktopExecParamDialog::initUi()
{
    paramMessageContent->setWordWrap(true);

    QHBoxLayout *paramChooseLayout = new QHBoxLayout;
    paramChooseLayout->addStretch();
    paramChooseLayout->addWidget(paramComb);
    paramChooseLayout->addWidget(paramChooseBtn);
    paramChooseLayout->addStretch();
    paramChooseLayout->setStretch(1,2);
    paramChooseLayout->setStretch(2,1);

    QHBoxLayout *paramMessageLayout = new QHBoxLayout;
    paramMessageLayout->addSpacing(10);
    paramMessageLayout->addWidget(paramMessageLabel);
    paramMessageLayout->setAlignment(paramMessageLabel, Qt::AlignTop | Qt::AlignRight);
    paramMessageLayout->addSpacing(10);
    paramMessageLayout->addWidget(paramMessageContent);
    paramMessageLayout->setAlignment(paramMessageContent, Qt::AlignTop | Qt::AlignLeft);
    paramMessageLayout->addStretch();
    paramMessageLayout->setSpacing(0);
    paramMessageLayout->setMargin(0);

    QVBoxLayout *paramSettingLayout = new QVBoxLayout;
    paramSettingLayout->addLayout(paramChooseLayout);
    paramSettingLayout->addSpacing(10);
    paramSettingLayout->addLayout(paramMessageLayout);
//    paramSettingLayout->setMargin(0);

    QGroupBox *paramSettingBox = new QGroupBox("参数设定");
    paramSettingBox->setLayout(paramSettingLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(paramSettingBox);
    mainLayout->setMargin(0);

    paramComb->addItem("%f", "%f");
    paramComb->addItem("%F", "%F");
    paramComb->addItem("%u", "%u");
    paramComb->addItem("%U", "%U");
//    paramComb->addItem("%d", "%d");
//    paramComb->addItem("%D", "%D");
//    paramComb->addItem("%n", "%n");
//    paramComb->addItem("%N", "%N");
//    paramComb->addItem("%i", "%i");
//    paramComb->addItem("%c", "%c");
//    paramComb->addItem("%k", "%k");
//    paramComb->addItem("%v", "%v");
//    paramComb->addItem("%m", "%m");
    setLayout(mainLayout);
}

void DesktopExecParamDialog::onMessageContent()
{
/** The Exec key
%f	A single file name (including the path), even if multiple files are selected. The system reading the desktop entry should recognize that the program in question cannot handle multiple file arguments, and it should should probably spawn and execute multiple copies of a program for each selected file if the program is not able to handle additional file arguments. If files are not on the local file system (i.e. are on HTTP or FTP locations), the files will be copied to the local file system and %f will be expanded to point at the temporary file. Used for programs that do not understand the URL syntax.
%F	A list of files. Use for apps that can open several local files at once. Each file is passed as a separate argument to the executable program.
%u	A single URL. Local files may either be passed as file: URLs or as file path.
%U	A list of URLs. Each URL is passed as a separate argument to the executable program. Local files may either be passed as file: URLs or as file path.
%d	Deprecated.
%D	Deprecated.
%n	Deprecated.
%N	Deprecated.
%i	The Icon key of the desktop entry expanded as two arguments, first --icon and then the value of the Icon key. Should not expand to any arguments if the Icon key is empty or missing.
%c	The translated name of the application as listed in the appropriate Name key in the desktop entry.
%k	The location of the desktop file as either a URI (if for example gotten from the vfolder system) or a local filename or empty if no location is known.
%v	Deprecated.
%m	Deprecated.
*/
// ％f  即使选择了多个文件，单个文件名（包括路径）也是如此。读取桌面条目的系统应认识到所讨论的程序无法处理多个文件参数，并且如果该程序无法处理其他文件参数，则应该为每个选定文件生成并执行该程序的多个副本。如果文件不在本地文件系统上（即位于HTTP或FTP位置），则文件将被复制到本地文件系统，并且％f将被扩展为指向临时文件。用于不了解URL语法的程序。
// ％F  文件列表。用于可一次打开多个本地文件的应用程序。每个文件都作为单独的参数传递给可执行程序。
// ％u  单个URL。本地文件可以作为文件URL或作为文件路径传递。
// ％U  URL列表。每个URL作为单独的参数传递给可执行程序。本地文件可以作为文件URL或作为文件路径传递。
// ％d  已弃用。
// ％D  已弃用。
// ％n  已弃用。
// ％N  已弃用。
// ％i  桌面项的Icon键已扩展为两个参数，第一个是--icon，然后是Icon键的值。如果Icon键为空或丢失，则不应扩展为任何参数。
// ％c  桌面条目中相应“名称”键中列出的应用程序的转换名称。
// ％k  桌面文件的位置，可以是URI（例如，例如从vfolder系统获取）或本地文件名，如果不知道位置，则为空。
// ％v  已弃用。
// ％m  已弃用。


    QString param = paramComb->currentText();
    if (param == "%f") {
        paramMessageContent->setText("单个文件名，即使选择了多个文件。如果已选择的文件不在本地文件系统中（比如说在HTTP或者FTP上），这个文件将被作为一个临时文件复制到本地，％f将指向本地临时文件；");
    } else if (param == "%F") {
        paramMessageContent->setText("文件列表，用于程序可以同时打开多个本地文件。每个文件以分割段的方式传递给执行程序。");
    } else if (param == "%u") {
        paramMessageContent->setText("单个URL，本地文件以文件URL或文件路径的方式传递。");
    } else if (param == "%U") {
        paramMessageContent->setText("URL列表，每个URL以分割段的方式传递给执行程序。本地文件以文件URL或文件路径的方式传递。");
    }

//一个文件名(包括路径)，即使选择了多个文件。读取桌面条目的系统应该认识到有问题的程序不能处理多个文件参数，如果程序不能处理额外的文件参数，它应该为每个选定的文件生成并执行多个程序副本。如果文件不在本地文件系统上(例如，在HTTP或FTP位置上)，文件将被复制到本地文件系统，%f将扩展到指向临时文件。用于不理解URL语法的程序。
    //文件列表。适用于可以同时打开多个本地文件的应用程序。每个文件都作为一个单独的参数传递给可执行程序。
}

void DesktopExecParamDialog::onParamSelected()
{
//    done(QDialog::Accepted);
    accept();
}


//%f：单个文件名，即使选择了多个文件。如果已选择的文件不在本地文件系统中（比如说在HTTP或者FTP上），这个文件将被作为一个临时文件复制到本地，％f将指向本地临时文件；
//%F：文件列表。用于程序可以同时打开多个本地文件。每个文件以分割段的方式传递给执行程序。
//%u：单个URL。本地文件以文件URL或文件路径的方式传递。
//%U：URL列表。每个URL以分割段的方式传递给执行程序。本地文件以文件URL或文件路径的方式传递。
//url link of: https://www.jianshu.com/p/a531ae0ab8af


void DesktopExecParamDialog::paintEvent(QPaintEvent *event)
{

}
