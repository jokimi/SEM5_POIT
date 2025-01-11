cmake_minimum_required (VERSION 3.8)
project ("Windows" C)
add_executable (service-ctl "service-ctl.c")

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET service-ctl PROPERTY C_STANDARD 11)
endif()

target_link_libraries(service-ctl PRIVATE ws2_32)
target_link_libraries(service-ctl PRIVATE Shlwapi)