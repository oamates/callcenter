# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.6

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/honglt/MyProject/Platform/UnitTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/honglt/MyProject/Platform/UnitTest

# Include any dependencies generated for this target.
include CMakeFiles/UnitTest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/UnitTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/UnitTest.dir/flags.make

CMakeFiles/UnitTest.dir/TestCase.cpp.o: CMakeFiles/UnitTest.dir/flags.make
CMakeFiles/UnitTest.dir/TestCase.cpp.o: TestCase.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/honglt/MyProject/Platform/UnitTest/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/UnitTest.dir/TestCase.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/UnitTest.dir/TestCase.cpp.o -c /home/honglt/MyProject/Platform/UnitTest/TestCase.cpp

CMakeFiles/UnitTest.dir/TestCase.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/UnitTest.dir/TestCase.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/honglt/MyProject/Platform/UnitTest/TestCase.cpp > CMakeFiles/UnitTest.dir/TestCase.cpp.i

CMakeFiles/UnitTest.dir/TestCase.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/UnitTest.dir/TestCase.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/honglt/MyProject/Platform/UnitTest/TestCase.cpp -o CMakeFiles/UnitTest.dir/TestCase.cpp.s

CMakeFiles/UnitTest.dir/TestCase.cpp.o.requires:
.PHONY : CMakeFiles/UnitTest.dir/TestCase.cpp.o.requires

CMakeFiles/UnitTest.dir/TestCase.cpp.o.provides: CMakeFiles/UnitTest.dir/TestCase.cpp.o.requires
	$(MAKE) -f CMakeFiles/UnitTest.dir/build.make CMakeFiles/UnitTest.dir/TestCase.cpp.o.provides.build
.PHONY : CMakeFiles/UnitTest.dir/TestCase.cpp.o.provides

CMakeFiles/UnitTest.dir/TestCase.cpp.o.provides.build: CMakeFiles/UnitTest.dir/TestCase.cpp.o
.PHONY : CMakeFiles/UnitTest.dir/TestCase.cpp.o.provides.build

# Object files for target UnitTest
UnitTest_OBJECTS = \
"CMakeFiles/UnitTest.dir/TestCase.cpp.o"

# External object files for target UnitTest
UnitTest_EXTERNAL_OBJECTS =

/home/honglt/MyProject/Platform/Bin/UnitTest: CMakeFiles/UnitTest.dir/TestCase.cpp.o
/home/honglt/MyProject/Platform/Bin/UnitTest: CMakeFiles/UnitTest.dir/build.make
/home/honglt/MyProject/Platform/Bin/UnitTest: CMakeFiles/UnitTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable /home/honglt/MyProject/Platform/Bin/UnitTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/UnitTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/UnitTest.dir/build: /home/honglt/MyProject/Platform/Bin/UnitTest
.PHONY : CMakeFiles/UnitTest.dir/build

CMakeFiles/UnitTest.dir/requires: CMakeFiles/UnitTest.dir/TestCase.cpp.o.requires
.PHONY : CMakeFiles/UnitTest.dir/requires

CMakeFiles/UnitTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/UnitTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/UnitTest.dir/clean

CMakeFiles/UnitTest.dir/depend:
	cd /home/honglt/MyProject/Platform/UnitTest && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/honglt/MyProject/Platform/UnitTest /home/honglt/MyProject/Platform/UnitTest /home/honglt/MyProject/Platform/UnitTest /home/honglt/MyProject/Platform/UnitTest /home/honglt/MyProject/Platform/UnitTest/CMakeFiles/UnitTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/UnitTest.dir/depend

