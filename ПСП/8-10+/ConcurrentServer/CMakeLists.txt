set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ConcurrentServer ConcurrentServer.cpp Errors.cpp)
target_link_libraries(ConcurrentServer ws2_32)

target_link_libraries(ConcurrentServer ServiceLibrary)
