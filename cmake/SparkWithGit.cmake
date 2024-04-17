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

macro(spark_include )
    set(_spark_files ${ARGN})
    foreach(_spark_file IN LISTS _spark_files)
        file(GLOB_RECURSE _file RELATIVE "${SPARK_FRAMEWORK_TO}" ${_spark_file})
        if(EXISTS "${SPARK_FRAMEWORK_TO}/${_file}")
            message("FOUND: ${SPARK_FRAMEWORK_TO}/${_file}")
            include("${SPARK_FRAMEWORK_TO}/${_file}")
        endif(EXISTS "${SPARK_FRAMEWORK_TO}/${_file}")
    endforeach(_spark_file IN LISTS _spark_files)
endmacro(spark_include _spark_file)


# 引入的所有 Spark 构建模块
spark_framework_from_git(
    PREFIX
        http://gitee.com/zinface/
    COMPONENTS
        spark.assets-icon
        spark.build-graphviz
        spark.cmake-info
        spark.deb-package
        spark.framework
        spark.env 
        spark.macros
        spark.macros-extend
        spark.find-library
        spark.find-qt5
        spark.find-qt6
        spark.find-dtk
        spark.translator-macro
        spark.desktop-macro
        spark.appimage-macros-online
        spark.install-macros
)
