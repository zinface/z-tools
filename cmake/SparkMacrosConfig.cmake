cmake_minimum_required(VERSION 3.5.1)

# 定义一些 macro 用于自动生成构建结构

# spark_aux_source_directory outvar invar [skip]
# 获取目录下的所有源代码
macro(spark_aux_source_directory OUTVAR INVAR)
    # iv: internal_variable
    set(iv_args ${ARGN})
    list(LENGTH iv_args iv_arglen)
    
    file(GLOB iv_SOURCE_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${INVAR}/*.c ${INVAR}/*.cpp ${INVAR}/*.cc)
    file(GLOB iv_HEADER_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${INVAR}/*.h ${INVAR}/*.hpp)
    file(GLOB iv_QT_UI_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${INVAR}/*.ui ${INVAR}/*.qrc)

    if(iv_arglen EQUAL 1)
        list(APPEND ${OUTVAR} ${iv_SOURCE_LIST} ${iv_HEADER_LIST} ${iv_QT_UI_LIST})
    else()
        set(${OUTVAR} ${iv_SOURCE_LIST} ${iv_HEADER_LIST} ${iv_QT_UI_LIST})
    endif(iv_arglen EQUAL 1)

    unset(iv_args)
    unset(iv_arglen)
    unset(iv_SOURCE_LIST)
    unset(iv_HEADER_LIST)
    unset(iv_QT_UI_LIST)

endmacro(spark_aux_source_directory OUTVAR INVAR)

# spark_aux_source_directories outvar invar [...]
# 获取目录列表下的所有源代码
    # spark_aux_source_directory 的扩展，支持多个 invar 与追加参数
macro(spark_aux_source_directories OUTVAR INVAR)
    set(iv_aux_directories ${ARGN})
    
    spark_aux_source_directory(${OUTVAR} ${INVAR})

    foreach(iv_directory IN LISTS iv_aux_directories)
        spark_aux_source_directory(${OUTVAR} ${iv_directory} SKIP)
    endforeach(iv_directory IN LISTS iv_aux_directories)

    unset(iv_aux_directories)

endmacro(spark_aux_source_directories OUTVAR INVAR)


# spark_add_library <lib_name> [files]...
# 构建一个库，基于指定的源文件
    # 并根据库名生成 target_link_<lib_name> 函数
macro(spark_add_library _lib_name)
    spark_debug_message("================ ${_lib_name} Library ================")
    add_library(${_lib_name} ${ARGN})

    set(SRCS ${ARGN})
    foreach(item IN LISTS SRCS)
        spark_debug_message(" -> ${item}")
    endforeach(item IN LISTS SRCS)

    function(target_link_${_lib_name} TARGET)
        spark_debug_message("${_lib_name}")
        target_link_libraries(${TARGET} ${_lib_name})
    endfunction(target_link_${_lib_name} TARGET)

endmacro(spark_add_library _lib_name)

# spark_add_library_path <lib_name> <lib_path>
# 构建一个库，基于指定的路径
    # 并根据库名生成 target_link_<lib_name> 函数
        # 函数内增加以 <lib_path> 头文件搜索路径
macro(spark_add_library_path _lib_name _lib_path)

    # 0. 建立初始变量体系
    set(${_lib_name}_TYPE)
    set(${_lib_name}_TYPE_MESSAGE "STATIC(Default)")
    set(${_lib_name}_ARGN ${ARGN})

    # 1. 判断 _lib_path 是否是 SHARED 或 STATIC
    if(${_lib_path} STREQUAL SHARED OR ${_lib_path} STREQUAL STATIC)
        set(${_lib_name}_TYPE ${_lib_path})
        set(${_lib_name}_TYPE_MESSAGE ${${_lib_name}_TYPE})

        if(${ARGC} LESS 3)
            message(FATAL_ERROR "Missing parameter, library path not specified.")
        endif(${ARGC} LESS 3)
    else()
        # 如没有则将 _lib_path 加入到 ARGN 
        list(APPEND ${_lib_name}_ARGN ${_lib_path})
    endif(${_lib_path} STREQUAL SHARED OR ${_lib_path} STREQUAL STATIC)

    # 1. 处理由 spark_add_library_realpaths 构建转本构建时的清洗机制
    spark_debug_message("> Building: ${_lib_name}, type: ${${_lib_name}_TYPE_MESSAGE}")
    set(${_lib_name}_ARGN_REF ${${_lib_name}_ARGN})
    unset(${_lib_name}_ARGN)
    foreach(_old IN LISTS ${_lib_name}_ARGN_REF)
        set(_new ${_old})
        string(FIND "${_old}" "+" _plus_index)
        if(${_plus_index} GREATER 0)
            string(SUBSTRING "${_old}" 0 ${_plus_index} _new)
            spark_debug_message("  [CONVERT] ${_new} <- ${_old}")
        endif(${_plus_index} GREATER 0)
        list(APPEND ${_lib_name}_ARGN ${_new})
    endforeach(_old IN LISTS ${_lib_name}_ARGN_REF)
    

    # 2.目标参数项分析出子项
    # 拆分出源代码、路径、未知项等
    set(${_lib_name}_ARGN_SOURCES)
    set(${_lib_name}_ARGN_APPEND_PATHS)
    set(${_lib_name}_ARGN_UNKNOW)
    foreach(item IN LISTS ${_lib_name}_ARGN)
        spark_debug_message("  [ARGN] check:" ${item})
        if(CMAKE_VERSION VERSION_LESS 3.14)
            string(REGEX MATCH "\.qm$" fext "${item}")
        else()
            get_filename_component(fext "${item}" LAST_EXT)
        endif(CMAKE_VERSION VERSION_LESS 3.14)
        
        if(NOT EXISTS ${item} AND NOT "${fext}"  STREQUAL ".qm")
            set(item ${CMAKE_CURRENT_LIST_DIR}/${item})
        endif()
        if(EXISTS ${item})
            # spark_debug_message("       exists: true")
            if(CMAKE_VERSION VERSION_LESS 3.19)
                if(IS_ABSOLUTE ${item})
                    set(${_lib_name}_ARGN_item ${item})
                else()
                    set(${_lib_name}_ARGN_item ${CMAKE_CURRENT_SOURCE_DIR}/${item})
                endif(IS_ABSOLUTE ${item})
            else()
                file(REAL_PATH ${item} ${_lib_name}_ARGN_item)
            endif(CMAKE_VERSION VERSION_LESS 3.19)
            if(IS_DIRECTORY  ${${_lib_name}_ARGN_item})
                list(APPEND ${_lib_name}_ARGN_APPEND_PATHS ${item})
            else()
                list(APPEND ${_lib_name}_ARGN_SOURCES ${item})
            endif(IS_DIRECTORY  ${${_lib_name}_ARGN_item})
        else()
            if("${fext}" STREQUAL ".qm")
                list(APPEND ${_lib_name}_ARGN_SOURCES ${item})
            else()
                list(APPEND ${_lib_name}_ARGN_UNKNOW ${item})
                spark_debug_message("       exists: false")
            endif("${fext}" STREQUAL ".qm")
        endif()
    endforeach()

    list(LENGTH ${_lib_name}_ARGN_SOURCES      ${_lib_name}_ARGN_SOURCES_LENGTH)
    list(LENGTH ${_lib_name}_ARGN_APPEND_PATHS ${_lib_name}_ARGN_APPEND_PATHS_LENGTH)
    list(LENGTH ${_lib_name}_ARGN_UNKNOW       ${_lib_name}_ARGN_UNKNOW_LENGTH)
    spark_debug_message("       result: files(${${_lib_name}_ARGN_SOURCES_LENGTH}), paths(${${_lib_name}_ARGN_APPEND_PATHS_LENGTH}), unknow(${${_lib_name}_ARGN_UNKNOW_LENGTH})" ${item})

    # 3. 获取所以源代码为 any_files
    spark_debug_message("  files:")
    set(any_files ${${_lib_name}_ARGN_SOURCES})
    foreach(item IN LISTS ${_lib_name}_ARGN_APPEND_PATHS)
        spark_aux_source_directory(item_files ${item})
        list(APPEND any_files ${item_files})
        foreach(item_file IN LISTS item_files)
            spark_debug_message("       ${item_file}")
        endforeach(item_file IN LISTS item_files)
    endforeach(item IN LISTS ${_lib_name}_ARGN_APPEND_PATHS)

    # 3.1 转化 Qt5/6 的 ui 文件
    set(${_lib_name}_SOURCE_UIS)
    set(ui_wrap_files)
    # foreach(item IN LISTS ${_lib_name}_ARGN_SOURCES any_files)
    #     get_filename_component(ex "${item}" LAST_EXT)
    #     if("${ex}" STREQUAL ".ui")
    #         list(APPEND ${_lib_name}_SOURCE_UIS ${item})
    #     endif("${ex}" STREQUAL ".ui")
    # endforeach(item IN LISTS ${_lib_name}_ARGN_SOURCES any_files)
    
    # if(SPARK_FIND_QT6)
    #     qt_wrap_ui(ui_wrap_files ${${_lib_name}_SOURCE_UIS})
    # elseif(SPARK_FIND_QT5)
    #     qt5_wrap_ui(ui_wrap_files ${${_lib_name}_SOURCE_UIS})
    # endif(SPARK_FIND_QT6)

    # 4. 构建目标库
    add_library(${_lib_name} ${${_lib_name}_TYPE} 
        ${${_lib_name}_ARGN_SOURCES}
        ${any_files} ${ui_wrap_files})

    # 5. 建立引用点 
        # target_link_<_lib_name> 函数
        # target_include_<_lib_name> 函数
        
        # target_<_lib_name>_include 函数
        # target_<_lib_name>_link 函数

    function(target_${_lib_name}_include _include)
        spark_debug_message("添加引用: ${_lib_name} <- ${_include} ${${_lib_name}_INCLUDE_ARGN}")
        target_include_directories(${_lib_name} PRIVATE ${_include})
    endfunction(target_${_lib_name}_include _include)

    function(target_${_lib_name}_link _library)
        spark_debug_message("添加链接: ${_lib_name} <- ${_library} ${${_lib_name}_LINK_ARGN}")
        target_link_libraries(${_lib_name} ${_library})
    endfunction(target_${_lib_name}_link _library)
    
    function(target_link_${_lib_name} TARGET)
        spark_debug_message("链接引用: ${TARGET} <- ${_lib_name}")
        target_include_directories(${TARGET} PRIVATE 
            "${${_lib_name}_SOURCE_PATH}" ${${_lib_name}_ARGN_APPEND_PATHS})
        target_link_libraries(${TARGET} ${_lib_name})
    endfunction(target_link_${_lib_name} TARGET)

    function(target_include_${_lib_name} TARGET)
        spark_debug_message("引入引用: ${TARGET} <- ${_lib_name}")
        target_include_directories(${TARGET} PUBLIC 
            "${${_lib_name}_SOURCE_PATH}" ${${_lib_name}_ARGN_APPEND_PATHS})
    endfunction(target_include_${_lib_name} TARGET)

    
    target_include_directories(${_lib_name} PRIVATE 
        "${${_lib_name}_ARGN_APPEND_PATHS}")

    # 输出 includes
    spark_debug_message("  ${_lib_name}_ARGN_APPEND_PATHS: ")
    foreach(item IN LISTS ${_lib_name}_ARGN_APPEND_PATHS)
        string(REPLACE "${CMAKE_SOURCE_DIR}/" "" item_var "${item}")
        spark_debug_message("        ${item_var}")
    endforeach(item IN LISTS ${_lib_name}_ARGN_APPEND_PATHS)

    # 如果想用以下操作手动实现 target_link_include_directories
        # 请注意对 LIST 类型使用 "" 进行包围
        # target_link_include_directories 的 PUBLIC 将会填充(追加)目标的 INCLUDE_DIRECTORIES 属性
        # target_link_include_directories 支持 cmake 生成大表达式，更容易操作，手动将无法实现此类能力
        # target_link_include_directories 支持相对路径和绝对路径参数
            # 手动操作将必须使用绝对路径，这是不好的地方
    # get_target_property(_lib_include_directories ${_lib_name} INCLUDE_DIRECTORIES)
    # list(APPEND _lib_include_directories "${CMAKE_CURRENT_LIST_DIR}/${${_lib_name}_SOURCE_PATH}")
    # spark_debug_message("----> ${CMAKE_CURRENT_LIST_DIR}/${${_lib_name}_SOURCE_PATH}")
    # spark_debug_message("----> ${_lib_include_directories}")
    # set_target_properties(${_lib_name} PROPERTIES
    #     INCLUDE_DIRECTORIES "${_lib_include_directories}"
        # INTERFACE_INCLUDE_DIRECTORIES "${_lib_include_directories}"
    # )

endmacro(spark_add_library_path _lib_name _lib_path)

# spark_add_shared_library <target> [files ...]
# 构建一个共享库，基于指定的源代码
    # 并根据库名生成 target_link_<lib_name> 函数
macro(spark_add_shared_library _lib_name)
    spark_add_library(${_lib_name} SHARED ${ARGN})
endmacro(spark_add_shared_library _lib_name)

# spark_add_shared_library_path <target> [files ... paths]
# 构建一个共享库，基于指定的路径
    # 并根据库名生成 target_link_<lib_name> 函数
macro(spark_add_shared_library_path _lib_name)
    spark_add_library_path(${_lib_name} SHARED ${ARGN})
endmacro(spark_add_shared_library_path _lib_name)

# spark_add_executable <exec_name> [files]...
# 构建一个可执行文件，基于指定的源文件
    # Qt编译时源文件包括很多类型，需要指定 *.h/*.cpp/*.qrc/*.qm/... 等
macro(spark_add_executable _exec_name)

    set(${_exec_name}_TYPE_MESSAGE "可执行程序")
    spark_debug_message("> Building: ${_exec_name}, type: ${${_exec_name}_TYPE_MESSAGE}")
    
    add_executable(${_exec_name} ${ARGN})

endmacro(spark_add_executable _exec_name)

# spark_add_executable_path <target> <path> [files ... paths]
# 构建一个可执行程序，基于指定的路径
macro(spark_add_executable_path _exec_name _exec_path)
    spark_add_executable(${_exec_name})

    # 0. 建立初始变量体系
    # set(${_exec_name}_TYPE)
    # set(${_exec_name}_TYPE_MESSAGE "可执行程序")
    set(${_exec_name}_ARGN ${ARGN})

    # 1. 处理由 spark_add_executable_realpaths 构建转本构建时的清洗机制
    # spark_debug_message("> Building: ${_exec_name}, type: ${${_exec_name}_TYPE_MESSAGE}")
    set(${_exec_name}_ARGN_REF ${${_exec_name}_ARGN})
    unset(${_exec_name}_ARGN)
    foreach(_old IN LISTS ${_exec_name}_ARGN_REF)
        set(_new ${_old})
        string(FIND "${_old}" "+" _plus_index)
        if(${_plus_index} GREATER 0)
            string(SUBSTRING "${_old}" 0 ${_plus_index} _new)
            spark_debug_message("  [CONVERT] ${_new} <- ${_old}")
        endif(${_plus_index} GREATER 0)
        list(APPEND ${_exec_name}_ARGN ${_new})
    endforeach(_old IN LISTS ${_exec_name}_ARGN_REF)

    # 1.目标参数项分析出子项
    # 拆分出源代码、路径、未知项等
    # spark_debug_message("> Building: ${_exec_name}, type: ${${_exec_name}_TYPE_MESSAGE}")
    set(${_exec_name}_ARGN_SOURCES)
    set(${_exec_name}_ARGN_APPEND_PATHS ${_exec_path})
    set(${_exec_name}_ARGN_UNKNOW)
    foreach(item IN LISTS ${_exec_name}_ARGN)
        spark_debug_message("  [ARGN] check:" ${item})
        if(CMAKE_VERSION VERSION_LESS 3.14)
            string(REGEX MATCH "\.qm$" fext "${item}")
        else()
            get_filename_component(fext "${item}" LAST_EXT)
        endif(CMAKE_VERSION VERSION_LESS 3.14)
        
        if(NOT EXISTS ${item} AND NOT "${fext}"  STREQUAL ".qm")
            set(item ${CMAKE_CURRENT_LIST_DIR}/${item})
        endif()
        if(EXISTS ${item})
            # spark_debug_message("       exists: true")
            if(CMAKE_VERSION VERSION_LESS 3.19)
                if(IS_ABSOLUTE ${item})
                    set(${_exec_name}_ARGN_item ${item})
                else()
                    set(${_exec_name}_ARGN_item ${CMAKE_CURRENT_SOURCE_DIR}/${item})
                endif(IS_ABSOLUTE ${item})
            else()
                file(REAL_PATH ${item} ${_lib_name}_ARGN_item)
            endif(CMAKE_VERSION VERSION_LESS 3.19)
            if(IS_DIRECTORY  ${${_exec_name}_ARGN_item})
                list(APPEND ${_exec_name}_ARGN_APPEND_PATHS ${item})
            else()
                list(APPEND ${_exec_name}_ARGN_SOURCES ${item})
            endif(IS_DIRECTORY  ${${_exec_name}_ARGN_item})
        else()
            if("${fext}" STREQUAL ".qm")
                list(APPEND ${_exec_name}_ARGN_SOURCES ${item})
            else()
                list(APPEND ${_exec_name}_ARGN_UNKNOW ${item})
                spark_debug_message("       exists: false")
            endif("${fext}" STREQUAL ".qm")
        endif()
    endforeach()

    list(LENGTH ${_exec_name}_ARGN_SOURCES      ${_exec_name}_ARGN_SOURCES_LENGTH)
    list(LENGTH ${_exec_name}_ARGN_APPEND_PATHS ${_exec_name}_ARGN_APPEND_PATHS_LENGTH)
    list(LENGTH ${_exec_name}_ARGN_UNKNOW       ${_exec_name}_ARGN_UNKNOW_LENGTH)
    spark_debug_message("       result: files(${${_exec_name}_ARGN_SOURCES_LENGTH}), paths(${${_exec_name}_ARGN_APPEND_PATHS_LENGTH}), unknow(${${_exec_name}_ARGN_UNKNOW_LENGTH})" ${item})


    # 2. 获取所以源代码为 any_files
    spark_debug_message("  files:")
    set(any_files ${${_exec_name}_ARGN_SOURCES})
    foreach(item IN LISTS ${_exec_name}_ARGN_APPEND_PATHS)
        spark_aux_source_directory(item_files ${item})
        list(APPEND any_files ${item_files})
        foreach(item_file IN LISTS item_files)
            spark_debug_message("       ${item_file}")
        endforeach(item_file IN LISTS item_files)
    endforeach(item IN LISTS ${_exec_name}_ARGN_APPEND_PATHS)

    # 3.1 转化 Qt5/6 的 ui 文件
    set(${_exec_name}_SOURCE_UIS)
    set(ui_wrap_files)
    # foreach(item IN LISTS ${_exec_name}_ARGN_SOURCES any_files)
    #     get_filename_component(ex "${item}" LAST_EXT)
    #     if("${ex}" STREQUAL ".ui")
    #         list(APPEND ${_exec_name}_SOURCE_UIS ${item})
    #     endif("${ex}" STREQUAL ".ui")
    # endforeach(item IN LISTS ${_exec_name}_ARGN_SOURCES any_files)
    
    # if(SPARK_FIND_QT6)
    #     qt_wrap_ui(ui_wrap_files ${${_exec_name}_SOURCE_UIS})
    # elseif(SPARK_FIND_QT5)
    #     qt5_wrap_ui(ui_wrap_files ${${_exec_name}_SOURCE_UIS})
    # endif(SPARK_FIND_QT6)
 
    # 3. 构建可执行目标所需要的文件
    # add_executable(${_exec_name} 
    #     ${${_exec_name}_ARGN_SOURCES}
    #     ${any_files})

    target_sources(${_exec_name} PRIVATE
        ${${_exec_name}_ARGN_SOURCES}
        ${any_files} ${ui_wrap_files})

     # 4. 建立引用点 
        # target_<_exec_name>_include 函数
        # target_<_exec_name>_link 函数
    function(target_${_exec_name}_include _include)
        spark_debug_message("添加引用: ${_exec_name} <- ${_include} ${${_exec_name}_INCLUDE_ARGN}")
        target_include_directories(${_exec_name} PRIVATE ${_include})
    endfunction(target_${_exec_name}_include _include)

    function(target_${_exec_name}_link _library)
        spark_debug_message("添加链接: ${_exec_name} <- ${_library} ${${_exec_name}_LINK_ARGN}")
        target_link_libraries(${_exec_name} ${_library})
    endfunction(target_${_exec_name}_link _library)
    
    target_include_directories(${_exec_name} PRIVATE 
        ${_exec_path})
    spark_debug_message("  include: ${_exec_path}\n")
    
endmacro(spark_add_executable_path _exec_name _exec_path)

# spark_find_library
# 搜索一个库，基于指定的库名，调用 pkg-config 搜索库
    # 并根据库名生成一个 target_link_<prefix> 函数
macro(spark_find_library _prefix)
    find_package(PkgConfig REQUIRED)

    # libnotify
    pkg_check_modules(${_prefix} ${ARGN})
    function(target_link_${_prefix} TARGET)
        target_include_directories(${TARGET} PUBLIC
            ${${_prefix}_INCLUDE_DIRS})
        target_link_libraries(${TARGET}
            ${${_prefix}_LIBRARIES})
    endfunction(target_link_${_prefix} TARGET)

endmacro(spark_find_library _prefix)

macro(target_link_qt)
    
    if(SPARK_FIND_QT6)
        target_link_qt6(${ARGN})
    elseif(SPARK_FIND_QT5)
        target_link_qt5(${ARGN})
    endif(SPARK_FIND_QT6)

endmacro(target_link_qt)

# spark_add_executable_paths
# 自定义构建宏，基于指定的前缀名称，处理后续参数为子目录
    # item: 为进行遍历后的单项，类似于 python3 中的 (for item in items:)
    # file: 为在目录中不以递归(GLOB_RECURSE)方式寻找 qrc 文件，需要将其参与编译才能被 rcc
    # 并根据 prefix-<item> 生成构建目标，
macro(spark_add_executable_paths _prefix_path)
    set(PATHS ${ARGN})
    foreach(item IN LISTS PATHS)
        file(GLOB QRCS "${item}/*.qrc")
        spark_debug_message(">>> add_executable: " "${_prefix_path}-${item} ${item} + ${QRCS}")
        string(REPLACE "/" "-" new_item "${item}")
        spark_add_executable_path(${_prefix_path}-${new_item} ${item} ${QRCS})
        target_link_qt(${_prefix_path}-${item})
    endforeach(item IN LISTS PATHS)
endmacro(spark_add_executable_paths _prefix_path)

# spark_add_link
# 自定义宏以代替当前使用 fucntion 定义 target_link_<name> 结构
    # _IN_NAME: 此宏生成 target_link_<name> 的要求参数
    # ARGN: 此宏剩余的参数列表
        # 在使用 target_link_<name> 时
        # _NAME: 用于此 fucntion 中的要求参数: <_NAME>目标将要连接此库
macro(spark_add_link _name)
    function(target_link_${_name} _link)
        spark_debug_message("> Linking: ${_link}")
        spark_debug_message("        <- ${ARGN}\n")
        target_link_libraries(${_link}
            ${ARGN})
    endfunction(target_link_${_name} _link)
endmacro(spark_add_link _name)
