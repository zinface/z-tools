
# spark_install_target
# 基于传入的路径/目标进行安装
# 可接受的值为: 安装路径 目标A
# 可接受的值为: 安装路径 目标A 目标B 目标C...
macro(spark_install_target INSTALL_TARGET_DIR INSTALL_TARGETS)
    install(TARGETS 
        ${INSTALL_TARGETS} ${ARGN}
        DESTINATION ${INSTALL_TARGET_DIR})
endmacro(spark_install_target INSTALL_TARGET_DIR INSTALL_TARGETS)

# spark_install_file
# 基于传入的路径/文件进行安装
# 可接受的值为: 安装路径 文件A
# 可接受的值为: 安装路径 文件A 文件B 文件C...
macro(spark_install_file INSTALL_FILE_DIR INSTALL_FILE)
    install(FILES 
        ${INSTALL_FILE} ${ARGN}
        DESTINATION ${INSTALL_FILE_DIR})
endmacro(spark_install_file INSTALL_FILE_DIR INSTALL_FILE)

# spark_install_program
# 基于传入的路径/文件进行安装，并自动为其添加可执行权限
# 可接受的值为: 安装路径 文件A
# 可接受的值为: 安装路径 文件A 文件B 文件C...
macro(spark_install_program INSTALL_PROGRAM_DIR INSTALL_PROGRAM)
    install(PROGRAMS 
        ${INSTALL_PROGRAM} ${ARGN}
        DESTINATION ${INSTALL_PROGRAM_DIR})
endmacro(spark_install_program INSTALL_PROGRAM_DIR INSTALL_PROGRAM)


# spark_install_directory
# 基于传入的路径/目录进行安装
# 可接受的值为: 安装路径 路径A
# 可接受的值为: 安装路径 路径A/* 为安装路径A下所有内容
macro(spark_install_directory INSTALL_DIRECTORY_DIR INSTALL_DIRECOTRY)
    # INSTALL_DIRECOTRY 可能包含 * ？
    # 1. 找到 '*', 截取，列出目录下所有文件，安装
    # 2. 是文件的直接使用 spark_install_file 安装
    # 2. 是目录的直接使用 spark_install_directory 安装
    # message(FATAL_ERROR "${INSTALL_DIRECTORY_DIR}")
    # string(FIND <string> <substring> <output_variable> [REVERSE])
    string(FIND "${INSTALL_DIRECOTRY}" "*" INSTALL_DIRECTORY_FIND_INDEX)
    # message(FATAL_ERROR "${INSTALL_DIRECTORY_FIND_INDEX}:  ${INSTALL_DIRECTORY_DIR}")

    # file(GLOB <variable>
    #  [LIST_DIRECTORIES true|false] [RELATIVE <path>] [CONFIGURE_DEPENDS]
    #  [<globbing-expressions>...])

    if (NOT INSTALL_DIRECTORY_FIND_INDEX EQUAL -1)
        # string(SUBSTRING <string> <begin> <length> <output_variable>)
        string(SUBSTRING "${INSTALL_DIRECOTRY}" 0 ${INSTALL_DIRECTORY_FIND_INDEX} INSTALL_DIRECTORY_FIND_INDEX_SUBSTRING)
        # message(FATAL_ERROR "directory: ${INSTALL_DIRECTORY_FIND_INDEX_SUBSTRING}")

        # file(GLOB <variable>
        #     [LIST_DIRECTORIES true|false] [RELATIVE <path>] [CONFIGURE_DEPENDS]
        #     [<globbing-expressions>...])

        file(GLOB INSTALL_DIRECTORY_FIND_INDEX_SUBSTRING_FILE_GLOB_LIST  ${INSTALL_DIRECTORY_FIND_INDEX_SUBSTRING}/*)
        list(LENGTH INSTALL_DIRECTORY_FIND_INDEX_SUBSTRING_FILE_GLOB_LIST INSTALL_DIRECTORY_FIND_INDEX_SUBSTRING_FILE_GLOB_LIST_LENGTH)
        foreach(item IN LISTS INSTALL_DIRECTORY_FIND_INDEX_SUBSTRING_FILE_GLOB_LIST)
            # message("-> ${item}")
            if(IS_DIRECTORY ${item})
                message("-> ${item} IS_DIRECTORY")
                # spark_install_directory(${INSTALL_DIRECTORY_DIR} ${item})
                install(DIRECTORY
                    ${item}
                    DESTINATION ${INSTALL_DIRECTORY_DIR}
                    USE_SOURCE_PERMISSIONS)
            else()
                message("-> ${item} NOT IS_DIRECTORY")
                spark_install_program(${INSTALL_DIRECTORY_DIR} ${item})
                # spark_install_file(${INSTALL_DIRECTORY_DIR} ${item})
            endif(IS_DIRECTORY ${item})
        endforeach(item IN LISTS INSTALL_DIRECTORY_FIND_INDEX_SUBSTRING_FILE_GLOB_LIST)
        
        # message(FATAL_ERROR " directory: ${INSTALL_DIRECTORY_FIND_INDEX_SUBSTRING_FILE_GLOB_LIST}")
        # message(FATAL_ERROR " directory: ${INSTALL_DIRECTORY_FIND_INDEX_SUBSTRING_FILE_GLOB_LIST_LENGTH}")

    else()
        message(FATAL_ERROR "install： ${INSTALL_DIRECTORY_DIR}")

        install(DIRECTORY
            ${INSTALL_DIRECOTRY} ${ARGN}
            DESTINATION ${INSTALL_DIRECTORY_DIR})
    endif(NOT INSTALL_DIRECTORY_FIND_INDEX EQUAL -1)

endmacro(spark_install_directory INSTALL_DIRECTORY_DIR INSTALL_DIRECOTRY)


# spark_install_changelog
# 基于传入的路径/ changelog 文件路径进行安装，经过一系列检查并使用 gzip 进行压缩并安装
# 可接受的值为: 安装路径 changelog文件路径
macro(spark_install_changelog CHANGE_LOG_FILE)
    set(SOURCE_CHANGE_LOG_FILE ${CHANGE_LOG_FILE})
    if (EXISTS ${SOURCE_CHANGE_LOG_FILE})

        execute_process(COMMAND test -f ${SOURCE_CHANGE_LOG_FILE}
            RESULT_VARIABLE changelog_test
        )
        execute_process(COMMAND which gzip
            RESULT_VARIABLE gzip_test
        )
        if (NOT changelog_test  EQUAL 0)
            message(FATAL_ERROR "NOTE: 不是常规文件: ${SOURCE_CHANGE_LOG_FILE}")
        endif(NOT changelog_test  EQUAL 0)

        if (NOT gzip_test  EQUAL 0)
            message(FATAL_ERROR "NOTE: 未安装 gzip, 无法压缩 changelog")
        endif(NOT gzip_test  EQUAL 0)

        # 压缩与安装日志文件
        add_custom_command(
            OUTPUT "${CMAKE_BINARY_DIR}/changelog.gz"
            COMMAND gzip -cn9 "${SOURCE_CHANGE_LOG_FILE}" > "${CMAKE_BINARY_DIR}/changelog.gz"
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            COMMENT "Compressing changelog"
        )
        add_custom_target(changelog ALL DEPENDS "${CMAKE_BINARY_DIR}/changelog.gz")

        # include(GNUInstallDirs)
        set(SPARK_INSTALL_CHANGE_LOG_DIR "/usr/share/doc/${PROJECT_NAME}/")
        install(FILES 
            ${CMAKE_BINARY_DIR}/changelog.gz
            debian/copyright

            DESTINATION ${SPARK_INSTALL_CHANGE_LOG_DIR})
    else()
        message(FATAL_ERROR "未找到: ${SOURCE_CHANGE_LOG_FILE}")
    endif(EXISTS ${SOURCE_CHANGE_LOG_FILE})
endmacro(spark_install_changelog CHANGE_LOG_FILE)
