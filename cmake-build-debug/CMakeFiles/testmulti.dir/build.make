# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /snap/clion/137/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/137/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/data/data_structure_homework

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/data/data_structure_homework/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/testmulti.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/testmulti.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/testmulti.dir/flags.make

CMakeFiles/testmulti.dir/test/test.cpp.o: CMakeFiles/testmulti.dir/flags.make
CMakeFiles/testmulti.dir/test/test.cpp.o: ../test/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/data/data_structure_homework/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/testmulti.dir/test/test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/testmulti.dir/test/test.cpp.o -c /home/data/data_structure_homework/test/test.cpp

CMakeFiles/testmulti.dir/test/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/testmulti.dir/test/test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/data/data_structure_homework/test/test.cpp > CMakeFiles/testmulti.dir/test/test.cpp.i

CMakeFiles/testmulti.dir/test/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/testmulti.dir/test/test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/data/data_structure_homework/test/test.cpp -o CMakeFiles/testmulti.dir/test/test.cpp.s

# Object files for target testmulti
testmulti_OBJECTS = \
"CMakeFiles/testmulti.dir/test/test.cpp.o"

# External object files for target testmulti
testmulti_EXTERNAL_OBJECTS =

../bin/testmulti: CMakeFiles/testmulti.dir/test/test.cpp.o
../bin/testmulti: CMakeFiles/testmulti.dir/build.make
../bin/testmulti: lib/libgtestd.a
../bin/testmulti: CMakeFiles/testmulti.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/data/data_structure_homework/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/testmulti"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/testmulti.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/testmulti.dir/build: ../bin/testmulti

.PHONY : CMakeFiles/testmulti.dir/build

CMakeFiles/testmulti.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/testmulti.dir/cmake_clean.cmake
.PHONY : CMakeFiles/testmulti.dir/clean

CMakeFiles/testmulti.dir/depend:
	cd /home/data/data_structure_homework/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/data/data_structure_homework /home/data/data_structure_homework /home/data/data_structure_homework/cmake-build-debug /home/data/data_structure_homework/cmake-build-debug /home/data/data_structure_homework/cmake-build-debug/CMakeFiles/testmulti.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/testmulti.dir/depend
