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
    ScanPort
    FileReceiver
    FileSessionManager
    FileTaskManager
)

add_subdirectory(ScanPort)
add_subdirectory(FileSender)
add_subdirectory(FileReceiver)
add_subdirectory(FileSessionManager)
add_subdirectory(FileTaskManager)

add_library(${MODULE_NAME}
    filetransfersender.cpp filetransfersender.h
    filetransferreceiver.cpp filetransferreceiver.h
    filetransferrecivermanualhostdialog.cpp filetransferrecivermanualhostdialog.h
)

target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Widgets Qt5::Network FileSender ScanPort FileReceiver FileSessionManager)
