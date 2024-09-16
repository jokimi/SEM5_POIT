cmake_minimum_required(VERSION 3.8)

project("lab1" C)
include_directories("include")

add_executable(translate src/main.c src/translate_array.c src/translate_pointer.c)
add_executable(test_translate tests/test_translate.c src/translate_array.c src/translate_pointer.c)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET translate PROPERTY C_STANDARD 11)
  set_property(TARGET test_translate PROPERTY C_STANDARD 11)
endif()