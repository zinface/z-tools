cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "FileTransferManager")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(PkgConfig)

add_library(FileItemInfo
    fileiteminfo.cpp fileiteminfo.h
)

target_link_libraries(FileItemInfo Qt5::Core Qt5::Widgets Qt5::Network)

include_directories(
    FileSender
)

add_subdirectory(FileSender)

add_library(${MODULE_NAME}
    filetransfermanager.cpp filetransfermanager.h
    filetransfersender.cpp filetransfersender.h
    filetransferreceiver.cpp filetransferreceiver.h
    fileiteminfo.cpp fileiteminfo.h
)

target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Widgets Qt5::Network FileSender)
