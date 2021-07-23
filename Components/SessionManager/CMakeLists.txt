cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "SessionManager")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)


add_library(${MODULE_NAME}
    sessionmanager.cpp sessionmanager.h
)

target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Widgets Qt5::Network)
