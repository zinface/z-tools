cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "AptCommands")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

include_directories(
    AptUtils
)

add_subdirectory(AptUtils)

add_executable(${MODULE_NAME}
    aptsendercommands.cpp aptsendercommands.h
    aptreceivercommands.cpp aptreceivercommands.h
    main.cpp
)

target_link_libraries(${MODULE_NAME} Qt5::Core Qt5::Widgets Qt5::Network PackageInfo AptUtils SessionManager InfomationManager)

# RemotePackageAction:
    # FetchPackageList
    # Name
    # Version
    # Arch
    # Installed
    # RemoteLocalPackageCount
    # RemoteMirrorsPackageCount
