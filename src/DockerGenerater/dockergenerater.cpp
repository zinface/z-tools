#include "choosedockercontentfrom.h"
#include "dockergenerater.h"

#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QStringListModel>
#include <QTextEdit>

DockerGenerater::DockerGenerater(QWidget *parent) : QWidget(parent)
{
    initUi();

    connect(folderChoose,&QPushButton::clicked, this, &DockerGenerater::onFolderChoose);
    connect(dockerActionChoose,&QPushButton::clicked, this, &DockerGenerater::onDockerActionChoose);

//    setFixedWidth(500);

//    setFixedHeight(300);
}

void DockerGenerater::onFolderChoose()
{

}

void DockerGenerater::onDockerActionChoose()
{
    QString dialogTitle;
    QString actionTitle;
    QString actionCommand;
    QString actionDescripton;
    QString placeholderText;
    ChooseDockerContentFrom Choose;
    switch (dockerActionComb->currentData().toInt()) {
    case FROM: actionTitle="FROM";dialogTitle="指定基于的镜像";actionCommand="FROM";actionDescripton=from;placeholderText="例如：nginx[:latest]";break;
    case RUN: actionTitle="RUN";dialogTitle="指定执行指令-并提交为中间镜像";actionCommand="RUN";actionDescripton=run;placeholderText="例如：echo \"正在构建基于nginx的容器\"";break;
    case CMD: actionTitle="CMD";dialogTitle="指定启动容器时执行的命令";actionCommand="CMD";actionDescripton=cmd;placeholderText="例如：echo \"正在启动基于nginx的容器\"";break;
    case ENV: actionTitle="ENV";dialogTitle="设置环境变量-该后续执行指令将在该环境下执行";actionCommand="ENV";actionDescripton=env;placeholderText="例如：NGINX_CONFIG=/etc/nginx/nginx.conf";break;
    case ADD: actionTitle="ADD";dialogTitle="指定要从当前目录复制到容器的目录或文件";actionCommand="ADD";actionDescripton=add;placeholderText="例如：web/index.html";break;
    case COPY: actionTitle="COPY";dialogTitle="指定要从当前目录复制到容器的目录或文件";actionCommand="COPY";actionDescripton=copy;placeholderText="例如：web/index.html";break;
    case USER: actionTitle="USER";dialogTitle="指定用户-该后续执行指令将在该用户下执行";actionCommand="USER";actionDescripton=user;placeholderText="例如：root";break;
    case EXPOSE: actionTitle="EXPOSE";dialogTitle="指定暴露端口-该端口将在使用-P参数运行时自动分配";actionCommand="EXPOSE";actionDescripton=expose;placeholderText="例如：80";break;
    case VOLUME: actionTitle="VOLUME";dialogTitle="指定匿名数据卷";actionCommand="VOLUME";actionDescripton=volume;placeholderText="例如：/data/";break;
    case WORKDIR: actionTitle="WORKDIR";dialogTitle="指定工作目录-该后续执行指令将在该目录下执行";actionCommand="WORKDIR";actionDescripton=workDir;placeholderText="例如：tmp";break;
    case ONBUILD: actionTitle="ONBUILD";dialogTitle="指定执行指令-该生成的镜像被作为基础镜像构建时操作指令";actionCommand="ONBUILD";actionDescripton=onbuild;placeholderText="例如：echo \"x被作为基础镜像构建\"";break;
    case ENTRYPOINT: actionTitle="ENTRYPOINT";dialogTitle="指定程序运行及定参";actionCommand="ENTRYPOINT";actionDescripton=entryPoint;placeholderText="例如：";break;
    case MAINTAINER: actionTitle="MANTAINER";dialogTitle="指定镜像维护者及其联系方式";actionCommand="MANTAINER";actionDescripton=maintainer;placeholderText="例如：docker development <dockerdev@docker.com>";break;
    }
    Choose.setShowAction(dialogTitle,actionTitle,placeholderText,actionCommand,actionDescripton);
    if(Choose.exec() != QDialog::Accepted) {
        return;
    }
    QString actionValue = Choose.GetActionValue();
    QStringList temp = ((QStringListModel*)(actionValueList->model()))->stringList();
    temp.append(QString(actionTitle+": "+actionValue));
    ((QStringListModel*)(actionValueList->model()))->setStringList(temp);
}

void DockerGenerater::initUi()
{
    initContentWidgets();
    initContentEditLine();
    initContentEditLineToolTip();
    initFileLocal();
    initDockerActionComb();

    QGridLayout *entryLayout = new QGridLayout;
    entryLayout->addWidget(dockerContentFromLabel,0,0,1,1);
    entryLayout->addWidget(dockerContentFrom,0,1,1,1);
    entryLayout->addWidget(dockerContentMaintainerLabel,1,0,1,1);
    entryLayout->addWidget(dockerContentMaintainer,1,1,1,1);
    entryLayout->addWidget(dockerContentVolumeLabel,2,0,1,1);
    entryLayout->addWidget(dockerContentVolume,2,1,1,1);
    entryLayout->addWidget(dockerContentAddLabel,3,0,1,1);
    entryLayout->addWidget(dockerContentAdd,3,1,1,1);
    entryLayout->addWidget(dockerContentCopyLabel,4,0,1,1);
    entryLayout->addWidget(dockerContentCopy,4,1,1,1);
    entryLayout->addWidget(dockerContentRunLabel,5,0,1,1);
    entryLayout->addWidget(dockerContentRun,5,1,1,1);
    entryLayout->addWidget(dockerContentEnvLabel,6,0,1,1);
    entryLayout->addWidget(dockerContentEnv,6,1,1,1);
    entryLayout->addWidget(dockerContentCmdLabel,7,0,1,1);
    entryLayout->addWidget(dockerContentCmd,7,1,1,1);
    entryLayout->addWidget(dockerContentExposeLabel,8,0,1,1);
    entryLayout->addWidget(dockerContentExpose,8,1,1,1);
    entryLayout->addWidget(dockerContentWorkDirLabel,9,0,1,1);
    entryLayout->addWidget(dockerContentWorkDir,9,1,1,1);


    QHBoxLayout *newActionLayout = new QHBoxLayout;
    newActionLayout->addWidget(dockerActionComb);
    newActionLayout->addWidget(dockerActionChoose);

    QVBoxLayout *fromActionPreview = new QVBoxLayout;
    actionValueList = new QListView;
    actionValueList->setMovement(QListView::Free);
    actionValueList->setModel(new QStringListModel(QStringList() << "FROM: nginx"
                                                            << "MAINTAINER: docker development <docker@docker.com>"));
    fromActionPreview->addWidget(actionValueList);
    fromActionPreview->addWidget(new QTextEdit);

    QVBoxLayout *mainFromLayout = new QVBoxLayout;
    mainFromLayout->addLayout(newActionLayout);
    mainFromLayout->addLayout(fromActionPreview);

    QGroupBox *dockerEntryFromBox = new QGroupBox("Docker File");
    dockerEntryFromBox->setLayout(mainFromLayout);
//    dockerEntryBox->setLayout(entryLayout);

//    QHBoxLayout *fileFolderLayout = new QHBoxLayout;
//    fileFolderLayout->addWidget(new QLabel("文件存放位置："));
//    fileFolderLayout->addWidget(fileFolderComb);
//    fileFolderLayout->setAlignment(fileFolderComb, Qt::AlignLeft);
//    fileFolderLayout->addWidget(folderChoose);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(dockerEntryFromBox);
//    mainLayout->addLayout(fileFolderLayout);
//    mainLayout->addStretch();
    setLayout(mainLayout);
}

void DockerGenerater::initContentWidgets()
{
    dockerContentFromLabel = new QLabel("FROM:");
    dockerContentMaintainerLabel = new QLabel("MAINTAINER:");
    dockerContentVolumeLabel = new QLabel("VOLUME:");
    dockerContentAddLabel = new QLabel("ADD:");
    dockerContentCopyLabel = new QLabel("COPY:");
    dockerContentRunLabel = new QLabel("RUN:");
    dockerContentEnvLabel = new QLabel("ENV:");
    dockerContentEntryPointLabel = new QLabel("ENTRYPOINT:");
    dockerContentCmdLabel = new QLabel("CMD:");
    dockerContentExposeLabel = new QLabel("EXPOSE:");
    dockerContentUserLabel = new QLabel("USER:");
    dockerContentWorkDirLabel = new QLabel("WorkDir:");
    dockerContentOnbuildLabel = new QLabel("ONBUILD:");
}

void DockerGenerater::initContentEditLine()
{
    dockerContentFrom = new QLineEdit;
    dockerContentFrom->setPlaceholderText("例如：nginx[:latest]");
    dockerContentMaintainer = new QLineEdit;
    dockerContentMaintainer->setPlaceholderText("例如：docker development <docker@docker.com>");
    dockerContentVolume = new QLineEdit;
    dockerContentVolume->setPlaceholderText("例如：/data/");
    dockerContentAdd = new QLineEdit;
    dockerContentAdd->setPlaceholderText("例如：web/index.html");
    dockerContentCopy = new QLineEdit;
    dockerContentCopy->setPlaceholderText("例如：web");
    dockerContentRun = new QLineEdit;
    dockerContentRun->setPlaceholderText("例如：echo \"正在构建基于nginx的容器\"");
    dockerContentEnv = new QLineEdit;
    dockerContentEnv->setPlaceholderText("例如：");
    dockerContentEntryPoint = new QLineEdit;
    dockerContentCmd = new QLineEdit;
    dockerContentCmd->setPlaceholderText("例如：");
    dockerContentExpose = new QLineEdit;
    dockerContentExpose->setPlaceholderText("");
    dockerContentUser = new QLineEdit;
    dockerContentWorkDir = new QLineEdit;
    dockerContentWorkDir->setPlaceholderText("");
    dockerContentOnbuild = new QLineEdit;
    dockerContentOnbuild->setPlaceholderText("");
}

void DockerGenerater::initContentEditLineToolTip()
{


    dockerContentFrom->setToolTip(from);
    dockerContentMaintainer->setToolTip(maintainer);
    dockerContentRun->setToolTip(run);
    dockerContentCmd->setToolTip(cmd);
    dockerContentExpose->setToolTip(expose);
    dockerContentEnv->setToolTip(env);
    dockerContentAdd->setToolTip(add);
    dockerContentCopy->setToolTip(copy);
    dockerContentEntryPoint->setToolTip(entryPoint);
    dockerContentVolume->setToolTip(volume);
    dockerContentUser->setToolTip(user);
    dockerContentWorkDir->setToolTip(workDir);
    dockerContentOnbuild->setToolTip(onbuild);
}

void DockerGenerater::initFileLocal()
{
    fileFolderComb = new QComboBox;

    folderChoose = new QPushButton("选择路径");
}

void DockerGenerater::initDockerActionComb()
{
    dockerActionComb = new QComboBox;
    dockerActionComb->addItem("FROM - 指定基于的镜像",FROM);
    dockerActionComb->addItem("RUN - 指定运行的指令",RUN);
    dockerActionComb->addItem("CMD - 基于已指定程序及定参后的变参",CMD);
    dockerActionComb->addItem("ENV - 设置环境变量",ENV);
    dockerActionComb->addItem("ADD - 指定要从当前目录复制到容器的目录或文件",ADD);
    dockerActionComb->addItem("COPY - 指定要从当前目录复制到容器的目录或文件",COPY);
    dockerActionComb->addItem("USER - 定执行后续命令的用户和用户组",USER);
    dockerActionComb->addItem("EXPOSE - 声明端口",EXPOSE);
    dockerActionComb->addItem("VOLUME - 指定匿名数据卷",VOLUME);
    dockerActionComb->addItem("WORKDIR - 指定工作目录",WORKDIR);
    dockerActionComb->addItem("ONBUILD - 指定作为基础镜像时用于创建新镜像时所执行的操作指令",ONBUILD);
    dockerActionComb->addItem("ENTRYPOINT - 指定程序运行及定参",ENTRYPOINT);
    dockerActionComb->addItem("MAINTINER - 指定镜像维护者及其联系方式",MAINTAINER);

    dockerActionChoose = new QPushButton("新建动作项");
}
