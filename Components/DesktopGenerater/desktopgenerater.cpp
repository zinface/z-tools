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

    setContentsMargins(0,0,0,0);
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

/**
 * @brief 信号槽-当输入框内容更改时刷新icon图标的显示
 */
void DesktopGenerater::onContentIconChanged()
{
    const auto ratio = qApp->devicePixelRatio();
    QSize size(32,32);
    QString content = contentIcon->text();
    QFile file(content);
    if (file.exists()) {
        QPixmap icon;
        if (icon.load(content)){
            icon = icon.scaled(size,Qt::IgnoreAspectRatio);
            icon.setDevicePixelRatio(ratio);
            contentIconPreview->setPixmap(icon);
        }else{
            contentIconPreview->clear();
        }
    } else {
        QPixmap icon = QIcon::fromTheme(contentIcon->text()).pixmap(32,32);
        icon.setDevicePixelRatio(ratio);
        contentIconPreview->setPixmap(icon);
    }
}

/**
 * @brief 信号槽-当文件类型被修改时，改变全局存储状态，并更新对应内容布局
 */
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
    QString fileName = QFileDialog::getSaveFileName(this, "保存到", fileFolderComb->currentData().toString(),"Desktop 描述文件 (*.desktop)");
    // 1.检查是否未选择目录与存储位置
    if (fileName.isEmpty()) {
        return;
    }

    // 2.检查是否包含.desktop后缀
    if (!fileName.endsWith(".desktop")) {
        fileName.append(".desktop");
    }

    // 3.检查是否可创建文件
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

/**
 * @brief 信号槽-当点击复制到剪贴板时触发
 */
void DesktopGenerater::onCopyClipper()
{
    onGeneraterContent();
    QClipboard *clipboard = QApplication::clipboard();
    QString originalText = clipboard->text();
    clipboard->setText(contentText->toPlainText());
    QMessageBox::information(this, "提示", "已复制到剪贴板!");
}

/**
 * @brief Desktop内容生成与组织函数
 *
 * 新增对 Type=Application|Link 的结构处理
 */
void DesktopGenerater::onGeneraterContent()
{
    //Type=Application|Link 的结构处理
    QString currentType = contentTypeComb->currentText();
    QString typeContent;
    if (currentType == "Application") {
        // 此处填充第二个%2时需要检查%1不为空，确保处理内容细节
        typeContent = QString("Exec=%1 %2").arg(contentExec->text()).arg((contentExec->text().trimmed().isEmpty()?"":m_currentParam));
    } else if (currentType == "Link") {
        typeContent = QString("URL=%1").arg((contentUrl->text().trimmed()));
    }//Type=Application|Link 的结构处理

    QStringList text;
    text << "[Desktop Entry]"
           << QString("Version=%1").arg(contentVersion->text())
           << QString("Name=%1").arg(contentName->text())
           << QString("Name[zh_CN]=%1").arg(contentNameZhCn->text())
           << QString("Comment=%1").arg(contentComment->text())
           << QString("Type=%1").arg(contentTypeComb->currentText())
           << typeContent
           << QString("Icon=%1").arg(contentIcon->text())
           << QString("Categories=%1").arg(contentCategoriesComb->currentText());
    QString s = text.join("\n");
    s.append("\n\n# Generated from the DesktopGenerater component of the z-Tools toolkit");
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
    int index = 0, execIndex, iconIndex;
    entryLayout->addWidget(fileFolderLabel,index,0,1,1);
    entryLayout->addWidget(fileFolderComb,index,1,1,1);
    entryLayout->addWidget(folderChoose,index,2,1,1);
    index++;
    entryLayout->addWidget(fileContentVersionLabel,index,0,1,1);
    entryLayout->addWidget(contentVersion,index,1,1,2);
    index++;
    entryLayout->addWidget(fileContentNameLabel,index,0,1,1);
    entryLayout->addWidget(contentName,index,1,1,2);
    index++;
    entryLayout->addWidget(fileContentNameZhCnLabel,index,0,1,1);
    entryLayout->addWidget(contentNameZhCn,index,1,1,2);
    index++;
    entryLayout->addWidget(fileContentCommentLabel,index,0,1,1);
    entryLayout->addWidget(contentComment,index,1,1,2);
    index++;
    entryLayout->addWidget(fileContentTypeLabel,index,0,1,1);
    entryLayout->addWidget(contentTypeComb,index,1,1,2);
    index++;
    entryLayout->addWidget(fileContentUrlLabel,index,0,1,1);
    entryLayout->addWidget(contentUrl,index,1,1,2);
    execIndex = ++index;
    entryLayout->addWidget(fileContentExecLabel,index,0,1,1);
    entryLayout->addWidget(contentExec,index,1,1,1);
    iconIndex = ++index;
    entryLayout->addWidget(fileContentIconLabel,index,0,1,1);
    entryLayout->addWidget(contentIcon,index,1,1,1);
    index++;
    entryLayout->addWidget(fileContentCategoriesLabel,index,0,1,1);
    entryLayout->addWidget(contentCategoriesComb,index,1,1,2);
    index++;
    entryLayout->setColumnMinimumWidth(2,170);

    // 执行文件:参数设置，文件选择
    QHBoxLayout *execParam = new QHBoxLayout;
    execParam->addWidget(execParamChoose);
    execParam->addWidget(execFileChoose);
    entryLayout->addLayout(execParam,execIndex,2,1,1);

    // 图标文件: 预览，选择图标
    QHBoxLayout *iconPreview = new QHBoxLayout;
    iconPreview->addWidget(contentIconPreview);
    iconPreview->addWidget(iconFileChoose);
    iconPreview->setSpacing(5);
    iconPreview->setSizeConstraint(QLayout::SetFixedSize);
    entryLayout->addLayout(iconPreview,iconIndex,2,1,1);

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
    contentTypeComb->addItem("Directory(未实现)","Directory");

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
    contentName->setPlaceholderText("例如：DesktopGenertaer (适用于字母搜索)");
    contentNameZhCn = new QLineEdit;
    contentNameZhCn->setPlaceholderText("例如：桌面图标生成器");
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
    fileContentNameLabel = new QLabel("搜索的名称:");
    fileContentNameZhCnLabel = new QLabel("显示的名称:");
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
