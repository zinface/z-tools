cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "ServerScan")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

add_library(${MODULE_NAME} serverscan.cpp serverscan.h
    scanworkerthread.cpp scanworkerthread.h
)

target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Widgets Qt5::Network)



# 服务端口扫描器，该工具功能分为两个。
# 1. 服务扫描器，通过给出的 ip 地址前缀 扫描提供指定端口服务的 ip 地址列表。
# 2. 端口扫描器，通过给出的 ip 地址 扫描该服务提供的所有端口服务。


