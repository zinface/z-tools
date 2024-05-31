cmake_minimum_required(VERSION 3.0.0)

project(z-tools-imagesbrowser VERSION 0.4.1 LANGUAGES CXX)

include(cmake/SparkWithGit.cmake)

spark_add_executable_path(${PROJECT_NAME}
    src
    src/*/*
    
    ${QRC_SOURCES}
)
target_link_qt5(${PROJECT_NAME})

spark_add_links_qt5(DBus)
target_link_qt5_dbus(${PROJECT_NAME})

spark_framework_include_with_git(${PROJECT_NAME}
    https://gitee.com/spark-framework-libs/spark-framework-features
    FEATURES eaters)

set(component "imagesbrowser")

spark_include(SparkDesktopMacros.cmake)
# 内容默认应用名称: Name= 应与项目名称相同
spark_desktop_macros(
    # 应用名称: Name=
    "ImageBrowser"
    # 应用名称: Name[zh_CN]=
    "图片浏览器"
    # 应用说明: Comment=
    "用来浏览指定目录下及子目录下所有 jpg, png, svg 图片"
    # 应用类型: Type=
    "Application"
    # 执行程序: Exec=
    "/opt/zTools/bin/z-tools-${component}"
    # 图标路径: Icon=
    "/opt/zTools/Components/${component}/resource/icon.png"
    # 应用分类: Categories=
    "Graphics"
    # MIME类型: MimeType=
)

spark_install_target(/opt/zTools/bin
    ${PROJECT_NAME})
spark_install_file(/opt/zTools/Components/${component}/resource/
    resources/icon.png)
spark_install_file(/usr/share/applications
    ${SPARK_DESKTOP_FILE})
add_package_descript(cmake/spark-deb-package.descript)
