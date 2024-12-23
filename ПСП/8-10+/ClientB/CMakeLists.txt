set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ClientB ClientB.cpp Errors.cpp)
target_link_libraries(ClientB ws2_32)