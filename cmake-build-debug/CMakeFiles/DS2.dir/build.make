# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/nomi/Downloads/clion-2019.3.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/nomi/Downloads/clion-2019.3.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nomi/DS2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nomi/DS2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/DS2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/DS2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DS2.dir/flags.make

CMakeFiles/DS2.dir/main.cpp.o: CMakeFiles/DS2.dir/flags.make
CMakeFiles/DS2.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nomi/DS2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/DS2.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DS2.dir/main.cpp.o -c /home/nomi/DS2/main.cpp

CMakeFiles/DS2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DS2.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nomi/DS2/main.cpp > CMakeFiles/DS2.dir/main.cpp.i

CMakeFiles/DS2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DS2.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nomi/DS2/main.cpp -o CMakeFiles/DS2.dir/main.cpp.s

CMakeFiles/DS2.dir/unionFind.cpp.o: CMakeFiles/DS2.dir/flags.make
CMakeFiles/DS2.dir/unionFind.cpp.o: ../unionFind.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nomi/DS2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/DS2.dir/unionFind.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DS2.dir/unionFind.cpp.o -c /home/nomi/DS2/unionFind.cpp

CMakeFiles/DS2.dir/unionFind.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DS2.dir/unionFind.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nomi/DS2/unionFind.cpp > CMakeFiles/DS2.dir/unionFind.cpp.i

CMakeFiles/DS2.dir/unionFind.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DS2.dir/unionFind.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nomi/DS2/unionFind.cpp -o CMakeFiles/DS2.dir/unionFind.cpp.s

CMakeFiles/DS2.dir/hash.cpp.o: CMakeFiles/DS2.dir/flags.make
CMakeFiles/DS2.dir/hash.cpp.o: ../hash.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nomi/DS2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/DS2.dir/hash.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/DS2.dir/hash.cpp.o -c /home/nomi/DS2/hash.cpp

CMakeFiles/DS2.dir/hash.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/DS2.dir/hash.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nomi/DS2/hash.cpp > CMakeFiles/DS2.dir/hash.cpp.i

CMakeFiles/DS2.dir/hash.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/DS2.dir/hash.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nomi/DS2/hash.cpp -o CMakeFiles/DS2.dir/hash.cpp.s

# Object files for target DS2
DS2_OBJECTS = \
"CMakeFiles/DS2.dir/main.cpp.o" \
"CMakeFiles/DS2.dir/unionFind.cpp.o" \
"CMakeFiles/DS2.dir/hash.cpp.o"

# External object files for target DS2
DS2_EXTERNAL_OBJECTS =

DS2: CMakeFiles/DS2.dir/main.cpp.o
DS2: CMakeFiles/DS2.dir/unionFind.cpp.o
DS2: CMakeFiles/DS2.dir/hash.cpp.o
DS2: CMakeFiles/DS2.dir/build.make
DS2: CMakeFiles/DS2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nomi/DS2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable DS2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/DS2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/DS2.dir/build: DS2

.PHONY : CMakeFiles/DS2.dir/build

CMakeFiles/DS2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/DS2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/DS2.dir/clean

CMakeFiles/DS2.dir/depend:
	cd /home/nomi/DS2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nomi/DS2 /home/nomi/DS2 /home/nomi/DS2/cmake-build-debug /home/nomi/DS2/cmake-build-debug /home/nomi/DS2/cmake-build-debug/CMakeFiles/DS2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/DS2.dir/depend
