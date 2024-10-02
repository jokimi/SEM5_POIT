cmake_minimum_required(VERSION 3.8)

project ("OS-03-05" C)

add_executable(OS-03-05 OS-03-05.c)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OS-03-05 PROPERTY C_STANDARD 11)
endif()