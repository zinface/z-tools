cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "SpeedWidget")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

add_subdirectory(infoUtils)

add_library(${MODULE_NAME} speedwidget.cpp speedwidget.h)

target_link_libraries(${MODULE_NAME}
    Qt5::Core Qt5::Widgets Qt5::Network

    infoUtils
)
