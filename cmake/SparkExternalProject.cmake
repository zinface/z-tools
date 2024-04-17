# SparkExternalProject.cmake
    # 项目内插件构建的内建支持模块

    # 当前可适用于 Linux 平台
    
include(ExternalProject)

# spark_add_external_git_cmake_project
    # 自定义宏：构建一个基于 git 仓库的外部 CMake 项目
        # 如果是其它的构建形式，请使用原生 ExternalProject_Add 进行自定义
    # 要求参数：
        # prj       项目名称 - 建议名称尾部增加 _git 以用于标识
        # prj_git   项目仓库地址
        
        # prj_cfg   额外的配置内容，不使用时要求传入 "" （空白占位)
        # prj_bld   额外的构建内容，不使用时要求传入 "" （空白占位)
        # prj_inl   额外的安装内容，不使用时要求传入 "" （空白占位)
    
        # ARGN 其它配置，用于对您的 ExternalProject_Add 添加更多的定制

        # 在完成配置后，将会提供新的变量用于后续使用
            # ${prj}-source     该项目的源目录
            # ${prj}-build      该项目的构建目录
            # ${prj}-install    该项目的安装目录

    # 额外部分要求：
        # 仅支持单个参数 ""
        # 如果要对额外内容添加多个，请使用 "A;B;C"
        
        # 例如：对 prj_cfg 配置一个构建模式的参数
            # "-DCMAKE_BUILD_TYPE=Release"
            # "-DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=\${prj_install}"
            # 说明: \${prj_install} 表示先将 ${prj_install} 传入到指令
                # 而不直接展开，因为外库没有 prj_install 值，仅在内部有。
                # 第一次不进行展开，在第二次时，也就是在指令中，将会展开 ${prj_install} 的值
        
        # 例如：对 prj_bld 配置多个参数
            # "--config Release -- -j12"
            # 或者使用 ';' 来区分多个参数 "--config;Release;--;-j12"
            # 使用 ';' 会在 cmake 中认为这是一个列表，在展开的时候将会进行增加空白分隔

    # ARGN 其它配置部分
        # 在完成额外配置后，还可以对 ExternalProject_Add 添加其它有关的配置
function(spark_add_external_git_cmake_project prj prj_git prj_cfg prj_bld prj_inl)
    set(prefix      "3rlib")
    set(prj_source  "3rlib/src/${prj}")
    set(prj_build   "3rlib/src/${prj}-build")
    set(prj_install "3rlib/src/${prj}-install")

    set(prj_source      "${CMAKE_CURRENT_BINARY_DIR}/${prj_source}")
    set(prj_build       "${CMAKE_CURRENT_BINARY_DIR}/${prj_build}")
    set(prj_install     "${CMAKE_CURRENT_BINARY_DIR}/${prj_install}")
    
    include(ExternalProject)
    ExternalProject_Add(${prj}
        PREFIX          ${prefix}
        GIT_REPOSITORY  ${prj_git}

        CONFIGURE_COMMAND ${CMAKE_COMMAND} -S ${prj_source} -B ${prj_build} ${prj_cfg}
        BUILD_COMMAND     ${CMAKE_COMMAND} --build   ${prj_build}           ${prj_bld}
        INSTALL_COMMAND   ${CMAKE_COMMAND} --install ${prj_build} 
                                           --prefix  ${prj_install}         ${prj_inl}
        ${ARGN}
    )

    set(${prj}-source  "${prj_source}"  PARENT_SCOPE)
    set(${prj}-build   "${prj_build}"   PARENT_SCOPE)
    set(${prj}-install "${prj_install}" PARENT_SCOPE)
endfunction(spark_add_external_git_cmake_project prj prj_git prj_cfg prj_bld prj_inl)


# spark_add_external_git_cmake_project(lxqt-build-tool
#     https://www.gitlink.org.cn/zinface/lxqt-build-tools.git
#         "" # 默认配置
#         "" # 默认编译
#         "" # 默认安装
#         GIT_SHALLOW TRUE)

# spark_add_external_git_cmake_project(qtermwidget_git
#     https://www.gitlink.org.cn/zinface/qtermwidget.git
#          # 在生成前的配置参数
#         "-DCMAKE_PREFIX_PATH=${lxqt-build-tool-install};-DCMAKE_INSTALL_PREFIX=\${prj_install}"
#          # 在生成时的配置参数
#         "--;-j12"
#          # 在安装时的配置参数
#         ""
#         GIT_SHALLOW TRUE)
