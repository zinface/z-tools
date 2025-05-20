#!/usr/bin/env bash


# 参考：
    # https://www.cnblogs.com/chendeqiang/p/12861670.html
        # (QtDBus 快速入门 + qdbusxml2cpp(已使用) + qdbuscpp2xml(未使用))

    # https://www.codeleading.com/article/98315863646/  or https://blog.csdn.net/billxin2012/article/details/52224019
        # (QtDBus 复杂参数传递)

    # https://www.cnblogs.com/tudou1179006580/p/14875214.html
        # (当前 shell 脚本位置)

# qdbusxml2cpp - 生成适配代码
    # -a filename：输出 Adaptor 代码到 filename
    # -c classname：使用classname作为生成类的类名
    # -i filename：增加#include到输出
    # -l classname：当生成Adaptor代码时，使用 classname作为父类
    # -m：在cpp文件中包含 #include "filename.moc" 语句
    # -N：不使用名称空间
    # -p filename：生成Proxy代码到filename文件

# qdbuscpp2xml - 生成 xml 接口描述
    # ..



script_dir=$(cd $(dirname $0);pwd)
cd $script_dir

# 创建生成目录
mkdir -p gens
cd gens


# 转换开始，每个 -i 都是一个需要手动补充的 class 信息体(代表每个实体信息)
    # 也许我们可以像 https://github.com/openSUSE/imagewriter 项目一样
    # 直接用一个 MetaTypes.h 来写完所有类型
QT_SELECT=qt5 qdbusxml2cpp $script_dir/org.ayatana.bamf.matcher.xml -p matcher -c Matcher -i "MetaTypes.h"
QT_SELECT=qt5 qdbusxml2cpp $script_dir/org.ayatana.bamf.window.xml -p window -c Window -i "MetaTypes.h"
QT_SELECT=qt5 qdbusxml2cpp $script_dir/org.ayatana.bamf.view.xml -p view -c View -i "MetaTypes.h"
