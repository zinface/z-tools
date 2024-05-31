cmake_minimum_required(VERSION 3.15.0)

project(z-tools-filetransfer VERSION 1.1 LANGUAGES CXX)

include(cmake/SparkWithGit.cmake)

# 用于 spark_include 引入相关的列表项，每一项都是可引用的构建模块
spark_include(
    SparkEnvConfig.cmake
    SparkMacrosConfig.cmake
    SparkFindQt5Config.cmake
    # SparkFindQt6Config.cmake
    # SparkFindDtkConfig.cmake
    SparkFindLibraries.cmake
    SparkMacrosExtendConfig.cmake

#     SparkFramework.cmake
#     SparkCMakeInfoAfterConfig.cmake
#     SparkCMakeInfoBeforeConfig.cmake
#     SparkExternalProject.cmake

#     SparkTranslatorConfig.cmake
    SparkInstallMacrosConfig.cmake
    SparkBuildGraphviz.cmake
#     SparkDesktopMacros.cmake
#     SparkDebPackageConfig.cmake
#     SparkAppimageConfig.cmake
)

set(QRC_SOURCES "resources/images.qrc")

spark_add_executable_source(${PROJECT_NAME} ${QRC_SOURCES})

# 一行一目标
# spark_add_target_realpaths <tag> [realpaths]
# realpaths:
    # s: static (s:src/libs/hello)
    # d: shared (d:src/libs/say)
    # t: target (t:<target>:src+hello+say)
spark_add_target_realpaths(ALL
# 第一部分：
    # 构建出一个可维护 TcpSocket 的会话层
    s:Components/SessionManager
    # 构建一个在会话层之上的数据传输层，用于传输基本特征数据与通信管理
    s:Components/InfomationManager+SessionManager

# 第二部分：基于数据传输层的 文件传输核心、管理器、任务工作者(可变上传/下载角色)
    # 1. 基于数据传输构建出任务管理器与任务工作者
    s:Components/FileTransferManager/FileTaskManager+FileTasks+InfomationManager
    s:Components/FileTransferManager/FileTaskManager/FileTasks+FileTaskManager
    
    # 2. 文件传输核心 - 基于整个通信架构最主要的实现层
    s:Components/FileTransferManager+FileTaskManager
    
# 第三部分：基于 文件传输核心 的 UI 组件
    s:Components/FileTransferManager/ScanPort   # 一个简单且独立的端口扫描工具类

    # 1. 构建拥有 C/S 特性及不同身份的的 UI 组件
    s:Components/FileTransferManager/Widgets/FileSenderWidget+FileTransferManager
    s:Components/FileTransferManager/Widgets/FileReceiverWidget+FileTransferManager+ScanPort

    # 2. 构建基于 C/S 一体化的 文件传输核心的 GUI 图形主界面
    t:${PROJECT_NAME}:src+FileTransferManager+FileSenderWidget+FileReceiverWidget

# 第五部分：基于 文件传输核心 的 命令行 外壳
    # 1. 构建 FileCommands 命令行工具，支持超多参数配置、完整的 C/S 切换能力的的文件同步与传输的命令工具
    t:FileCommands:Components/FileTransferManager/FileCommands+FileTransferManager+ScanPort

    # 2. 构建 FileDirectory 命令行工具，支持超多参数配置、完整的 C/S 切换能力的的目录同步与传输的命令工具
    # s:Components/FileTransferManager/FileDirectory/DirUtils
    # t:FileDirectory:Components/FileTransferManager/FileDirectory+FileTransferManager+DirUtils
)


# Archive build control
set(BUILD_TYPE "${BUILD_TYPE}")
if(BUILD_TYPE STREQUAL "TGZ")
    include(cmake/BuildSourceTgz.cmake)
else()
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        include(cmake/BundleGenerater.cmake)
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
        # include(cmake/DebGenerater.cmake)
        spark_include(SparkDebPackageConfig.cmake)
        add_package_descript(cmake/spark-deb-package.descript)
        spark_install_program(/opt/zTools/bin
            ${PROJECT_NAME})
        spark_install_file(/opt/zTools/Components/resources/icon
            resources/z-Tools.svg)
        spark_install_file(/usr/share/applications
            resources/z-tools-filetransfer.desktop)
    endif()
endif()
