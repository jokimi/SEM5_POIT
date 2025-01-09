cmake_minimum_required (VERSION 3.8)
project ("Windows" C)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -O2")

add_library(mapping-s STATIC mapping-s.c)
target_include_directories(mapping-s PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(lab-08a lab-08a.c)

target_link_libraries(lab-08a mapping-s)