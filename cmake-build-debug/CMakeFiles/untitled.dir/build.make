# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/189/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/189/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/martyna/Pulpit/SYSOPY

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/martyna/Pulpit/SYSOPY/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/untitled.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/untitled.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/untitled.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/untitled.dir/flags.make

CMakeFiles/untitled.dir/lab6/zad2/client.c.o: CMakeFiles/untitled.dir/flags.make
CMakeFiles/untitled.dir/lab6/zad2/client.c.o: ../lab6/zad2/client.c
CMakeFiles/untitled.dir/lab6/zad2/client.c.o: CMakeFiles/untitled.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martyna/Pulpit/SYSOPY/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/untitled.dir/lab6/zad2/client.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/untitled.dir/lab6/zad2/client.c.o -MF CMakeFiles/untitled.dir/lab6/zad2/client.c.o.d -o CMakeFiles/untitled.dir/lab6/zad2/client.c.o -c /home/martyna/Pulpit/SYSOPY/lab6/zad2/client.c

CMakeFiles/untitled.dir/lab6/zad2/client.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/untitled.dir/lab6/zad2/client.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/martyna/Pulpit/SYSOPY/lab6/zad2/client.c > CMakeFiles/untitled.dir/lab6/zad2/client.c.i

CMakeFiles/untitled.dir/lab6/zad2/client.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/untitled.dir/lab6/zad2/client.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/martyna/Pulpit/SYSOPY/lab6/zad2/client.c -o CMakeFiles/untitled.dir/lab6/zad2/client.c.s

CMakeFiles/untitled.dir/lab6/zad2/server.c.o: CMakeFiles/untitled.dir/flags.make
CMakeFiles/untitled.dir/lab6/zad2/server.c.o: ../lab6/zad2/server.c
CMakeFiles/untitled.dir/lab6/zad2/server.c.o: CMakeFiles/untitled.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/martyna/Pulpit/SYSOPY/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/untitled.dir/lab6/zad2/server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/untitled.dir/lab6/zad2/server.c.o -MF CMakeFiles/untitled.dir/lab6/zad2/server.c.o.d -o CMakeFiles/untitled.dir/lab6/zad2/server.c.o -c /home/martyna/Pulpit/SYSOPY/lab6/zad2/server.c

CMakeFiles/untitled.dir/lab6/zad2/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/untitled.dir/lab6/zad2/server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/martyna/Pulpit/SYSOPY/lab6/zad2/server.c > CMakeFiles/untitled.dir/lab6/zad2/server.c.i

CMakeFiles/untitled.dir/lab6/zad2/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/untitled.dir/lab6/zad2/server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/martyna/Pulpit/SYSOPY/lab6/zad2/server.c -o CMakeFiles/untitled.dir/lab6/zad2/server.c.s

# Object files for target untitled
untitled_OBJECTS = \
"CMakeFiles/untitled.dir/lab6/zad2/client.c.o" \
"CMakeFiles/untitled.dir/lab6/zad2/server.c.o"

# External object files for target untitled
untitled_EXTERNAL_OBJECTS =

untitled: CMakeFiles/untitled.dir/lab6/zad2/client.c.o
untitled: CMakeFiles/untitled.dir/lab6/zad2/server.c.o
untitled: CMakeFiles/untitled.dir/build.make
untitled: CMakeFiles/untitled.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/martyna/Pulpit/SYSOPY/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable untitled"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/untitled.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/untitled.dir/build: untitled
.PHONY : CMakeFiles/untitled.dir/build

CMakeFiles/untitled.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/untitled.dir/cmake_clean.cmake
.PHONY : CMakeFiles/untitled.dir/clean

CMakeFiles/untitled.dir/depend:
	cd /home/martyna/Pulpit/SYSOPY/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/martyna/Pulpit/SYSOPY /home/martyna/Pulpit/SYSOPY /home/martyna/Pulpit/SYSOPY/cmake-build-debug /home/martyna/Pulpit/SYSOPY/cmake-build-debug /home/martyna/Pulpit/SYSOPY/cmake-build-debug/CMakeFiles/untitled.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/untitled.dir/depend
