cmake_minimum_required (VERSION 3.8)
project ("Windows" C)
add_executable (lab-05b "lab-05b.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab-05b PROPERTY C_STANDARD 11)
endif()