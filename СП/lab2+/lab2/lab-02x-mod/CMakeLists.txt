cmake_minimum_required (VERSION 3.8)
project ("lab2" C)
add_executable (lab-02x-mod "lab-02x-mod.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab-02x-mod PROPERTY C_STANDARD 11)
endif()