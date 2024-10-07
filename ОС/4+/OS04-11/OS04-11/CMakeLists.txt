cmake_minimum_required (VERSION 3.8)
project ("OS04-11" C)
add_executable (OS04-11 "OS04-11.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OS04-11 PROPERTY C_STANDARD 11)
endif()

find_package(Threads REQUIRED)
target_link_libraries(OS04-11 PRIVATE Threads::Threads)