cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "RemoteInfomation")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

include_directories(
    LinuxInfomation/
    LinuxInfomation/CPU/
    LinuxInfomation/Memory/
    LinuxInfomation/Network/
    LinuxInfomation/Uptime/
    InfomationManager
)

add_subdirectory(LinuxInfomation)
add_subdirectory(InfomationManager)
add_subdirectory(InfoCommands)
add_subdirectory(InfoWindow)

add_library(${MODULE_NAME}
    remoteinfomation.cpp remoteinfomation.h)

target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Widgets Qt5::Network LinuxInfomation)
