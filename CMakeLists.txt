
cmake_minimum_required(VERSION 3.5.1)

project(z-tools-desktop-generater LANGUAGES CXX VERSION 1.2.3)

include(cmake/SparkWithGit.cmake)
spark_initialize_modules()

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS DBus REQUIRED)

# 资源文件路径
set(QRC_SOURCES "src/DesktopGenerater/desktop.qrc")

# 基于传入的项进行构建
# 可接受的值为: 路径列表
# 可接受的值为: 路径列表+依赖库A+依赖库B
spark_add_library_realpaths(
    intros/gens
    src/DesktopGenerater/dialog+gens
    src/DesktopGenerater+dialog
)
target_link_libraries(gens
    Qt${QT_VERSION_MAJOR}::DBus
)

spark_add_executable_path(${PROJECT_NAME}
    src
    ${QRC_SOURCES}
)
target_link_DesktopGenerater(${PROJECT_NAME})
target_link_gens(${PROJECT_NAME})

# 部分测试
add_subdirectory(tests)

# 安装主程序 z-tools-aptmanager
spark_install_target(/opt/zTools/bin
    ${PROJECT_NAME})

# 安装 icon 文件
spark_install_file(/opt/zTools/Components/DesktopGenerater/resources/icon
    ${CMAKE_SOURCE_DIR}/assets/z-Tools.svg)

# 生成 z-tools-aptmanager.desktop 文件
spark_include(cmake/SparkDesktopMacros.cmake)
# 内容默认应用名称: Name= 应与项目名称相同
spark_desktop_macros(
    # 应用名称: Name=    
    "${PROJECT_NAME}"
    # 应用名称: Name[zh_CN]=
    "桌面图标生成器(desktop)"
    # 应用说明: Comment=
    "致力生产工具，简单明确需求"
    # 应用类型: Type=
    "Application"
    # 执行程序: Exec=
    "/opt/zTools/bin/z-tools-desktop-generater"
    # 图标路径: Icon=
    "/opt/zTools/Components/DesktopGenerater/resources/icon/z-Tools.svg"
    # 应用分类: Category=
    "Development"
)

# 安装 desktop 文件
spark_install_file(/usr/share/applications
    ${DESKTOP_FILE}
    utils/open-system-applications.desktop
    utils/open-user-applications.desktop)

 # 生成依赖关系图
spark_include(cmake/SparkBuildGraphviz.cmake)

# 注释行(使用方式)
find_package(SparkDebPackage PATHS ${CMAKE_SOURCE_DIR})
add_package_descript(cmake/spark-deb-package.descript)
