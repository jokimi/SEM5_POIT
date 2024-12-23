set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(RConsole RConsole.cpp Errors.cpp)
target_link_libraries(RConsole ws2_32)