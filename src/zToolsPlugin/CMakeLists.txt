cmake_minimum_required(VERSION 3.5.1)

find_package(zTools)

if(zTools_FOUND)

    add_tools_plugin_source_paths(BaseApkView
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_SOURCE_DIR}/src/ApkManager
        ${CMAKE_SOURCE_DIR}/src/ApkManager/aapt
    )
    # target_link_qt5(BaseApkView)

endif(zTools_FOUND)


# 这是为 zToolsBox 自动构建扩展的模块