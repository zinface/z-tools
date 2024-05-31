# Spark WithGit

set(SPARK_FRAMEWORK_TO "${CMAKE_SOURCE_DIR}/cmake/_spark")

function(spark_framework_from_git)

    # 1. 解析参数，使用原生 cmake 提供的参数解析器
    # set(OPTIONS)
    set(ONEVARG PREFIX)
    set(MULVARG COMPONENTS)

    cmake_parse_arguments(SPARK "" "${ONEVARG}" "${MULVARG}" ${ARGN})

    # 2. 如果包含 PREFIX 则开始进行解析
    if(SPARK_PREFIX)
        if(NOT EXISTS "${SPARK_FRAMEWORK_TO}")
            execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${SPARK_FRAMEWORK_TO}")
        endif(NOT EXISTS "${SPARK_FRAMEWORK_TO}")

        foreach(SPARK_COMPONENT IN LISTS SPARK_COMPONENTS)
            # execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${SPARK_PREFIX}.${SPARK_COMPONENT}'"
            #     WORKING_DIRECTORY ${SPARK_FRAMEWORK_TO})
            if(NOT EXISTS "${SPARK_FRAMEWORK_TO}/${SPARK_COMPONENT}")
                execute_process(COMMAND git clone "${SPARK_PREFIX}/${SPARK_COMPONENT}"
                    WORKING_DIRECTORY ${SPARK_FRAMEWORK_TO})
            else()
                message("[SparkWithGit] Exits: ${SPARK_COMPONENT}")
            endif(NOT EXISTS "${SPARK_FRAMEWORK_TO}/${SPARK_COMPONENT}")
        endforeach(SPARK_COMPONENT IN LISTS SPARK_COMPONENTS)
    endif(SPARK_PREFIX)

    # 方案一：.gitignoe
    if(NOT EXISTS "${SPARK_FRAMEWORK_TO}/.gitignore")
        file(WRITE "${SPARK_FRAMEWORK_TO}/.gitignore" "spark.*/\n.gitignore")
    endif(NOT EXISTS "${SPARK_FRAMEWORK_TO}/.gitignore")

    # 方案二： ../.gitignore
    # if(NOT EXISTS "${SPARK_FRAMEWORK_TO}/../.gitignore")
    #     file(WRITE "${SPARK_FRAMEWORK_TO}/../.gitignore" "_spark/\n.gitignore")
    # endif(NOT EXISTS "${SPARK_FRAMEWORK_TO}/../.gitignore")

endfunction(spark_framework_from_git)

macro(spark_include)
    set(_spark_files ${ARGN})
    foreach(_spark_file IN LISTS _spark_files)
        if(EXISTS "${_spark_file}")
            include(${_spark_file})
            continue()
        endif(EXISTS "${_spark_file}")
        file(GLOB_RECURSE _file RELATIVE "${SPARK_FRAMEWORK_TO}" ${_spark_file})
        list(FILTER _file EXCLUDE REGEX "\\.\\.")
        
        if(NOT "${_file}" STREQUAL "")
            if(EXISTS "${SPARK_FRAMEWORK_TO}/${_file}")
                message("FOUND: ${SPARK_FRAMEWORK_TO}/${_file}")
                include("${SPARK_FRAMEWORK_TO}/${_file}")
            endif(EXISTS "${SPARK_FRAMEWORK_TO}/${_file}")
        else()
            message(WARNING "NOT FOUND: ${_spark_file}")
        endif(NOT "${_file}" STREQUAL "")
    endforeach(_spark_file IN LISTS _spark_files)
endmacro(spark_include)


# 引入的所有 Spark 构建模块
spark_framework_from_git(
    PREFIX
        http://gitee.com/zinface/
    COMPONENTS
        spark.assets-icon
        spark.env
        spark.macros
        spark.find-qt5
        spark.find-qt6
        spark.find-dtk
        spark.find-library
        spark.macros-extend
        spark.build-graphviz

        spark.framework
        spark.cmake-info
        spark.external-project

        spark.translator-macro
        spark.install-macros
        spark.desktop-macro
        spark.deb-package
        spark.appimage-macros-online
)

# include(cmake/SparkWithGit.cmake)

# 用于 spark_include 引入相关的列表项，每一项都是可引用的构建模块
spark_include(
    SparkEnvConfig.cmake
    SparkMacrosConfig.cmake
    SparkFindQt5Config.cmake
#     SparkFindQt6Config.cmake
    # SparkFindDtkConfig.cmake
#     SparkFindLibraries.cmake
    SparkMacrosExtendConfig.cmake

#     SparkFramework.cmake
#     SparkCMakeInfoAfterConfig.cmake
#     SparkCMakeInfoBeforeConfig.cmake
#     SparkExternalProject.cmake

#     SparkTranslatorConfig.cmake
    SparkInstallMacrosConfig.cmake
    SparkBuildGraphviz.cmake
    SparkDesktopMacros.cmake
    SparkDebPackageConfig.cmake
#     SparkAppimageConfig.cmake
)
