cmake_minimum_required (VERSION 3.8)
project ("Linux" C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_COMPILER clang)

add_library(mapping-s STATIC mapping-s.c)
target_include_directories(mapping-s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(lab-08c lab-08c.c)
target_link_libraries(lab-08c PRIVATE mapping-s)