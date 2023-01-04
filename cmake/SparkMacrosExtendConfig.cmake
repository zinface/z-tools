
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
    message("--> 传入的 INVAL: --> 内容: ${INVAL}")
    message("--> 传入的 INVAL: --> 长度: ${INVALLEN}")
    message("--> 传入的 INVAL: --> +位置: ${RIGHT_PLUS}")

    # 判断是否有右侧 + 号
    if(RIGHT_PLUS LESS 0)
        message("--> 传入的 INVAL: --> 无需计算新的+位置")
        # message("--> 计算新的 + 位置: ${_PLUSINDEX}")
        list(APPEND _tmps ${INVAL})
    else()
        math(EXPR _PLUSINDEX "${RIGHT_PLUS}+1")
        message("--> 传入的 INVAL: --> 需计算+位置 --> 右移: ${_PLUSINDEX}")

        string(SUBSTRING "${INVAL}" ${_PLUSINDEX} ${INVALLEN} NewVal)
        message("--> 传入的 INVAL: --> 需计算+位置 --> 右移: ${_PLUSINDEX} -> 内容: ${NewVal}")
        # string(REPLACE "+" ";" _tmps "${NewVal}")
        # list(LENGTH FFFFL FFFFLEN)
        
        # message("--> 计算新的 + 位置: ${_PLUSINDEX} --> 后面的 NewVal: ${NewVal}")
        
        # find_plus(${NewVal} _NextPlus)
        # if(_NextPlus LESS 0)
            # list(APPEND _tmps ${NewVal})
            # message("--> 追加新的 + 位置: ${_PLUSINDEX} --> 后面的")
        # else()
        #     message("--> 追加新的 + 位置: ${_PLUSINDEX} --> 后面的")
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
# message("--------> ${FFF}: -> ${FFLEN}")

# spark_add_library_realpaths
# 基于传入的项进行构建
# 可接受的值为: 路径列表
# 可接受的值为: 路径列表+依赖库A+依赖库B
macro(spark_add_library_realpaths)
    message("---> 基于传入的项进行构建 <---")
    # message("--> src/unclassified/ItemDelegates/NdStyledItemDelegate")
    # string(FIND <string> <substring> <output_variable> [REVERSE])
    # string(SUBSTRING <string> <begin> <length> <output_variable>)
    # math(EXPR value "100 * 0xA" OUTPUT_FORMAT DECIMAL)      # value is set to "1000"

    set(REALPATHS ${ARGN})
    foreach(REALPATH IN LISTS REALPATHS)
        message("---> 传入路径: ${REALPATH} <--- ")
        string(LENGTH "${REALPATH}" REALPATH_LENGTH)
        message("---> 计算传入路径长度: --> 长度: ${REALPATH_LENGTH}")

        string(FIND "${REALPATH}" "/" LASTINDEX REVERSE)
        message("---> 计算传入路径末尾/位置: --> 长度: ${LASTINDEX}")
        math(EXPR LASTINDEX "${LASTINDEX}+1")
        message("---> 计算传入路径末尾/右移: --> 长度: ${LASTINDEX}")
        string(SUBSTRING "${REALPATH}" ${LASTINDEX} ${REALPATH_LENGTH} REALNAME_Dependency)

        # 找 + 号下标，这是找+号的函数
        find_plus(${REALPATH} RIGHT_PLUS) 
        
        # 判断是否有找到 + 号下标，值为 -1 或 正整数
        if(RIGHT_PLUS LESS 0) # 小于0: 不存在 + 号
            set(REALNAME "${REALNAME_Dependency}")
            message("---> 传入路径末尾/右移部分: --> ${REALNAME} <-- 无依赖+")

            message("---> 构建 ${REALNAME} -> ${REALNAME} ${REALPATH} ")

            spark_add_library_path(${REALNAME} ${REALPATH})
            target_link_qt5(${REALNAME})
        else()
            message("---> 传入路径末尾/右移部分: --> ${REALNAME_Dependency} <-- 依赖+")
            
            # 存在+号，将截取从 / 到 + 号之间的内容作为目标名称
            # 例如 src/unclassified/widgets/DocTypeListView+JsonDeploy
            #                             ^(LASTINDEX)    ^(RIGHT_PLUS)
            # 将 RIGHT_PLUS - LASTINDEX 计算出 DocTypeListView 字符长度
            math(EXPR REALNAME_LENGTH "${RIGHT_PLUS}-${LASTINDEX}")
            
            message("---> 计算传入路径末尾/右移部分: --> 位置: ${RIGHT_PLUS}")
            # message("---> 计算传入路径末尾/右移部分: --> 长度: ${REALNAME_Dependency}")
            
            # 目标名称为 DocTypeListView
            # 依赖为    JsonDeploy
            # set(REALNAME "")
            string(SUBSTRING "${REALPATH}" 0 ${RIGHT_PLUS} _REALPATH_DIR)
            string(SUBSTRING "${REALPATH}" ${LASTINDEX} ${REALNAME_LENGTH} REALNAME)
            
            message("---> 计算传入路径末尾/右移部分: --> 库名: ${REALNAME}")

            string(SUBSTRING "${REALPATH}" ${RIGHT_PLUS} ${REALPATH_LENGTH} Dependency)
            message("---> 计算传入路径末尾/右移部分: --> 库名: ${REALNAME} --> +部分: ${Dependency}")

            # plus_list(${Dependency} dependencies dependencies_len)
            string(REPLACE "+" ";" dependencies "${Dependency}")
            message("---> 计算传入路径末尾/右移部分: --> 库名: ${REALNAME} --> +部分: ${Dependency} --> 列表: ${dependencies} <-- ")


            message("---> 构建 ${REALNAME} -> ${REALNAME} ${_REALPATH_DIR}")
            
            spark_add_library_path(${REALNAME} ${_REALPATH_DIR})
            # target_link_qt5(${REALNAME}) # 使用依赖的依赖或许也不错
            
            target_include_directories(${REALNAME} PUBLIC ${_REALPATH_DIR})
            target_link_libraries(${REALNAME} ${dependencies})

        endif(RIGHT_PLUS LESS 0)
    endforeach(REALPATH IN LISTS REALPATHS)

endmacro(spark_add_library_realpaths)


# spark_add_source_paths
# 将指定路径中的文件变成可用的源文件列表
# 
macro(spark_add_source_paths SOURCE_VARIABLE_NAME)
    set(SOURCE_PATHS ${ARGN})
    set(${SOURCE_VARIABLE_NAME}_PATHS "")
    set(${SOURCE_VARIABLE_NAME} "")
    foreach(SOURCE_PATH IN LISTS SOURCE_PATHS)
        list(APPEND ${SOURCE_VARIABLE_NAME}_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_PATH})
        aux_source_directory(${SOURCE_PATH} _SOURCES)
        foreach(item IN LISTS _SOURCES)
            # message(" -> ${item}")
            list(APPEND ${SOURCE_VARIABLE_NAME} ${item})
        endforeach(item IN LISTS _SOURCES)

        # file(GLOB HEADER_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCE_PATH}/*.h)
        # foreach(item IN LISTS HEADER_LIST)
        #     # message(" -> ${item}")
        #     list(APPEND ${SOURCE_VARIABLE_NAME} ${item})
        # endforeach(item IN LISTS HEADER_LIST)
        
        file(GLOB UI_LIST RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${SOURCE_PATH}/*.ui)
        foreach(item IN LISTS UI_LIST)
            # message(" -> ${item}")
            list(APPEND ${SOURCE_VARIABLE_NAME} ${item})
        endforeach(item IN LISTS UI_LIST)
    endforeach(SOURCE_PATH IN LISTS SOURCE_PATHS)
endmacro(spark_add_source_paths SOURCE_VARIABLE_NAME)

