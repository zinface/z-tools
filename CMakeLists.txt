
cmake_minimum_required(VERSION 3.5.1)

project(z-tools-scanport LANGUAGES CXX VERSION 0.1.2)

include(cmake/SparkEnvConfig.cmake)
include(cmake/SparkMacrosConfig.cmake)
include(cmake/SparkFindLibraries.cmake)
include(cmake/SparkFindQt5Config.cmake)
# include(cmake/SparkFindQt6Config.cmake)
include(cmake/SparkMacrosExtendConfig.cmake)
include(cmake/SparkInstallMacrosConfig.cmake)   # 提供了 spark_install 开头的 macro 宏用于安装 target、file、program、directory、changelog 等内容


# 资源文件路径
set(QRC_SOURCES "")

# 基于传入的项进行构建
# 可接受的值为: 路径列表
# 可接受的值为: 路径列表+依赖库A+依赖库B
spark_add_library_realpaths(
    src/ServerScan      # 服务端口扫描器，该工具功能分为两个。
                        # 1. 服务扫描器，通过给出的 ip 地址前缀 扫描提供指定端口服务的 ip 地址列表。
                        # 2. 端口扫描器，通过给出的 ip 地址 扫描该服务提供的所有端口服务。
)

spark_add_executable_path(${PROJECT_NAME} src
    ${QRC_SOURCES}
)
target_link_ServerScan(${PROJECT_NAME})

# 安装主程序 z-tools-aptmanager
spark_install_target(/opt/zTools/bin
    ${PROJECT_NAME})

# 安装 icon 文件
spark_install_file(/opt/zTools/Components/ServerScan/resources/icon
    ${CMAKE_SOURCE_DIR}/assets/z-Tools.svg)

# 生成 z-tools-aptmanager.desktop 文件
include(cmake/SparkDesktopMacros.cmake)
# 内容默认应用名称: Name= 应与项目名称相同
spark_desktop_macros(
    # 应用名称: Name=
    "${PROJECT_NAME}"
    # 应用名称: Name[zh_CN]=
    "端口扫描(tcp)"
    # 应用说明: Comment=
    "致力生产工具，简单明确需求"
    # 应用类型: Type=
    "Application"
    # 执行程序: Exec=
    "/opt/zTools/bin/z-tools-scanport"
    # 图标路径: Icon=
    "/opt/zTools/Components/ServerScan/resources/icon/z-Tools.svg"
    # 应用分类: Category=
    "Development"
)

# 安装 desktop 文件
spark_install_file(/usr/share/applications
    ${DESKTOP_FILE})

include(cmake/SparkBuildGraphviz.cmake)

# 注释行(使用方式)
find_package(DebPackage PATHS ${CMAKE_SOURCE_DIR})
add_package_descript(cmake/package-deb.descript)