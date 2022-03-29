cmake_minimum_required(VERSION 3.5)

set(MODULE_NAME "AdbUtils")
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)

add_library(${MODULE_NAME} "adbutils.h" "adbutils.cpp")

target_link_libraries(${MODULE_NAME} Qt5::Core)
