#ifndef DESKTOPGENERATER_H
#define DESKTOPGENERATER_H

#include <QDir>
#include <QWidget>

class QLabel;
class QComboBox;
class QPushButton;
class QGroupBox;
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QCheckBox;
class QTextEdit;
class DesktopExtendedGroupBox;
class DesktopCustomeContent;


class DesktopGenerater : public QWidget
{
    Q_OBJECT
public:
    explicit DesktopGenerater(QWidget *parent = nullptr);

signals:

public slots:
    void onFolderChoose();
    void onExecFileChoose();
    void onIconFileChoose();
    void onContentIconChanged();
    void onContentTypeChanged();
    void onExecParamChoose();
    void onSaveAsFile();
    void onCopyClipper();

    void onGeneraterContent();

private:
    void initUi();
    void initFileLocal();
    void initTypes();
    void initCategories();
    void initContentEditeLine();
    void initContentWidgets();
    void initShowContentBox();
    //home/zinface/.local/share/applications
    QString s_userApplicationTargetDir = "~/.local/share/applications";
    QString s_dirsconfig = "user-dirs.dirs";
    QString s_homeDir = QDir::homePath();
    QString m_currentParam;
    QString m_currentDir;

    QLabel *fileFolderLabel;        // 文件存放路径提示
    QLabel *fileContentVersionLabel;     // 文件版本(默认1.0)
    QLabel *fileContentNameLabel;        // 显示的名称En
    QLabel *fileContentNameZhCnLabel;    // 显示的名称zh_CN
    QLabel *fileContentCommentLabel;     // 文件说明
    QLabel *fileContentTypeLabel;        // 文件类型
    QLabel *fileContentUrlLabel;         // Url链接
    QLabel *fileContentExecLabel;        // 可执行文件选择
    QLabel *fileContentIconLabel;        // 图标文件选择
    QLabel *fileContentCategoriesLabel;  // 程序分类
    QLabel *fileContentNoDisplay;        // 程序关键词
    // QLabel *fileContentNoDisplay;        // 程序标识关键词


    QLineEdit *contentVersion;
    QLineEdit *contentName;
    QLineEdit *contentNameZhCn;
    QLineEdit *contentComment;
    QLineEdit *contentUrl;
    QLineEdit *contentExec;
    QLineEdit *contentIcon;
    QLabel *contentIconPreview;

    QComboBox *fileFolderComb;      // 文件路径选择(可选增加)
    QPushButton *folderChoose;      // 文件位置选择
    QComboBox *execParamComb;
    QPushButton *execParamChoose;
    QPushButton *execFileChoose;
    QPushButton *iconFileChoose;

    QComboBox *contentIconComb;        // 图标选择
/**
//    JPEG 图像 jpg,jpeg,jpe
//    PNG 图像 png
//    SVG 图像 svg
//    Windows BMP 图像 bmp
//    BitMap 图像 xbm,icon,bitmap
//    PixMap 图像 xpm
*/
    QComboBox *contentTypeComb;        // 类型选择
/**
//    Application
//    Link
//    Directory
*/
    QComboBox *contentCategoriesComb;  // 分类选择
/**
//    AudioVideo
//    Audio
//    Video
//    Development
//    Education
//    Game
//    Graphics
//    Network
//    Office
//    Science
//    Settings
//    System
//    Utility
*/

    DesktopExtendedGroupBox *contentGroupBox;
    QTextEdit *contentText;
    QPushButton *saveAsFile;
    QPushButton *copyClipper;

};

#endif // DESKTOPGENERATER_H

/** e.g .desktop
[Desktop Entry]
Version=1.0
Name=自定义应用
GenericName=
Comment=这是自定义应用说明
Type
Exec
Url
Icon
StartupNofity
Terminal
Categories
OnlyShowIn
NotShowIn
Hidden
MimeType
NoDisplay
TryExec
Path
Terminal -- boolean
Actions
Keywords
*/

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
