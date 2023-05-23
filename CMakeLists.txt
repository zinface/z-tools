
cmake_minimum_required(VERSION 3.5.1)

project(z-tools-base-apk-view LANGUAGES CXX VERSION 1.1)

include(cmake/SparkEnvConfig.cmake)
include(cmake/SparkMacrosConfig.cmake)
include(cmake/SparkFindLibraries.cmake)
include(cmake/SparkFindQt5Config.cmake)
# include(cmake/SparkFindQt6Config.cmake)
include(cmake/SparkMacrosExtendConfig.cmake)
include(cmake/SparkInstallMacrosConfig.cmake)   # 提供了 spark_install 开头的 macro 宏用于安装 target、file、program、directory、changelog 等内容


# 资源文件路径
set(QRC_SOURCES "assets/sources.qrc")

# 基于传入的项进行构建
# 可接受的值为: 路径列表
# 可接受的值为: 路径列表+依赖库A+依赖库B
spark_add_library_realpaths(
    src/ApkManager/aapt
    src/ApkManager/utils
    src/ApkManager+aapt+utils
)

spark_add_executable_path(${PROJECT_NAME} src
    ${QRC_SOURCES}
)
target_link_ApkManager(${PROJECT_NAME})

# 构建 zToolsPlugin 扩展
add_subdirectory(src/zToolsPlugin)

# 安装主程序 z-tools-aptmanager
spark_install_target(/opt/zTools/bin
    ${PROJECT_NAME})

# 安装 icon 文件
spark_install_file(/opt/zTools/Components/BaseApkView/resources/icon
    ${CMAKE_SOURCE_DIR}/assets/z-Tools.svg)

# 生成 z-tools-aptmanager.desktop 文件
include(cmake/SparkDesktopMacros.cmake)
# 内容默认应用名称: Name= 应与项目名称相同
spark_desktop_macros(
    # 应用名称: Name=
    "${PROJECT_NAME}"
    # 应用名称: Name[zh_CN]=
    "APK信息查看器(aapt)"
    # 应用说明: Comment=
    "致力生产工具，简单明确需求"
    # 应用类型: Type=
    "Application"
    # 执行程序: Exec=
    "/opt/zTools/bin/z-tools-base-apk-view %F"
    # 图标路径: Icon=
    "/opt/zTools/Components/BaseApkView/resources/icon/z-Tools.svg"
    # 应用分类: Category=
    "Development"
    # MIME类型: MimeType=
    "application/vnd.android.package-archive"
)

# 安装 desktop 文件
spark_install_file(/usr/share/applications
    ${DESKTOP_FILE})

include(cmake/SparkBuildGraphviz.cmake)

# 注释行(使用方式)
find_package(DebPackage PATHS ${CMAKE_SOURCE_DIR})
add_package_descript(cmake/package-deb.descript)
