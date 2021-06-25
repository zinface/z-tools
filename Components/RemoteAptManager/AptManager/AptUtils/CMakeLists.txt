cmake_minimum_required(VERSION 3.5)

set(MODULE_NAME "AptUtils")
set(CMAKE_AUTOMOC ON)

find_package(Qt5Core REQUIRED)


add_library(${MODULE_NAME} "aptutils.h" "aptutils.cpp")

target_link_libraries(${MODULE_NAME} Qt5::Core)

