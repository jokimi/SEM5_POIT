﻿cmake_minimum_required (VERSION 3.8)
project ("lab-03a-client" C)
add_executable (lab-03a-client "lab-03a-client.c")
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET lab-03a-client PROPERTY C_STANDARD 11)
endif()