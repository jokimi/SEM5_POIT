cmake_minimum_required (VERSION 3.8)
project ("Windows" C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lab-08b/bin)

if(NOT WIN32)
    message(FATAL_ERROR "This project is designed for Windows.")
endif()

add_library(mapping-d SHARED mapping-d.c)
target_sources(mapping-d PRIVATE mapping-d.def)

add_library(mapping-d2 SHARED mapping-d2.c)
target_compile_definitions(mapping-d2 PRIVATE "EXPORTING")

add_executable(lab-08b lab-08b.c)

target_link_libraries(lab-08b PRIVATE mapping-d mapping-d2)
target_include_directories(lab-08b PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})