# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/kirill/CLionProjects/UnorderedMap

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kirill/CLionProjects/UnorderedMap/build

# Include any dependencies generated for this target.
include CMakeFiles/unordered_map.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/unordered_map.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/unordered_map.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/unordered_map.dir/flags.make

CMakeFiles/unordered_map.dir/test.cpp.o: CMakeFiles/unordered_map.dir/flags.make
CMakeFiles/unordered_map.dir/test.cpp.o: /home/kirill/CLionProjects/UnorderedMap/test.cpp
CMakeFiles/unordered_map.dir/test.cpp.o: CMakeFiles/unordered_map.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/kirill/CLionProjects/UnorderedMap/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/unordered_map.dir/test.cpp.o"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/unordered_map.dir/test.cpp.o -MF CMakeFiles/unordered_map.dir/test.cpp.o.d -o CMakeFiles/unordered_map.dir/test.cpp.o -c /home/kirill/CLionProjects/UnorderedMap/test.cpp

CMakeFiles/unordered_map.dir/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/unordered_map.dir/test.cpp.i"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kirill/CLionProjects/UnorderedMap/test.cpp > CMakeFiles/unordered_map.dir/test.cpp.i

CMakeFiles/unordered_map.dir/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/unordered_map.dir/test.cpp.s"
	/usr/bin/clang++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kirill/CLionProjects/UnorderedMap/test.cpp -o CMakeFiles/unordered_map.dir/test.cpp.s

# Object files for target unordered_map
unordered_map_OBJECTS = \
"CMakeFiles/unordered_map.dir/test.cpp.o"

# External object files for target unordered_map
unordered_map_EXTERNAL_OBJECTS =

unordered_map: CMakeFiles/unordered_map.dir/test.cpp.o
unordered_map: CMakeFiles/unordered_map.dir/build.make
unordered_map: CMakeFiles/unordered_map.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/kirill/CLionProjects/UnorderedMap/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable unordered_map"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/unordered_map.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/unordered_map.dir/build: unordered_map
.PHONY : CMakeFiles/unordered_map.dir/build

CMakeFiles/unordered_map.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/unordered_map.dir/cmake_clean.cmake
.PHONY : CMakeFiles/unordered_map.dir/clean

CMakeFiles/unordered_map.dir/depend:
	cd /home/kirill/CLionProjects/UnorderedMap/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kirill/CLionProjects/UnorderedMap /home/kirill/CLionProjects/UnorderedMap /home/kirill/CLionProjects/UnorderedMap/build /home/kirill/CLionProjects/UnorderedMap/build /home/kirill/CLionProjects/UnorderedMap/build/CMakeFiles/unordered_map.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/unordered_map.dir/depend

