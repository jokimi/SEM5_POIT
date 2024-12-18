cmake_minimum_required (VERSION 3.8)
project ("Linux" C)
add_executable (lab-06e "lab-06e.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab-06e PROPERTY C_STANDARD 11)
endif()