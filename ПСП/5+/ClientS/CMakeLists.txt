set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ClientS ClientS.cpp)
target_link_libraries(ClientS ws2_32)