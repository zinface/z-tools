cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "InfoCommands")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

add_executable(${MODULE_NAME}
    main.cpp
    infosendercommands.cpp infosendercommands.h
    inforecivercommands.cpp inforecivercommands.h
#    mainwindow.cpp mainwindow.h
)

target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Widgets LinuxInfomation InfomationManager)
