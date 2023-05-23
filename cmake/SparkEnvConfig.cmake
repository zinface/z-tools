cmake_minimum_required(VERSION 3.5.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
# set(CMAKE_BUILD_TYPE "Debug")

option(SPARK_DEBUG_MESSAGE "CMake Spark Module Debug Message." OFF)

macro(spark_debug_message)
    if(SPARK_DEBUG_MESSAGE)
        message(${ARGN})
    endif(SPARK_DEBUG_MESSAGE)
endmacro(spark_debug_message)
