cmake_minimum_required (VERSION 3.8)
project ("lab7" C)
add_executable (lab-07a "lab-07a.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab-07a PROPERTY CXX_STANDARD 20)
endif()