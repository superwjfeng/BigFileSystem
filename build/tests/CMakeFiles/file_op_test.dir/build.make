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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/wjfeng/LargeFileSystem

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/wjfeng/LargeFileSystem/build

# Include any dependencies generated for this target.
include tests/CMakeFiles/file_op_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include tests/CMakeFiles/file_op_test.dir/compiler_depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/file_op_test.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/file_op_test.dir/flags.make

tests/CMakeFiles/file_op_test.dir/file_op_test.cc.o: tests/CMakeFiles/file_op_test.dir/flags.make
tests/CMakeFiles/file_op_test.dir/file_op_test.cc.o: ../tests/file_op_test.cc
tests/CMakeFiles/file_op_test.dir/file_op_test.cc.o: tests/CMakeFiles/file_op_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wjfeng/LargeFileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object tests/CMakeFiles/file_op_test.dir/file_op_test.cc.o"
	cd /home/wjfeng/LargeFileSystem/build/tests && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/file_op_test.dir/file_op_test.cc.o -MF CMakeFiles/file_op_test.dir/file_op_test.cc.o.d -o CMakeFiles/file_op_test.dir/file_op_test.cc.o -c /home/wjfeng/LargeFileSystem/tests/file_op_test.cc

tests/CMakeFiles/file_op_test.dir/file_op_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/file_op_test.dir/file_op_test.cc.i"
	cd /home/wjfeng/LargeFileSystem/build/tests && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wjfeng/LargeFileSystem/tests/file_op_test.cc > CMakeFiles/file_op_test.dir/file_op_test.cc.i

tests/CMakeFiles/file_op_test.dir/file_op_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/file_op_test.dir/file_op_test.cc.s"
	cd /home/wjfeng/LargeFileSystem/build/tests && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wjfeng/LargeFileSystem/tests/file_op_test.cc -o CMakeFiles/file_op_test.dir/file_op_test.cc.s

tests/CMakeFiles/file_op_test.dir/__/src/file_op.cc.o: tests/CMakeFiles/file_op_test.dir/flags.make
tests/CMakeFiles/file_op_test.dir/__/src/file_op.cc.o: ../src/file_op.cc
tests/CMakeFiles/file_op_test.dir/__/src/file_op.cc.o: tests/CMakeFiles/file_op_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wjfeng/LargeFileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object tests/CMakeFiles/file_op_test.dir/__/src/file_op.cc.o"
	cd /home/wjfeng/LargeFileSystem/build/tests && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT tests/CMakeFiles/file_op_test.dir/__/src/file_op.cc.o -MF CMakeFiles/file_op_test.dir/__/src/file_op.cc.o.d -o CMakeFiles/file_op_test.dir/__/src/file_op.cc.o -c /home/wjfeng/LargeFileSystem/src/file_op.cc

tests/CMakeFiles/file_op_test.dir/__/src/file_op.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/file_op_test.dir/__/src/file_op.cc.i"
	cd /home/wjfeng/LargeFileSystem/build/tests && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wjfeng/LargeFileSystem/src/file_op.cc > CMakeFiles/file_op_test.dir/__/src/file_op.cc.i

tests/CMakeFiles/file_op_test.dir/__/src/file_op.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/file_op_test.dir/__/src/file_op.cc.s"
	cd /home/wjfeng/LargeFileSystem/build/tests && g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wjfeng/LargeFileSystem/src/file_op.cc -o CMakeFiles/file_op_test.dir/__/src/file_op.cc.s

# Object files for target file_op_test
file_op_test_OBJECTS = \
"CMakeFiles/file_op_test.dir/file_op_test.cc.o" \
"CMakeFiles/file_op_test.dir/__/src/file_op.cc.o"

# External object files for target file_op_test
file_op_test_EXTERNAL_OBJECTS =

../bin/file_op_test: tests/CMakeFiles/file_op_test.dir/file_op_test.cc.o
../bin/file_op_test: tests/CMakeFiles/file_op_test.dir/__/src/file_op.cc.o
../bin/file_op_test: tests/CMakeFiles/file_op_test.dir/build.make
../bin/file_op_test: tests/CMakeFiles/file_op_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wjfeng/LargeFileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable ../../bin/file_op_test"
	cd /home/wjfeng/LargeFileSystem/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/file_op_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/file_op_test.dir/build: ../bin/file_op_test
.PHONY : tests/CMakeFiles/file_op_test.dir/build

tests/CMakeFiles/file_op_test.dir/clean:
	cd /home/wjfeng/LargeFileSystem/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/file_op_test.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/file_op_test.dir/clean

tests/CMakeFiles/file_op_test.dir/depend:
	cd /home/wjfeng/LargeFileSystem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wjfeng/LargeFileSystem /home/wjfeng/LargeFileSystem/tests /home/wjfeng/LargeFileSystem/build /home/wjfeng/LargeFileSystem/build/tests /home/wjfeng/LargeFileSystem/build/tests/CMakeFiles/file_op_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/file_op_test.dir/depend

