#ifndef DOCKERGENERATER_H
#define DOCKERGENERATER_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QComboBox;
class QPushButton;
class QListView;

class DockerGenerater : public QWidget
{
    Q_OBJECT
public:
    explicit DockerGenerater(QWidget *parent = nullptr);


private slots:
    void onFolderChoose();
    void onDockerActionChoose();

private:
    void initUi();
    void initContentWidgets();
    void initContentEditLine();
    void initContentEditLineToolTip();
    void initFileLocal();
    void initDockerActionComb();

    QLabel *dockerContentFromLabel;         // 指定基于的镜像(基础镜像)
    QLabel *dockerContentMaintainerLabel;   // 指定镜像维护者及其联系方式
    QLabel *dockerContentVolumeLabel;       // 指定匿名数据卷
    QLabel *dockerContentAddLabel;          // 指定要从当前目录复制到容器的目录或文件
    QLabel *dockerContentCopyLabel;         // 指定要从当前目录复制到容器的目录或文件
    QLabel *dockerContentRunLabel;          // 指定运行的指令
    QLabel *dockerContentEnvLabel;          // 设置环境变量
    QLabel *dockerContentEntryPointLabel;   // 指定程序运行及定参
    QLabel *dockerContentCmdLabel;          // 基于已指定程序及定参后的变参
    QLabel *dockerContentExposeLabel;       // 声明端口
    QLabel *dockerContentUserLabel;         // 指定用户
    QLabel *dockerContentWorkDirLabel;      // 指定工作目录
    QLabel *dockerContentOnbuildLabel;      // 指定作为基础镜像时用于创建新镜像时所执行的操作指令

    QLineEdit *dockerContentFrom;           //
    QLineEdit *dockerContentMaintainer;     //
    QLineEdit *dockerContentVolume;         //
    QLineEdit *dockerContentAdd;            //
    QLineEdit *dockerContentCopy;           //
    QLineEdit *dockerContentRun;            //
    QLineEdit *dockerContentEnv;            //
    QLineEdit *dockerContentEntryPoint;     //
    QLineEdit *dockerContentCmd;
    QLineEdit *dockerContentExpose;
    QLineEdit *dockerContentUser;
    QLineEdit *dockerContentWorkDir;
    QLineEdit *dockerContentOnbuild;

    QComboBox *fileFolderComb;      // 文件路径选择(可选增加)
    QPushButton *folderChoose;      // 文件位置选择


    QComboBox *dockerActionComb;
    QPushButton *dockerActionChoose;


    QListView *actionValueList;


private:
    enum DockerActions {
        FROM,
        RUN,
        CMD,
        ENV,
        ADD,
        COPY,
        USER,
        EXPOSE,
        VOLUME,
        WORKDIR,
        ONBUILD,
        ENTRYPOINT,
        MAINTAINER,
    };

    QString from = "FROM: \n"
                   "格式：FROM <image>[:<tag>]\n"
                   "指定将要定制的镜像，例如 nginx 是定制需要的基础镜像，\n"
                   "后续的操作都将基于 nginx。\n"
                   "FROM nginx 或 FROM nginx:latest\n"
                   "\n"
                   "第一条指令必须为 FROM 指令。\n"
                   "并且，如果在同一个Dockerfile中创建多个镜像时，可以使用多个 FROM 指令（每个镜像一次）。";

    QString maintainer = "MANTAINER:\n"
                         "格式：MANTAINER <name> 指定维护者信息\n"
                         "指定镜像维护者及其联系方式,一般以邮箱为主\n"
                         "例如：docker development <dockerdev@docker.com>";

    QString run = "RUN:\n"
                  "格式：RUN <command>\n"
                  "     RUN [\"executable\",\"param1\",\"param2\",...]\n"
                  "前者将在 shell 终端中运行命令，即 /bin/sh -c；后者则使用 exec 执行。\n"
                  "指定使用其它终端可以通过第二种方式实现，例如 RUN [\"/bin/bash\", \"-c\", \"echo hello\"]。\n"
                  "每条 RUN 指令将在当前镜像基础上执行指定命令，并提交为新的镜像。当命令较长时可以使用 \\ 来换行。\n";

    QString cmd = "CMD:\n"
                  "格式：CMD [\"executable\",\"param1\",\"param2\"] (exec形式，这是首选形式)\n"
                  "     CMD command param1 param2 (在 /bin/sh 中执行，提供给需要交互的应用)\n"
                  "     CMD [\"param1\",\"param2\"] (作为ENTRYPOINT的默认参数)\n"
                  "指定启动容器时执行的命令，每个 Dockerfile 只能有一条 CMD 命令。\n"
                  "如果指定了多条命令，只有最后一条会被执行。\n"
                  "如果用户启动容器时候指定了运行的命令，则会覆盖掉 CMD 指定的命令。";

    QString env = "ENV:\n"
                  "格式：ENV <key> <value>\n"
                  "指定一个环境变量，会被后续 RUN 指令使用，并在容器运行时保持。";

    QString add = "ADD:\n"
                  "格式：ADD <src> <dest>\n"
                  "复制指定的 <src> 到容器中的 <dest>。 \n"
                  "其中 <src> 可以是Dockerfile所在目录的一个相对路径；也可以是一个 URL；还可以是一个 tar 文件（自动解压为目录）。";

    QString copy = "COPY:\n"
                   "格式：COPY<src> <dest>\n"
                   "复制本地主机的 <src> 到容器中的 <dest>\n"
                   "<src>（为 Dockerfile 所在目录的相对路径），当使用本地目录为源目录时，推荐使用 COPY。";

    QString volume = "VOLUME:\n"
                     "格式: VOLUME [\"/data\"]。\n"
                     "创建一个可以从本地主机或其他容器挂载的挂载点，一般用来存放数据库和需要保持的数据等。";

    QString entryPoint = "ENTRYPOINT:\n"
                         "格式：ENTRYPOINT [\"executable\",\"param1\",\"param2\",...]\n"
                         "     ENTRYPOINT command param1 param2 ... \n"
                         "配置容器启动后执行的命令，并且不可被 docker run 提供的参数覆盖。\n"
                         "每个 Dockerfile 中只能有一个 ENTRYPOINT，当指定多个时，只有最后一个起效。";

    QString expose = "EXPOSE:\n"
                     "格式：EXPOSE <port> [<port>/<protocol>...]\n"
                     "告诉 Docker 服务端暴露容器运行时的监听端口给外部。\n"
                     "可指定端口是监听 TCP 或 UDP，如果未指定协议，则默认为 TCP。\n"
                     "在启动容器时使用 -P 参数，Docker 主机将会自动分配一个端口转发到指定的端口";

    QString user = "USER:\n"
                   "格式：USER <user>[:<group>]\n"
                   "     USER <UID>[:<GID>]\n"
                   "";

    QString workDir = "WORKDIR:\n"
                      "格式：WORKDIR /path/to/workdir\n"
                      "为后续的 RUN、CMD、ENTRYPOINT 指令配置工作目录\n"
                      "可以使用多个 WORKDIR 指令，后续命令如果参数是相对路径，则会基于之前命令指定的路径。\n"
                      "例如: \n"
                      "    WORKDIR /a\n"
                      "    WORKDIR b\n"
                      "    WORKDIR c\n"
                      "        则最终路径为 /a/b/c";

    QString onbuild = "ONBUILD:\n"
                      "格式：ONBUILD <其它指令>\n"
                      "配置当所创建的镜像作为其它新创建镜像的基本镜像时，所执行的操作指令";

};

#endif // DOCKERGENERATER_H


/******************************************************************

DockerFile:
  is *.dockerfile
\ Dockerfile 是一个用来构建镜像的文本文件，文件内容包含了一条条构建镜像所需的指令和说明
e.g 定制一个 nginx 镜像(构建好的镜像内会有一个 /usr/share/nginx/html/index.html 文件

nginx:
FROM nginx
RUN echo '这是一个本地构建的nginx镜像' > /usr/share/nginx/html/index.html
RUN: build时执行



content struct:



example:
# Docker image of enterprise-manager-web
# VERSION 0.0.1
# Author: Jiang Xin

FROM java:8
MAINTAINER xxxx <xxxxx@tech-oled.com>
VOLUME /tmp
ADD enterprise-manager-web-0.0.1-SNAPSHOT.jar app.jar
RUN sh -c 'touch /app.jar'
ENV JAVA_OPTS=""
ENTRYPOINT ["java","-Djava.security.egd=file:/dev/./urandom","-jar","/app.jar"]





******************************************************************/
