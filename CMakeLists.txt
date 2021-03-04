cmake_minimum_required(VERSION 3.0.0)

project(z-tools-adbmanager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

# Archive build control
set(BUILD_TYPE "${BUILD_TYPE}")
if(BUILD_TYPE STREQUAL "TGZ")
    include(cmake/BuildSourceTgz.cmake)
else()
    include(cmake/DebGenerater.cmake)
endif()

add_subdirectory(Components)

include_directories(
    src
    Components/AdbManager
)
set(QRC_SOURCES
    ""
)

add_executable(${PROJECT_NAME}
    "main.cpp"
    "src/mainwindow.h"
    "src/mainwindow.cpp"
    ${QRC_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    Qt5::Core Qt5::Widgets Qt5::Network

    AdbManager
)
