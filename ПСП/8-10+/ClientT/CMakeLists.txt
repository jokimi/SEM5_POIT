set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ClientT ClientT.cpp Errors.cpp)
target_link_libraries(ClientT ws2_32)
add_executable(ClientT2 ClientT.cpp Errors.cpp)
target_link_libraries(ClientT2 ws2_32)