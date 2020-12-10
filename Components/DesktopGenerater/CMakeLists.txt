cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "DesktopGenerater")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(QApt REQUIRED)

add_library(${MODULE_NAME}
    "desktopgenerater.cpp" "desktopgenerater.h"
    "desktopexecparamdialog.cpp" "desktopexecparamdialog.h"
    "desktopcustomecontent.cpp" "desktopcustomecontent.h"
    "desktopextendedgroupbox.cpp" "desktopextendedgroupbox.h")

target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Widgets Qt5::Network)
