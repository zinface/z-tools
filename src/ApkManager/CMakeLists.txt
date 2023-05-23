cmake_minimum_required(VERSION 3.0.0)

set(MODULE_NAME "ApkManager")

find_package(Qt5Core)
find_package(Qt5Widgets)
find_package(Qt5Network)

# add_subdirectory(aapt)

add_library(aapt
    aapt/Aapt.h
    aapt/Aapt.cpp
)

target_link_libraries(aapt Qt5::Core Qt5::Widgets Qt5::Network)

# add_subdirectory(utils)

add_library(uengine
    utils/UEngine.h
    utils/UEngine.cpp
)

target_link_libraries(uengine Qt5::Core Qt5::Widgets Qt5::Network)

add_library(${MODULE_NAME} 
    apkmanager.cpp apkmanager.h 
    apkinfopage.cpp apkinfopage.h
    apkinstallpage.cpp apkinstallpage.h
    splashwindow.cpp splashwindow.h
)

target_link_libraries(${MODULE_NAME} aapt uengine)
