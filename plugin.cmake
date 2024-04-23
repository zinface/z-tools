# 提供给其它插件项目引用的共享资源(如果其它插件想通过框架构建引用的话）
if(FRAMEWORK_WANT_INCLUDE)
    message("-- [PLUGIN EXPORT]: FRAMEWORK_WANT_INCLUDE?")

    if(feature_package)
        list(APPEND FRAMEWORK_INCLUDE_EXPORTS      # 共享的引用目录 - 用于 include
            ${CMAKE_CURRENT_LIST_DIR}/Components/InfomationManager
        )
        list(APPEND FRAMEWORK_SOURCES_EXPORTS      # 共享的资源文件 - 用于 构建
            ${CMAKE_CURRENT_LIST_DIR}/Components/InfomationManager/package*
        )
    endif(feature_package)

    # 特殊构建 - 为引用资源的目标配置预期所需依赖的构建
        # FRAMEWORK_PLUGIN 引用本资源的框架目标变量
    # find_package(Qt5Svg)
    # target_link_libraries(${FRAMEWORK_PLUGIN} Qt5::Svg)

    return()
endif(FRAMEWORK_WANT_INCLUDE)