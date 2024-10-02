cmake_minimum_required(VERSION 3.8)

project ("OS-03-06" C)

add_executable(OS-03-06 OS-03-06.c)
add_executable(OS-03-06-1 OS-03-06-1.c)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OS-03-06 PROPERTY C_STANDARD 11)
  set_property(TARGET OS-03-06-1 PROPERTY C_STANDARD 11)
endif()