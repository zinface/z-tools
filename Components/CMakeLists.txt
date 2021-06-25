cmake_minimum_required(VERSION 3.0.0)

include_directories(
    SessionManager/
    InfomationManager/
    RemoteAptManager/
)

add_subdirectory(SessionManager)
add_subdirectory(InfomationManager)
add_subdirectory(RemoteAptManager)
