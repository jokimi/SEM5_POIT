cmake_minimum_required (VERSION 3.8)
project ("Linux" C)
add_executable (lab-06d "lab-06d.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab-06d PROPERTY C_STANDARD 11)
endif()