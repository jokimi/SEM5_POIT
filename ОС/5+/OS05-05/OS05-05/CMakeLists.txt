cmake_minimum_required (VERSION 3.8)
project ("OS05-05" C)
add_executable (OS05-05 "OS05-05.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET OS05-05 PROPERTY C_STANDARD 11)
endif()