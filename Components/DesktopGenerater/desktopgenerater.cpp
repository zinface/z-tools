#include "desktopcustomecontent.h"
#include "desktopexecparamdialog.h"
#include "desktopextendedgroupbox.h"
#include "desktopgenerater.h"

#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QComboBox>
#include <QFileDialog>
#include <QGridLayout>
#include <QGroupBox>
#include <QIcon>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>
#include <QTextStream>

DesktopGenerater::DesktopGenerater(QWidget *parent) : QWidget(parent)
{
    initUi();

    connect(folderChoose, &QPushButton::clicked, this, &DesktopGenerater::onFolderChoose);
    connect(execParamChoose, &QPushButton::clicked, this, &DesktopGenerater::onExecParamChoose);
    connect(execFileChoose, &QPushButton::clicked, this, &DesktopGenerater::onExecFileChoose);
    connect(iconFileChoose, &QPushButton::clicked, this, &DesktopGenerater::onIconFileChoose);
    connect(contentIcon, &QLineEdit::textChanged, this, &DesktopGenerater::onContentIconChanged);
    connect(contentGroupBox, &DesktopExtendedGroupBox::onChanged, this, &DesktopGenerater::onGeneraterContent);
    connect(saveAsFile, &QPushButton::clicked, this, &DesktopGenerater::onSaveAsFile);
    connect(copyClipper, &QPushButton::clicked, this, &DesktopGenerater::onCopyClipper);

    setFixedWidth(560);
}

void DesktopGenerater::onFolderChoose()
{

    QString srcDirPath = QFileDialog::getExistingDirectory(this, "选择文件夹", s_homeDir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (srcDirPath.length() > 0)
        fileFolderComb->addItem(srcDirPath,srcDirPath);
    fileFolderComb->setCurrentText(srcDirPath);
}

void DesktopGenerater::onExecFileChoose()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory(m_currentDir.isEmpty()?s_homeDir:m_currentDir);
    if(dialog.exec() != QDialog::Accepted) {
        return;
    }

    const QString file = dialog.selectedFiles().at(0);
    contentExec->setText(file);
    m_currentDir = QFileInfo(file).dir().path();
}

void DesktopGenerater::onIconFileChoose()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setDirectory(m_currentDir.isEmpty()?s_homeDir:m_currentDir);
    dialog.setNameFilters(QStringList() //<< "*.jpg *.jpeg *.jpe *.png *.svg *.bmp *.xbm *.icon *.bitmap *.xpm"
                                        << "ALL (*.jpg *.jpeg *.jpe *.png *.svg *.bmp *.xbm *.icon *.bitmap *.xpm)"
                                        << "JPEG 图像 (*.jpg *.jpeg *.jpe)"
                                        << "PNG 图像 (*.png)"
                                        << "SVG 图像 (*.svg)"
                                        << "Windows BMP 图像 (*.bmp)"
                                        << "BitMap 图像 (*.xbm *.icon *.bitmap)"
                                        << "PixMap 图像 (*.xpm)");
    if(dialog.exec() != QDialog::Accepted)
        return;
    const QString file = dialog.selectedFiles().at(0);
    contentIcon->setText(file);
    m_currentDir = QFileInfo(file).dir().path();
}

void DesktopGenerater::onContentIconChanged()
{
    QSize size(32,32);
    QString content = contentIcon->text();
    QFile file(content);
    if (file.exists()) {
        QPixmap p;
        if (p.load(content)){
            p = p.scaled(size,Qt::IgnoreAspectRatio);
            contentIconPreview->setPixmap(p);
        }else{
            contentIconPreview->clear();
        }
    } else {
        contentIconPreview->setPixmap(QIcon::fromTheme(contentIcon->text()).pixmap(32,32));
    }
}

void DesktopGenerater::onContentTypeChanged()
{
    QString current_Type = contentTypeComb->currentText();
    if (current_Type == "Application"){
        fileContentUrlLabel->hide();
        contentUrl->hide();

        fileContentExecLabel->show();
        contentExec->show();
        execParamChoose->show();
        execFileChoose->show();
    } else if (current_Type == "Link"){
        fileContentUrlLabel->show();
        contentUrl->show();

        fileContentExecLabel->hide();
        contentExec->hide();
        execParamChoose->hide();
        execFileChoose->hide();
    } else if (current_Type == "Directory"){
        fileContentUrlLabel->hide();
        contentUrl->hide();

        fileContentExecLabel->hide();
        contentExec->hide();
        execParamChoose->hide();
        execFileChoose->hide();
    }
}

void DesktopGenerater::onExecParamChoose()
{
    DesktopExecParamDialog dialog;
    if(dialog.exec() != QDialog::Accepted){
        m_currentParam = "";
        execParamChoose->setText("参数设定");
        return;
    }
    m_currentParam = dialog.getParam();
    execParamChoose->setText(m_currentParam);
}

void DesktopGenerater::onSaveAsFile()
{
    onGeneraterContent();
    QString fileName = QFileDialog::getSaveFileName(this, "保存到", fileFolderComb->currentData().toString());
    QFile file(fileName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法保存文件：" + file.errorString());
        return;
    }

    setWindowTitle(fileName);
    QTextStream out(&file);
    out << contentText->toPlainText();
    file.close();
    QMessageBox::information(this, "提示", "文件已保存!");
}

void DesktopGenerater::onCopyClipper()
{
    onGeneraterContent();
    QClipboard *clipboard = QApplication::clipboard();
    QString originalText = clipboard->text();
    clipboard->setText(contentText->toPlainText());
    QMessageBox::information(this, "提示", "已复制到剪贴板!");
}

void DesktopGenerater::onGeneraterContent()
{
    QStringList text;
    text << "[Desktop Entry]"
           << QString("Version=%1").arg(contentVersion->text())
           << QString("Name=%1").arg(contentName->text())
           << QString("Comment=%1").arg(contentComment->text())
           << QString("Type=%1").arg(contentTypeComb->currentText())
           << QString("Exec=%1").arg(contentExec->text())
           << QString("Icon=%1").arg(contentIcon->text())
           << QString("Categories=%1").arg(contentCategoriesComb->currentText());
    QString s;
    foreach(const QString str, text){
        s.append(str + "\n");
    }

    contentText->setText(s);
}

void DesktopGenerater::initUi()
{
    initTypes();
    initFileLocal();
    initCategories();
    initContentEditeLine();
    initContentWidgets();
    initShowContentBox();

    QGridLayout *entryLayout = new QGridLayout;
    entryLayout->addWidget(fileFolderLabel,0,0,1,1);
    entryLayout->addWidget(fileFolderComb,0,1,1,1);
    entryLayout->addWidget(folderChoose,0,2,1,1);
    entryLayout->addWidget(fileContentVersionLabel,1,0,1,1);
    entryLayout->addWidget(contentVersion,1,1,1,2);
    entryLayout->addWidget(fileContentNameLabel,2,0,1,1);
    entryLayout->addWidget(contentName,2,1,1,2);
    entryLayout->addWidget(fileContentCommentLabel,3,0,1,1);
    entryLayout->addWidget(contentComment,3,1,1,2);
    entryLayout->addWidget(fileContentTypeLabel,4,0,1,1);
    entryLayout->addWidget(contentTypeComb,4,1,1,2);
    entryLayout->addWidget(fileContentUrlLabel,5,0,1,1);
    entryLayout->addWidget(contentUrl,5,1,1,2);
    entryLayout->addWidget(fileContentExecLabel,6,0,1,1);
    entryLayout->addWidget(contentExec,6,1,1,1);
    entryLayout->addWidget(fileContentIconLabel,7,0,1,1);
    entryLayout->addWidget(contentIcon,7,1,1,1);
    entryLayout->addWidget(fileContentCategoriesLabel,8,0,1,1);
    entryLayout->addWidget(contentCategoriesComb,8,1,1,2);
    entryLayout->setColumnMinimumWidth(2,170);

    QHBoxLayout *execParam = new QHBoxLayout;
    execParam->addWidget(execParamChoose);
    execParam->addWidget(execFileChoose);
    entryLayout->addLayout(execParam,6,2,1,1);

    QHBoxLayout *iconPreview = new QHBoxLayout;
    iconPreview->addWidget(contentIconPreview);
    iconPreview->addWidget(iconFileChoose);
    iconPreview->setSpacing(5);
    iconPreview->setSizeConstraint(QLayout::SetFixedSize);
    entryLayout->addLayout(iconPreview,7,2,1,1);


    QGroupBox *desktopEntryBox = new QGroupBox("Desktop Entry");
    desktopEntryBox->setLayout(entryLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(desktopEntryBox);
    mainLayout->setAlignment(desktopEntryBox, Qt::AlignTop);
    mainLayout->addWidget(contentGroupBox);
    mainLayout->setAlignment(contentGroupBox, Qt::AlignTop);
    mainLayout->setParent(parent());
    setLayout(mainLayout);
}

void DesktopGenerater::initFileLocal()
{
    fileFolderComb = new QComboBox;
    fileFolderComb->addItem(s_userApplicationTargetDir,s_homeDir+"/.local/share/applications");
}

void DesktopGenerater::initTypes()
{
    contentTypeComb = new QComboBox;
    contentTypeComb->addItem("Application","Application");
    contentTypeComb->addItem("Link","Link");
    contentTypeComb->addItem("Directory","Directory");

    connect(contentTypeComb, &QComboBox::currentTextChanged, this,&DesktopGenerater::onContentTypeChanged);
}

void DesktopGenerater::initCategories()
{
    contentCategoriesComb = new QComboBox;
    contentCategoriesComb->addItem("AudioVideo","AudioVideo");
    contentCategoriesComb->addItem("Audio","Audio");
    contentCategoriesComb->addItem("Video","Video");
    contentCategoriesComb->addItem("Development","Development");
    contentCategoriesComb->addItem("Education","Education");
    contentCategoriesComb->addItem("Game","Game");
    contentCategoriesComb->addItem("Graphics","Graphics");
    contentCategoriesComb->addItem("Network","Network");
    contentCategoriesComb->addItem("Office","Office");
    contentCategoriesComb->addItem("Science","Science");
    contentCategoriesComb->addItem("Settings","Settings");
    contentCategoriesComb->addItem("System","System");
    contentCategoriesComb->addItem("Utility","Utility");
}

void DesktopGenerater::initContentEditeLine()
{
    contentVersion = new QLineEdit;
    contentVersion->setText("1.0");
    contentVersion->setReadOnly(true);
    contentVersion->setEnabled(false);

    contentName = new QLineEdit;
    contentName->setPlaceholderText("例如：桌面图标生成器");
    contentComment = new QLineEdit;
    contentComment->setPlaceholderText("例如：创建简单的desktop图标文件");
    contentUrl = new QLineEdit;
    contentUrl->setPlaceholderText("例如：https://www.bing.com");
    contentExec = new QLineEdit;
    contentExec->setPlaceholderText("例如：/usr/bin/desktopGenerater %f");
    contentIcon = new QLineEdit;
    contentIcon->setPlaceholderText("例如：apper");
}

void DesktopGenerater::initContentWidgets()
{
    fileFolderLabel = new QLabel("文件路径存放:");
    folderChoose = new QPushButton("选择路径");
    execFileChoose = new QPushButton("选择文件");
    execParamChoose = new QPushButton("参数设定");
    iconFileChoose = new QPushButton("选择图标");

    fileContentVersionLabel = new QLabel("文件版本:");
    fileContentNameLabel = new QLabel("显示的名称:");
    fileContentCommentLabel = new QLabel("文件说明:");
    fileContentTypeLabel = new QLabel("文件类型:");
    fileContentUrlLabel = new QLabel("Url链接:");
    fileContentExecLabel = new QLabel("执行文件选择:");
    fileContentIconLabel = new QLabel("图标文件选择:");
    fileContentCategoriesLabel = new QLabel("程序分类:");

    contentIconPreview = new QLabel;
    contentIconPreview->setFixedSize(32,32);
    contentIconPreview->setPixmap(QIcon::fromTheme("apper").pixmap(32,32));

    fileContentUrlLabel->hide();
    contentUrl->hide();
}

void DesktopGenerater::initShowContentBox()
{

    contentText = new QTextEdit;
    contentText->setReadOnly(true);
    contentText->setFixedHeight(100);

    copyClipper = new QPushButton("复制到剪贴板");
    saveAsFile = new QPushButton("保存到文件");

    contentGroupBox = new DesktopExtendedGroupBox("Show Content", this, DesktopExtendedGroupBox::STATE_NORMAL);

    QHBoxLayout *contentOperatLayout = new QHBoxLayout;
    contentOperatLayout->addWidget(copyClipper);
    contentOperatLayout->addWidget(saveAsFile);
    contentOperatLayout->setAlignment(copyClipper, Qt::AlignTop | Qt::AlignRight);
    contentOperatLayout->setAlignment(saveAsFile, Qt::AlignTop | Qt::AlignRight);

    QVBoxLayout *contentBoxLayout = new QVBoxLayout;
    contentBoxLayout->addWidget(contentText);
    contentBoxLayout->setAlignment(contentText, Qt::AlignTop);
    contentBoxLayout->addLayout(contentOperatLayout);
    contentBoxLayout->setAlignment(contentOperatLayout, Qt::AlignTop | Qt::AlignRight);

    contentGroupBox->addWidget(contentText);
    contentGroupBox->addWidget(copyClipper);
    contentGroupBox->addWidget(saveAsFile);

    contentGroupBox->setLayout(contentBoxLayout);
}
