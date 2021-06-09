cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "FileCommands")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(PkgConfig)

add_executable(${MODULE_NAME}
    main.cpp
    filesendercommand.cpp filesendercommand.h
    filereceivercommand.cpp filereceivercommand.h)

target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Network FileSessionManager ScanPort)
