cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    SessionManager/
    InfomationManager/
    FileTransferManager/
)

add_subdirectory(SessionManager)
add_subdirectory(InfomationManager)
add_subdirectory(FileTransferManager)
