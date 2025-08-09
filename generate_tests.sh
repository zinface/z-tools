#!/bin/bash

set -e

# 0. 进入当前脚本所在目录
script_dir=$(cd "$(dirname "$0")" && pwd)
cd "$script_dir" || exit 1

# 1. 检查命令行参数
testname=${1}
if [ -z "$testname" ]; then
  echo "Usage:  <testname>"
  exit 1
fi

# # 2. 扫描测试目录
testdir=
if [ -d "test" ]; then
    echo "directory \"test\" exists"
    testdir=test
elif [ -d "tests" ]; then
    echo "directory \"tests\" exists"
    testdir=tests
else
    echo "directory \"test\" or \"tests\" does not exist"
    exit 1
fi

# 2. 检查测试目录是否存在
folder="$testdir/$testname"

echo "Generating test $folder..."
if [[ -d "$folder" && -f "$folder/main.cpp" ]]; then
    echo "directory \"$folder\" exists"
    echo "directory \"$folder/main.cpp\" exists"
    exit 0
fi

# 3. 创建测试目录
mkdir -p "$folder"

cat > "$folder/main.cpp" << EOF
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

sed -i "/^spark_add_executable_paths(test/a\ \ \ \ $testname" $testdir/CMakeLists.txt

echo "Generated test $folder"

