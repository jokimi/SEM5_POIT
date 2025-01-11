cmake_minimum_required (VERSION 3.8)
project ("Windows" C)
add_executable (service-09 "service-09.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET service-09 PROPERTY C_STANDARD 11)
endif()

target_link_libraries(service-09 PRIVATE ws2_32)
target_link_libraries(service-09 PRIVATE Shlwapi)