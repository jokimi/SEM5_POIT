cmake_minimum_required (VERSION 3.8)
project ("Windows" C)
add_executable (lab-06a "lab-06a.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab-06a PROPERTY C_STANDARD 11)
endif()

if(WIN32)
    target_link_libraries(lab-06a Shlwapi)
endif()