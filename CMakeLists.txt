
cmake_minimum_required(VERSION 3.5.1)

project(z-tools-aptmanager LANGUAGES CXX VERSION 0.7.4)

include(cmake/SparkEnvConfig.cmake)
include(cmake/SparkMacrosConfig.cmake)
include(cmake/SparkFindLibraries.cmake)
include(cmake/SparkFindQt5Config.cmake)
# include(cmake/SparkFindQt6Config.cmake)
include(cmake/SparkMacrosExtendConfig.cmake)
include(cmake/SparkInstallMacrosConfig.cmake)   # 提供了 spark_install 开头的 macro 宏用于安装 target、file、program、directory、changelog 等内容


# 资源文件路径
set(QRC_SOURCES "")

spark_find_library(QApt libqapt)

# 基于传入的项进行构建
# 可接受的值为: 路径列表
# 可接受的值为: 路径列表+依赖库A+依赖库B
spark_add_library_realpaths(
    src/AptManager/AptUtils
    src/AptManager+AptUtils
)

target_link_QApt(AptUtils)

spark_add_executable_path(${PROJECT_NAME} src
    ${QRC_SOURCES}
)
target_link_AptManager(${PROJECT_NAME})

# 安装主程序 z-tools-aptmanager
spark_install_target(/opt/zTools/bin
    ${PROJECT_NAME})

# 安装 icon 文件
spark_install_file(/opt/zTools/Components/APTManager/resources/icon
    ${CMAKE_SOURCE_DIR}/assets/z-Tools.svg)

# 生成 z-tools-aptmanager.desktop 文件
include(cmake/SparkDesktopMacros.cmake)
# 内容默认应用名称: Name= 应与项目名称相同
spark_desktop_macros(
    "${PROJECT_NAME}"
    "软件包查看器(apt)"
    "致力生产工具，简单明确需求"
    "Application"
    "/opt/zTools/bin/z-tools-aptmanager"
    "/opt/zTools/Components/APTManager/resources/icon/z-Tools.svg"
    "Development"
)

# 安装 desktop 文件
spark_install_file(/usr/share/applications
    ${DESKTOP_FILE})

include(cmake/SparkBuildGraphviz.cmake)

# 注释行(使用方式)
find_package(DebPackage PATHS ${CMAKE_SOURCE_DIR})
add_package_descript(cmake/package-deb.descript)