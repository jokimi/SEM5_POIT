# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build"

# Include any dependencies generated for this target.
include OS04-11/CMakeFiles/OS04-11.dir/depend.make

# Include the progress variables for this target.
include OS04-11/CMakeFiles/OS04-11.dir/progress.make

# Include the compile flags for this target's objects.
include OS04-11/CMakeFiles/OS04-11.dir/flags.make

OS04-11/CMakeFiles/OS04-11.dir/OS04-11.c.o: OS04-11/CMakeFiles/OS04-11.dir/flags.make
OS04-11/CMakeFiles/OS04-11.dir/OS04-11.c.o: ../OS04-11/OS04-11.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object OS04-11/CMakeFiles/OS04-11.dir/OS04-11.c.o"
	cd "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build/OS04-11" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/OS04-11.dir/OS04-11.c.o   -c "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/OS04-11/OS04-11.c"

OS04-11/CMakeFiles/OS04-11.dir/OS04-11.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/OS04-11.dir/OS04-11.c.i"
	cd "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build/OS04-11" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/OS04-11/OS04-11.c" > CMakeFiles/OS04-11.dir/OS04-11.c.i

OS04-11/CMakeFiles/OS04-11.dir/OS04-11.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/OS04-11.dir/OS04-11.c.s"
	cd "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build/OS04-11" && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/OS04-11/OS04-11.c" -o CMakeFiles/OS04-11.dir/OS04-11.c.s

# Object files for target OS04-11
OS04__11_OBJECTS = \
"CMakeFiles/OS04-11.dir/OS04-11.c.o"

# External object files for target OS04-11
OS04__11_EXTERNAL_OBJECTS =

OS04-11/OS04-11: OS04-11/CMakeFiles/OS04-11.dir/OS04-11.c.o
OS04-11/OS04-11: OS04-11/CMakeFiles/OS04-11.dir/build.make
OS04-11/OS04-11: OS04-11/CMakeFiles/OS04-11.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable OS04-11"
	cd "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build/OS04-11" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/OS04-11.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
OS04-11/CMakeFiles/OS04-11.dir/build: OS04-11/OS04-11

.PHONY : OS04-11/CMakeFiles/OS04-11.dir/build

OS04-11/CMakeFiles/OS04-11.dir/clean:
	cd "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build/OS04-11" && $(CMAKE_COMMAND) -P CMakeFiles/OS04-11.dir/cmake_clean.cmake
.PHONY : OS04-11/CMakeFiles/OS04-11.dir/clean

OS04-11/CMakeFiles/OS04-11.dir/depend:
	cd "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11" "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/OS04-11" "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build" "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build/OS04-11" "/mnt/d/bstu/5 sem/ос/лабы/4/os04-11/build/OS04-11/CMakeFiles/OS04-11.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : OS04-11/CMakeFiles/OS04-11.dir/depend

