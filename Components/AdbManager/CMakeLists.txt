cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "AdbManager")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_subdirectory(AdbUtils)

add_library(${MODULE_NAME}
    "adbmanager.cpp" "adbmanager.h"
    "adbpkgview.cpp" "adbpkgview.h")

target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Widgets AdbUtils)
