cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "RemoteAptManager")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

add_library(PackageInfo
    packageinfo.cpp packageinfo.h
)
target_link_libraries(PackageInfo Qt5::Core Qt5::Widgets Qt5::Network)

if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
    add_subdirectory(AptCommands)
endif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Linux")
