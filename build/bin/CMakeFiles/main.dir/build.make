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
include bin/CMakeFiles/main.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include bin/CMakeFiles/main.dir/compiler_depend.make

# Include the progress variables for this target.
include bin/CMakeFiles/main.dir/progress.make

# Include the compile flags for this target's objects.
include bin/CMakeFiles/main.dir/flags.make

bin/CMakeFiles/main.dir/main.cc.o: bin/CMakeFiles/main.dir/flags.make
bin/CMakeFiles/main.dir/main.cc.o: ../src/main.cc
bin/CMakeFiles/main.dir/main.cc.o: bin/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wjfeng/LargeFileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object bin/CMakeFiles/main.dir/main.cc.o"
	cd /home/wjfeng/LargeFileSystem/build/bin && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT bin/CMakeFiles/main.dir/main.cc.o -MF CMakeFiles/main.dir/main.cc.o.d -o CMakeFiles/main.dir/main.cc.o -c /home/wjfeng/LargeFileSystem/src/main.cc

bin/CMakeFiles/main.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/main.cc.i"
	cd /home/wjfeng/LargeFileSystem/build/bin && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wjfeng/LargeFileSystem/src/main.cc > CMakeFiles/main.dir/main.cc.i

bin/CMakeFiles/main.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/main.cc.s"
	cd /home/wjfeng/LargeFileSystem/build/bin && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wjfeng/LargeFileSystem/src/main.cc -o CMakeFiles/main.dir/main.cc.s

bin/CMakeFiles/main.dir/mmap_file.cc.o: bin/CMakeFiles/main.dir/flags.make
bin/CMakeFiles/main.dir/mmap_file.cc.o: ../src/mmap_file.cc
bin/CMakeFiles/main.dir/mmap_file.cc.o: bin/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wjfeng/LargeFileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object bin/CMakeFiles/main.dir/mmap_file.cc.o"
	cd /home/wjfeng/LargeFileSystem/build/bin && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT bin/CMakeFiles/main.dir/mmap_file.cc.o -MF CMakeFiles/main.dir/mmap_file.cc.o.d -o CMakeFiles/main.dir/mmap_file.cc.o -c /home/wjfeng/LargeFileSystem/src/mmap_file.cc

bin/CMakeFiles/main.dir/mmap_file.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/mmap_file.cc.i"
	cd /home/wjfeng/LargeFileSystem/build/bin && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wjfeng/LargeFileSystem/src/mmap_file.cc > CMakeFiles/main.dir/mmap_file.cc.i

bin/CMakeFiles/main.dir/mmap_file.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/mmap_file.cc.s"
	cd /home/wjfeng/LargeFileSystem/build/bin && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wjfeng/LargeFileSystem/src/mmap_file.cc -o CMakeFiles/main.dir/mmap_file.cc.s

bin/CMakeFiles/main.dir/file_op.cc.o: bin/CMakeFiles/main.dir/flags.make
bin/CMakeFiles/main.dir/file_op.cc.o: ../src/file_op.cc
bin/CMakeFiles/main.dir/file_op.cc.o: bin/CMakeFiles/main.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/wjfeng/LargeFileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object bin/CMakeFiles/main.dir/file_op.cc.o"
	cd /home/wjfeng/LargeFileSystem/build/bin && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT bin/CMakeFiles/main.dir/file_op.cc.o -MF CMakeFiles/main.dir/file_op.cc.o.d -o CMakeFiles/main.dir/file_op.cc.o -c /home/wjfeng/LargeFileSystem/src/file_op.cc

bin/CMakeFiles/main.dir/file_op.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/main.dir/file_op.cc.i"
	cd /home/wjfeng/LargeFileSystem/build/bin && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/wjfeng/LargeFileSystem/src/file_op.cc > CMakeFiles/main.dir/file_op.cc.i

bin/CMakeFiles/main.dir/file_op.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/main.dir/file_op.cc.s"
	cd /home/wjfeng/LargeFileSystem/build/bin && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/wjfeng/LargeFileSystem/src/file_op.cc -o CMakeFiles/main.dir/file_op.cc.s

# Object files for target main
main_OBJECTS = \
"CMakeFiles/main.dir/main.cc.o" \
"CMakeFiles/main.dir/mmap_file.cc.o" \
"CMakeFiles/main.dir/file_op.cc.o"

# External object files for target main
main_EXTERNAL_OBJECTS =

bin/main: bin/CMakeFiles/main.dir/main.cc.o
bin/main: bin/CMakeFiles/main.dir/mmap_file.cc.o
bin/main: bin/CMakeFiles/main.dir/file_op.cc.o
bin/main: bin/CMakeFiles/main.dir/build.make
bin/main: bin/CMakeFiles/main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/wjfeng/LargeFileSystem/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable main"
	cd /home/wjfeng/LargeFileSystem/build/bin && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bin/CMakeFiles/main.dir/build: bin/main
.PHONY : bin/CMakeFiles/main.dir/build

bin/CMakeFiles/main.dir/clean:
	cd /home/wjfeng/LargeFileSystem/build/bin && $(CMAKE_COMMAND) -P CMakeFiles/main.dir/cmake_clean.cmake
.PHONY : bin/CMakeFiles/main.dir/clean

bin/CMakeFiles/main.dir/depend:
	cd /home/wjfeng/LargeFileSystem/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/wjfeng/LargeFileSystem /home/wjfeng/LargeFileSystem/src /home/wjfeng/LargeFileSystem/build /home/wjfeng/LargeFileSystem/build/bin /home/wjfeng/LargeFileSystem/build/bin/CMakeFiles/main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bin/CMakeFiles/main.dir/depend

