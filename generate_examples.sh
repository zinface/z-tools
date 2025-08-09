#!/bin/bash

set -e

# 0. 进入当前脚本所在目录
script_dir=$(cd "$(dirname "$0")" && pwd)
cd "$script_dir" || exit 1

# 1. 检查命令行参数
name=${1}
if [ -z "$name" ]; then
    echo "Usage:  <example name>"
    exit 1
fi

# 2. 扫描用例目录
exampledir=
if [ -d "example" ]; then
    echo "directory \"example\" exists"
    exampledir=example
elif [ -d "examples" ]; then
    echo "directory \"examples\" exists"
    exampledir=examples
elif [ -d "src/examples" ]; then
    echo "directory \"src/examples\" exists"
    exampledir=src/examples
else
    echo "directory \"test\" or \"tests\" does not exist"
    exit 1
fi

# 3. 检查测试目录是否存在
folder="$exampledir/$name"

echo "Generating test $folder..."
if [[ -d "$folder" && -f "$folder/$name.cpp" ]]; then
    echo "directory \"$folder\" exists"
    echo "directory \"$folder/main.cpp\" exists"
    exit 0
fi

# 4. 创建测试目录
mkdir -p "$folder"

# 5. 创建测试文件 Gui or Console
read -p "Do you want to create a GUI example? (y/n) " -n 1 -r REPLY
echo    # (optional) move to a new line
if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Creating GUI example..."
    # 
    read -p "Enter the class prefix name (default: MainWindow): " class_prefix
    class_prefix=${class_prefix:-MainWindow}
    # 如果输入不是MainWindow结尾，则添加MainWindow
    if [[ ! $class_prefix =~ MainWindow$ ]]; then
        class_prefix="${class_prefix}MainWindow"
    fi
    class_prefix_upper=$(echo "$class_prefix" | tr '[:lower:]' '[:upper:]')
    class_prefix_lower=$(echo "$class_prefix" | tr '[:upper:]' '[:lower:]')
    # 1.create mainwindow.h
    cat > "$folder/${class_prefix_lower}.h" << EOF
#ifndef ${class_prefix_upper}_H
#define ${class_prefix_upper}_H

#include <QMainWindow>

namespace Ui {
class ${class_prefix};
}

class ${class_prefix} : public QMainWindow
{
    Q_OBJECT

public:
    explicit ${class_prefix}(QWidget *parent = nullptr);
    ~${class_prefix}();

private:
    Ui::${class_prefix} *ui;
};

#endif // ${class_prefix_upper}_H
EOF
    # 2.create mainwindow.cpp
    cat > "$folder/${class_prefix_lower}.cpp" << EOF
#include "${class_prefix_lower}.h"
#include "ui_${class_prefix_lower}.h"

${class_prefix}::${class_prefix}(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::${class_prefix})
{
    ui->setupUi(this);
}

${class_prefix}::~${class_prefix}()
{
    delete ui;
}
EOF
    # 3.create mainwindow.ui
    cat > "$folder/${class_prefix_lower}.ui" << EOF
<ui version="4.0">
 <author/>
 <comment/>
 <exportmacro/>
 <class>${class_prefix}</class>
 <widget name="${class_prefix}" class="QMainWindow">
  <property name="geometry">
   <rect>
    <x>0</x>
    <y>0</y>
    <width>1024</width>
    <height>600</height>
   </rect>
  </property>
  <property name="windowTitle">
   <string>MainWindow</string>
  </property>
  <widget name="menubar" class="QMenuBar"/>
  <widget name="centralwidget" class="QWidget"/>
  <widget name="statusbar" class="QStatusBar"/>
 </widget>
 <pixmapfunction/>
 <connections/>
</ui>
EOF
    # 4.create main.cpp
    cat > "$folder/$name.cpp" << EOF
#include <QApplication>

#include "${class_prefix_lower}.h"

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QApplication a(argc, argv);

    a.setApplicationDisplayName("$name");

    ${class_prefix} w;
    w.show();

    return a.exec();
}
EOF
    # 将测试添加到 dev.cmake
    cat >> "$exampledir/dev.cmake" << EOF
# find $exampledir/$name | grep -E '.h$|.cpp$|.ui$' | sort
add_executable(example-$name
    $exampledir/$name/$name.cpp
    $exampledir/$name/${class_prefix_lower}.h
    $exampledir/$name/${class_prefix_lower}.cpp
    $exampledir/$name/${class_prefix_lower}.ui
)
target_link_libraries(example-$name Qt\${QT_VERSION_MAJOR}::Widgets)

EOF

else
    echo "Creating console example..."
    cat > "$folder/$name.cpp" << EOF
#include <QApplication>
#include <QDateTime>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString createdAt = "2025-05-23T00:00:00+00:00";
    // auto dt = QDateTime::fromString(createdAt, "yyyy-MM-ddTHH:mm:ss+zzz");
    auto dt = QDateTime::fromString(createdAt, Qt::ISODateWithMs);
    qDebug() << dt;

    return 0;
}
EOF
    # 将测试添加到 dev.cmake
    cat >> "$exampledir/dev.cmake" << EOF
# find $exampledir/$name | grep -E '.h$|.cpp$|.ui$' | sort
add_executable(example-$name
    $exampledir/$name/$name.cpp
)
target_link_libraries(example-$name Qt::Core)

EOF
fi

echo "Generated example $folder"
