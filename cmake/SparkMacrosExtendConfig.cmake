
# find_plus
# 寻找 INVAl 传入的字符串，如果存在 + 字符将写入位置到 OUTVAL
function(find_plus INVAL OUTVAL)
    string(FIND "${INVAL}" "+" plus_index)
    set(${OUTVAL} ${plus_index} PARENT_SCOPE)
    # if(plus_index LESS 0)
    #     set(${OUTVAL} -1 PARENT_SCOPE)
    # else()
    #     set(${OUTVAL} ${plus_index} PARENT_SCOPE)
    # endif(plus_index LESS 0)
endfunction(find_plus INVAL OUTVAL)

# find_plus("FF" FFFF)
# message("--> FFFF ${FFFF}")  # --> FFFF -1
# find_plus("F+F" FFFF)
# message("--> FFFF ${FFFF}")  # --> FFFF 1
# find_plus("+F+F" FFFF)
# message("--> FFFF ${FFFF}")  # --> FFFF 0

# set(FFF)
# list(APPEND FFFF )
# list(APPEND FFFF "F")
# list(APPEND FFFF "FA")
# message("--> FFFF: ${FFFF}")  # --> FFFF: F;FA

# set(FFFFS "")
# list(APPEND FFFFS ${FFFF})
# message("--> FFFFS: ${FFFFS}")  # --> FFFFS: F;FA

# set(FFFF "+AA+BB+CC+DD")
# string(REPLACE "+" ";" FFFFL "${FFFF}")
# list(LENGTH FFFFL FFFFLEN)
# message("--> FFFFL: ${FFFFL} --> ${FFFFLEN}") # --> FFFFL: F;

# plus_list
# 将传入的 "+AAA+BBB+CCC" 类型数据变成一个 列表(list)
# 适用于不使用 string 进行替换 + 为 ";" 的情况下使用直接变成 list
function(plus_list INVAL OUTVAL OUTVALLEN)
    # set(${OUTVAL} "..." PARENT_SCOPE)
    # set(${OUTVALLEN} 0 PARENT_SCOPE)

    set(_tmps "")       # 设置为空的

    # 寻找下一个 + 位置
    find_plus(${INVAL} RIGHT_PLUS)

    string(LENGTH "${INVAL}" INVALLEN)
    spark_debug_message("--> 传入的 INVAL: --> 内容: ${INVAL}")
    spark_debug_message("--> 传入的 INVAL: --> 长度: ${INVALLEN}")
    spark_debug_message("--> 传入的 INVAL: --> +位置: ${RIGHT_PLUS}")

    # 判断是否有右侧 + 号
    if(RIGHT_PLUS LESS 0)
        spark_debug_message("--> 传入的 INVAL: --> 无需计算新的+位置")
        # spark_debug_message("--> 计算新的 + 位置: ${_PLUSINDEX}")
        list(APPEND _tmps ${INVAL})
    else()
        math(EXPR _PLUSINDEX "${RIGHT_PLUS}+1")
        spark_debug_message("--> 传入的 INVAL: --> 需计算+位置 --> 右移: ${_PLUSINDEX}")

        string(SUBSTRING "${INVAL}" ${_PLUSINDEX} ${INVALLEN} NewVal)
        spark_debug_message("--> 传入的 INVAL: --> 需计算+位置 --> 右移: ${_PLUSINDEX} -> 内容: ${NewVal}")
        # string(REPLACE "+" ";" _tmps "${NewVal}")
        # list(LENGTH FFFFL FFFFLEN)

        # spark_debug_message("--> 计算新的 + 位置: ${_PLUSINDEX} --> 后面的 NewVal: ${NewVal}")

        # find_plus(${NewVal} _NextPlus)
        # if(_NextPlus LESS 0)
            # list(APPEND _tmps ${NewVal})
            # spark_debug_message("--> 追加新的 + 位置: ${_PLUSINDEX} --> 后面的")
        # else()
        #     spark_debug_message("--> 追加新的 + 位置: ${_PLUSINDEX} --> 后面的")
        #     # 重新
        #     # plus_list(${NewVal} NewValS )
        #     # foreach(item)
        #         # list(APPEND _tmps ${item})
        #     # endforeach(item)
        # endif(_NextPlus LESS 0)
    endif(RIGHT_PLUS LESS 0)

    set(${OUTVAL} ${_tmps} PARENT_SCOPE)
    list(LENGTH _tmps _tmps_len)
    set(${OUTVALLEN} ${_tmps_len} PARENT_SCOPE)

endfunction(plus_list INVAL OUTVAL OUTVALLEN)

# plus_list("+AAA+BBB+CCC+DDD" FFF FFLEN)
# spark_debug_message("--------> ${FFF}: -> ${FFLEN}")

# spark_add_library_realpaths
# 基于传入的项进行构建
# 可接受的值为: 路径列表
# 可接受的值为: 路径列表+依赖库A+依赖库B
macro(spark_add_library_realpaths)
    spark_debug_message("---> 基于传入的项进行构建 <---")
    # spark_debug_message("--> src/unclassified/ItemDelegates/NdStyledItemDelegate")
    # string(FIND <string> <substring> <output_variable> [REVERSE])
    # string(SUBSTRING <string> <begin> <length> <output_variable>)
    # math(EXPR value "100 * 0xA" OUTPUT_FORMAT DECIMAL)      # value is set to "1000"

    set(REALPATHS ${ARGN})
    foreach(REALPATH IN LISTS REALPATHS)
        spark_debug_message("---> 传入路径: ${REALPATH} <--- ")
        string(LENGTH "${REALPATH}" REALPATH_LENGTH)
        spark_debug_message("---> 计算传入路径长度: --> 长度: ${REALPATH_LENGTH}")

        string(FIND "${REALPATH}" "/" LASTINDEX REVERSE)
        spark_debug_message("---> 计算传入路径末尾/位置: --> 长度: ${LASTINDEX}")
        math(EXPR LASTINDEX "${LASTINDEX}+1")
        spark_debug_message("---> 计算传入路径末尾/右移: --> 长度: ${LASTINDEX}")
        string(SUBSTRING "${REALPATH}" ${LASTINDEX} ${REALPATH_LENGTH} REALNAME_Dependency)

        # 找 + 号下标，这是找+号的函数
        find_plus(${REALPATH} RIGHT_PLUS)

        # 判断是否有找到 + 号下标，值为 -1 或 正整数
        if(RIGHT_PLUS LESS 0) # 小于0: 不存在 + 号
            set(REALNAME "${REALNAME_Dependency}")
            spark_debug_message("---> 传入路径末尾/右移部分: --> ${REALNAME} <-- 无依赖+")

            spark_debug_message("---> 构建 ${REALNAME} -> ${REALNAME} ${REALPATH} ")

            spark_add_library_path(${REALNAME} ${REALPATH})

            if(SPARK_FIND_QT5)
                target_link_qt5(${REALNAME})
            endif(SPARK_FIND_QT5)

            if(SPARK_FIND_QT6)
                target_link_qt6(${REALNAME})
            endif(SPARK_FIND_QT6)

        else()
            spark_debug_message("---> 传入路径末尾/右移部分: --> ${REALNAME_Dependency} <-- 依赖+")

            # 存在+号，将截取从 / 到 + 号之间的内容作为目标名称
            # 例如 src/unclassified/widgets/DocTypeListView+JsonDeploy
            #                             ^(LASTINDEX)    ^(RIGHT_PLUS)
            # 将 RIGHT_PLUS - LASTINDEX 计算出 DocTypeListView 字符长度
            math(EXPR REALNAME_LENGTH "${RIGHT_PLUS}-${LASTINDEX}")

            spark_debug_message("---> 计算传入路径末尾/右移部分: --> 位置: ${RIGHT_PLUS}")
            # spark_debug_message("---> 计算传入路径末尾/右移部分: --> 长度: ${REALNAME_Dependency}")

            # 目标名称为 DocTypeListView
            # 依赖为    JsonDeploy
            # set(REALNAME "")
            string(SUBSTRING "${REALPATH}" 0 ${RIGHT_PLUS} _REALPATH_DIR)
            string(SUBSTRING "${REALPATH}" ${LASTINDEX} ${REALNAME_LENGTH} REALNAME)

            spark_debug_message("---> 计算传入路径末尾/右移部分: --> 库名: ${REALNAME}")

            string(SUBSTRING "${REALPATH}" ${RIGHT_PLUS} ${REALPATH_LENGTH} Dependency)
            spark_debug_message("---> 计算传入路径末尾/右移部分: --> 库名: ${REALNAME} --> +部分: ${Dependency}")

            # plus_list(${Dependency} dependencies dependencies_len)
            string(REPLACE "+" ";" dependencies "${Dependency}")
            spark_debug_message("---> 计算传入路径末尾/右移部分: --> 库名: ${REALNAME} --> +部分: ${Dependency} --> 列表: ${dependencies} <-- ")


            spark_debug_message("---> 构建 ${REALNAME} -> ${REALNAME} ${_REALPATH_DIR}")

            spark_add_library_path(${REALNAME} ${_REALPATH_DIR})
            # target_link_qt5(${REALNAME}) # 使用依赖的依赖或许也不错

            target_include_directories(${REALNAME} PUBLIC ${_REALPATH_DIR})
            target_link_libraries(${REALNAME} ${dependencies})

        endif(RIGHT_PLUS LESS 0)
    endforeach(REALPATH IN LISTS REALPATHS)

endmacro(spark_add_library_realpaths)


# spark_aux_source_paths
# 将指定路径中的文件变成可用的AUX源文件列表
macro(spark_aux_source_paths AUX_VAR)
    set(${AUX_VAR} "")
    set(${AUX_VAR}_PATHS ${ARGN})

    foreach(aux_path IN LISTS ${AUX_VAR}_PATHS)
        # spark_debug_message("aux_path: ${aux_path}")
        aux_source_directory(${aux_path} ${AUX_VAR})
    endforeach(aux_path IN LISTS ${AUX_VAR}_PATHS)

endmacro(spark_aux_source_paths AUX_VAR)

# spark_file_glob
# 使用用 file(GLOB) 的匹配规则，并一次可匹配多个规则
#
macro(spark_file_glob FGLOB_VAR)
    set(${FGLOB_VAR} "")
    set(${FGLOB_VAR}_PATHS ${ARGN})

    foreach(fglob_path IN LISTS ${FGLOB_VAR}_PATHS)

        file(GLOB FGLOB_PATH_SRCS ${fglob_path})
        foreach(fglob_path_src IN LISTS FGLOB_PATH_SRCS)
            # spark_debug_message(" -> ${item}")
            list(APPEND ${FGLOB_VAR} ${fglob_path_src})
        endforeach(fglob_path_src IN LISTS FGLOB_PATH_SRCS)

    endforeach(fglob_path IN LISTS ${FGLOB_VAR}_PATHS)

endmacro(spark_file_glob FGLOB_VAR)


# spark_add_source_paths
# 将指定路径中的文件变成可用的源文件列表
#
macro(spark_add_source_paths SOURCE_VAR)
    set(${SOURCE_VAR} "")
    set(${SOURCE_VAR}_PATHS ${ARGN})

    spark_aux_source_paths(${SOURCE_VAR} ${ARGN})
    foreach(source_path IN LISTS ${SOURCE_VAR}_PATHS)
        # list(APPEND ${SOURCE_VAR}_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_PATH})
        # aux_source_directory(${SOURCE_PATH} _SOURCES)
        # foreach(item IN LISTS _SOURCES)
        #     # spark_debug_message(" -> ${item}")
        #     list(APPEND ${SOURCE_VAR} ${item})
        # endforeach(item IN LISTS _SOURCES)

        # file(GLOB HEADER_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCE_PATH}/*.h)
        # foreach(item IN LISTS HEADER_LIST)
        #     # spark_debug_message(" -> ${item}")
        #     list(APPEND ${SOURCE_VAR} ${item})
        # endforeach(item IN LISTS HEADER_LIST)

        file(GLOB UI_SRCS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${source_path}/*.ui)
        foreach(ui_src IN LISTS UI_SRCS)
            # spark_debug_message(" -> ${item}")
            list(APPEND ${SOURCE_VAR} ${ui_src})
        endforeach(ui_src IN LISTS UI_SRCS)
    endforeach(source_path IN LISTS ${SOURCE_VAR}_PATHS)
endmacro(spark_add_source_paths SOURCE_VAR)


# spark_add_library_file_glob
    # 
macro(spark_add_library_file_glob _lib_name)
    spark_file_glob(${_lib_name}_SOURCES ${ARGN})
    spark_add_library(${_lib_name} ${${_lib_name}_SOURCES})
endmacro(spark_add_library_file_glob _lib_name)
