cmake_minimum_required(VERSION 3.0.0)

string(TIMESTAMP BUILD_TIME "%Y%m%d")

set(CPACK_GENERATOR "TGZ")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}_${PROJECT_VERSION}-${BUILD_TIME}")

# 子构建配置(组件管理)
install(FILES Components/CMakeLists.txt
    DESTINATION target/Components
)
# 项目组件(DesktopGenerater)
install(DIRECTORY Components/DesktopGenerater
    DESTINATION target/Components
)
# 资源目录
install(DIRECTORY resources
    DESTINATION target
)
# 可扩展构建模块
install(DIRECTORY cmake
    DESTINATION target
)
# 顶级构建配置
install(FILES  CMakeLists.txt
    DESTINATION target
)

# 项目组织代码
install(FILES
    main.cpp
    DESTINATION target
)
install(DIRECTORY
    src
    DESTINATION target
)

include(CPack)
