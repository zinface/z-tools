
cmake_minimum_required(VERSION 3.5.1)

project(z-tools-base-apk-view LANGUAGES CXX VERSION 1.2.0)

include(cmake/SparkWithGit.cmake)
spark_initialize_modules()

# 控制 qDebug 行为
if(CMAKE_BUILD_TYPE MATCHES "Release")
    add_compile_definitions(QT_NO_WARNING_OUTPUT=1 QT_NO_DEBUG_OUTPUT=1)
else()
    # 在开发过程中使用 examples 进行测试时的示例数据目录
    add_compile_definitions(
        EXAMPLE_DATA_DIRECTORY="${CMAKE_SOURCE_DIR}/examples/data/"
        DEBUG_APK_FILE="${CMAKE_SOURCE_DIR}/assets/apks/X-plore.apk"
        DEBUG_LINUX_AAPT="${CMAKE_SOURCE_DIR}/assets/tools/build-tools/35.0.0/aapt")
endif(CMAKE_BUILD_TYPE MATCHES "Release")

# 资源文件路径
set(QRC_SOURCES "assets/sources.qrc")

# 基于传入的项进行构建
# 可接受的值为: 路径列表
# 可接受的值为: 路径列表+依赖库A+依赖库B
#spark_add_library_realpaths(
#    src/config
#    src/ApkManager/aapt+config
#    src/ApkManager/utils
#    src/ApkManager+aapt+utils
#)

spark_add_executable_path(${PROJECT_NAME}
    src
    src/config
    src/ApkManager
    src/ApkManager/aapt
    src/ApkManager/utils
    ${QRC_SOURCES}
)
#target_link_ApkManager(${PROJECT_NAME})
target_link_qt(${PROJECT_NAME})
spark_add_links_qt(Concurrent Svg)
target_link_qt_concurrent(${PROJECT_NAME})
target_link_qt_svg(${PROJECT_NAME})

spark_framework_include_with_git(${PROJECT_NAME}
    https://gitee.com/spark-framework-libs/spark-framework-features
    FEATURES screenutil version baseutil dbusutil)

spark_framework_include_with_git(${PROJECT_NAME}
    https://gitee.com/spark-framework-libs/z-tools-libs)

#spark_framework_include(${PROJECT_NAME}
#    /home/zinface/Desktop/repo.z-tools/z-tools-adbmanager/spark.cmake)

# 构建 zToolsPlugin 扩展
add_subdirectory(src/zToolsPlugin)

# 示例集合
option(ENABLE_BUILD_EXAMPLES "Enable Build Examples." OFF)
if(ENABLE_BUILD_EXAMPLES)
    include(examples/dev.cmake)
endif(ENABLE_BUILD_EXAMPLES)

# 安装主程序 z-tools-aptmanager
spark_install_target(/opt/zTools/bin
    ${PROJECT_NAME})

# 安装依赖工具 adb/aapt
spark_install_directory(/opt/zTools/Components/BaseApkView/assets
    assets/tools
    assets/apks

    USE_SOURCE_PERMISSIONS)

# 安装 icon 文件
spark_install_file(/opt/zTools/Components/BaseApkView/resources/icon
    ${CMAKE_SOURCE_DIR}/assets/z-Tools.svg)

# 生成 z-tools-aptmanager.desktop 文件
spark_include(cmake/SparkDesktopMacros.cmake)
# 内容默认应用名称: Name= 应与项目名称相同
spark_desktop_macros(
    # 应用名称: Name=
    "${PROJECT_NAME}"
    # 应用名称: Name[zh_CN]=
    "APK信息查看器(apk)"
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
    "application/vnd.android.package-archive;application/x-troff-man"
)

# 安装 desktop 文件
spark_install_file(/usr/share/applications
    ${SPARK_DESKTOP_FILE})

spark_include(cmake/SparkBuildGraphviz.cmake)

# 注释行(使用方式)
spark_include(SparkDebPackageConfig.cmake)
add_package_descript(cmake/spark-deb-package.descript)


enable_testing()
add_test(NAME xplore COMMAND ${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/assets/apks/X-plore.apk WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
# 添加一个名为 xplore 的测试(CTest,CMake内置功能)，在构建目录(BINARY_DIR)中执行可执行文件并传入一个包含在源代码目录下的apk文件路径作为参数
