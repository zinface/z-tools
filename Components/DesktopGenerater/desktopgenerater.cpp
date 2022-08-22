#include "desktopcustomecontent.h"
#include "desktopexecparamdialog.h"
#include "desktopextendedgroupbox.h"
#include "desktopgenerater.h"
#include "ui_desktopgenerater.h"

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
#include <QStandardPaths>
#include <QTextEdit>
#include <QTextStream>

DesktopGenerater::DesktopGenerater(QWidget *parent) : QWidget(parent)
  ,ui(new Ui::DesktopGenertater)
{
    ui->setupUi(this);

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
}

void DesktopGenerater::initFileLocal()
{
    fileFolderComb = ui->fileFolderComb;
    fileFolderComb->clear();
     fileFolderComb->addItem(s_userApplicationTargetDir, QStandardPaths::writableLocation(QStandardPaths::ApplicationsLocation));
}

void DesktopGenerater::initTypes()
{
    contentTypeComb = ui->contentTypeComb;
    connect(contentTypeComb, &QComboBox::currentTextChanged, this,&DesktopGenerater::onContentTypeChanged);
}

void DesktopGenerater::initCategories()
{
    contentCategoriesComb = ui->contentCategoriesComb;
}

void DesktopGenerater::initContentEditeLine()
{
    contentVersion = ui->contentVersion;
    contentVersion->setText("1.0");
    contentVersion->setReadOnly(true);
    contentVersion->setEnabled(false);

    contentName = ui->contentName;
    contentNameZhCn = ui->contentNameZhCn;
    contentComment = ui->contentComment;
    contentUrl = ui->contentUrl;
    contentExec = ui->contentExec;
    contentIcon = ui->contentIcon;
}

void DesktopGenerater::initContentWidgets()
{
    fileFolderLabel = ui->fileFolderLabel;
    folderChoose = ui->folderChoose;
    execFileChoose = ui->execFileChoose;
    execParamChoose = ui->execParamChoose;
    iconFileChoose = ui->iconFileChoose;

    fileContentVersionLabel = ui->fileContentVersionLabel;
    fileContentNameLabel = ui->fileContentNameLabel;
    fileContentNameZhCnLabel = ui->fileContentNameZhCnLabel;
    fileContentCommentLabel = ui->fileContentCommentLabel;
    fileContentTypeLabel = ui->fileContentTypeLabel;
    fileContentUrlLabel = ui->fileContentUrlLabel;
    fileContentExecLabel = ui->fileContentExecLabel;
    fileContentIconLabel = ui->fileContentIconLabel;
    fileContentCategoriesLabel = ui->fileContentCategoriesLabel;

    contentIconPreview = ui->contentIconPreview;
    contentIconPreview->setFixedSize(32,32);
    contentIconPreview->setPixmap(QIcon::fromTheme("apper").pixmap(32,32));

    fileContentUrlLabel->hide();
    contentUrl->hide();
}

void DesktopGenerater::initShowContentBox()
{

    contentText = ui->contentText;
    contentText->setReadOnly(true);
    contentText->setFixedHeight(100);

    copyClipper = ui->copyClipper;
    saveAsFile = ui->saveAsFile;


    contentGroupBox = ui->contentGroupBox;
    contentGroupBox->setState(DesktopExtendedGroupBox::STATE_NORMAL);


    contentGroupBox->addWidget(contentText);
    contentGroupBox->addWidget(copyClipper);
    contentGroupBox->addWidget(saveAsFile);
}
