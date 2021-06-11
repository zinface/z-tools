cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "LinuxInfomation")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

add_library(CPU CPU/cpu.cpp CPU/cpu.h)
add_library(Memory Memory/memory.cpp Memory/memory.h)
add_library(Network Network/network.cpp Network/network.h)
add_library(Uptime Uptime/uptime.cpp Uptime/uptime.h)

set(INFO_LIBS CPU Memory Network Uptime)

add_library(${MODULE_NAME}
    linuxinfomation.cpp linuxinfomation.h)

foreach(lib IN LISTS INFO_LIBS)
    target_link_libraries(${lib} Qt5::Core)
endforeach()
target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Widgets Qt5::Network ${INFO_LIBS})


