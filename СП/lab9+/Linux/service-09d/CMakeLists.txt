cmake_minimum_required (VERSION 3.8)
project ("Linux" C)

# Устанавливаем флаги компилятора для Clang (по умолчанию)
set(CMAKE_C_FLAGS "-Wall -std=c11")

# Указываем исходные файлы проекта
set(SOURCE_FILES service-09d.c)

add_compile_definitions(_POSIX_C_SOURCE=200809L)

# Создаем исполнимый файл
add_executable(service-09d ${SOURCE_FILES})

# Указываем нужные библиотеки (например, pthread для многозадачности)
target_link_libraries(service-09d pthread)

# Указываем, где должны располагаться итоговые файлы
install(TARGETS service-09d DESTINATION /usr/sbin/service-09d)

# Устанавливаем конфигурационные файлы
install(FILES service-09d.conf DESTINATION /etc)