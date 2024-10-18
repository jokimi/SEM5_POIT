cmake_minimum_required (VERSION 3.8)
project ("lab2" C)
add_executable (lab-02c "lab-02c.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab-02c PROPERTY C_STANDARD 11)
endif()