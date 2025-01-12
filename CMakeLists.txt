
cmake_minimum_required(VERSION 3.5.1)

project(z-tools-aptmanager LANGUAGES CXX VERSION 0.7.10)

include(cmake/SparkWithGit.cmake)

# 对模板文件进行填充，并生成填充的文件
    # 此部分内容为填充项目的的版本信息
configure_file(cmake/config.h.in 
    ${CMAKE_BINARY_DIR}/config.h)

# 资源文件路径
set(QRC_SOURCES "")

spark_find_library(QApt libqapt)

spark_add_executable_source(${PROJECT_NAME}
    ${QRC_SOURCES}
)

# 基于传入的项进行构建
# 可接受的值为: 路径列表
# 可接受的值为: 路径列表+依赖库A+依赖库B
spark_add_executable_path(${PROJECT_NAME}
    src
    # src/manager
    # src/utils
    # src/view
    # src/view/model
    # src/view/delegate
    src/*/*/*
)
target_link_QApt(${PROJECT_NAME})
target_link_qt(${PROJECT_NAME})

# 安装主程序 z-tools-aptmanager
spark_install_target(/opt/zTools/bin
    ${PROJECT_NAME})

# 安装 icon 文件
spark_install_file(/opt/zTools/Components/APTManager/resources/icon
    ${CMAKE_SOURCE_DIR}/assets/z-Tools.svg)

# 生成 z-tools-aptmanager.desktop 文件
spark_include(SparkDesktopMacros.cmake)
# 内容默认应用名称: Name= 应与项目名称相同
spark_desktop_macros(
    # 应用名称: Name=
    "${PROJECT_NAME}"
    # 应用名称: Name[zh_CN]=
    "软件包查看器(apt)"
    # 应用说明: Comment=
    "致力生产工具，简单明确需求"
    # 应用类型: Type=
    "Application"
    # 执行程序: Exec=
    "/opt/zTools/bin/z-tools-aptmanager"
    # 图标路径: Icon=
    "/opt/zTools/Components/APTManager/resources/icon/z-Tools.svg"
    # 应用分类: Categories=
    "Development"
    # MIME类型: MimeType=
)

# 安装 desktop 文件
spark_install_file(/usr/share/applications
    ${SPARK_DESKTOP_FILE})

# 注释行(使用方式)
spark_include(SparkDebPackageConfig.cmake)
add_package_descript(cmake/spark-deb-package.descript)
