cmake_minimum_required(VERSION 3.29)
project(lab08)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ClientT)
add_subdirectory(ClientB)
add_subdirectory(RConsole)
add_subdirectory(ConcurrentServer)
add_subdirectory(ServiceLibrary)