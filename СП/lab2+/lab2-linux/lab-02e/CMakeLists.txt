cmake_minimum_required (VERSION 3.8)
project ("lab2-linux" C)
add_executable (lab-02e "lab-02e.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab-02e PROPERTY C_STANDARD 11)
endif()