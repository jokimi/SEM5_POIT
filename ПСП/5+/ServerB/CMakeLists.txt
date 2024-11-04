set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ServerB ServerB.cpp)
target_link_libraries(ServerB ws2_32)
add_executable(server server.cpp)
target_link_libraries(server ws2_32)
