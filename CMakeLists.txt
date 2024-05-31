cmake_minimum_required(VERSION 3.0.0)

project(z-tools-fake-component VERSION 0.1 LANGUAGES CXX)

include(cmake/SparkWithGit.cmake)

spark_add_executable_path(${PROJECT_NAME}
    src
    src/*/*
    
    ${QRC_SOURCES}
)
target_link_qt5(${PROJECT_NAME})
