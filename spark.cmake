# 提供给其它项目引用的共享资源(如果想通过框架构建引用的话）
if(FRAMEWORK_WANT_INCLUDE)

    set(FRAMEWORK_INCLUDE_EXPORTS      # 共享的引用目录
        ${CMAKE_CURRENT_LIST_DIR}/src/AdbManager/AdbUtils
    )
    set(FRAMEWORK_SOURCES_EXPORTS      # 共享的资源文件
        ${CMAKE_CURRENT_LIST_DIR}/src/AdbManager/AdbUtils/*
    )

    set(FRAMEWORK_WITHGIT_EXPORTS      # 共享的引用构建资源
        # 1. 引用了公共存储库
        # https://gitee.com/ndd-community/framework-plugin-component-library

        # 2. 引用了该项目提供的 - 间接引用 QFtp
        # https://gitee.com/spark-framework-libs/QFtpExample
        # https://gitee.com/spark-framework-libs/QFtp

        # 3. 引用了该项目提供的 Drag/Drop Eaters 与 EventUtil
        # https://gitee.com/zinface/notepad--plugin.image-view
    )

    # 特殊构建 - 为引用资源的目标配置预期所需依赖的构建
        # FRAMEWORK_PLUGIN 引用本资源的框架目标变量
    # find_package(Qt5Svg)
    # target_link_libraries(${FRAMEWORK_PLUGIN} Qt5::Svg)

    return()
endif(FRAMEWORK_WANT_INCLUDE)