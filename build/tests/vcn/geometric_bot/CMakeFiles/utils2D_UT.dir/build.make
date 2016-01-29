# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/victor/vcn_bots

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/victor/vcn_bots/build

# Include any dependencies generated for this target.
include tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/depend.make

# Include the progress variables for this target.
include tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/progress.make

# Include the compile flags for this target's objects.
include tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/flags.make

tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o: tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/flags.make
tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o: ../tests/vcn/geometric_bot/utils2D_UT.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/victor/vcn_bots/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o"
	cd /home/victor/vcn_bots/build/tests/vcn/geometric_bot && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o   -c /home/victor/vcn_bots/tests/vcn/geometric_bot/utils2D_UT.c

tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/utils2D_UT.dir/utils2D_UT.c.i"
	cd /home/victor/vcn_bots/build/tests/vcn/geometric_bot && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/victor/vcn_bots/tests/vcn/geometric_bot/utils2D_UT.c > CMakeFiles/utils2D_UT.dir/utils2D_UT.c.i

tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/utils2D_UT.dir/utils2D_UT.c.s"
	cd /home/victor/vcn_bots/build/tests/vcn/geometric_bot && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/victor/vcn_bots/tests/vcn/geometric_bot/utils2D_UT.c -o CMakeFiles/utils2D_UT.dir/utils2D_UT.c.s

tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o.requires:
.PHONY : tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o.requires

tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o.provides: tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o.requires
	$(MAKE) -f tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/build.make tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o.provides.build
.PHONY : tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o.provides

tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o.provides.build: tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o

# Object files for target utils2D_UT
utils2D_UT_OBJECTS = \
"CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o"

# External object files for target utils2D_UT
utils2D_UT_EXTERNAL_OBJECTS =

tests/vcn/geometric_bot/libutils2D_UT.so: tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o
tests/vcn/geometric_bot/libutils2D_UT.so: tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/build.make
tests/vcn/geometric_bot/libutils2D_UT.so: tests/libvcn_test_add.so
tests/vcn/geometric_bot/libutils2D_UT.so: src/vcn/geometric_bot/libvcn_geometric_bot.so
tests/vcn/geometric_bot/libutils2D_UT.so: tests/libtest.a
tests/vcn/geometric_bot/libutils2D_UT.so: src/vcn/graph_bot/libvcn_graph_bot.so
tests/vcn/geometric_bot/libutils2D_UT.so: src/vcn/eigen_bot/libvcn_eigen_bot.so
tests/vcn/geometric_bot/libutils2D_UT.so: src/vcn/container_bot/libvcn_container_bot.so
tests/vcn/geometric_bot/libutils2D_UT.so: src/vcn/container_bot/liblist_container.so
tests/vcn/geometric_bot/libutils2D_UT.so: src/vcn/container_bot/libavl_container.so
tests/vcn/geometric_bot/libutils2D_UT.so: src/vcn/container_bot/libheap_container.so
tests/vcn/geometric_bot/libutils2D_UT.so: src/vcn/container_bot/libhtable_container.so
tests/vcn/geometric_bot/libutils2D_UT.so: src/libvcn_math_bot.so
tests/vcn/geometric_bot/libutils2D_UT.so: tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared module libutils2D_UT.so"
	cd /home/victor/vcn_bots/build/tests/vcn/geometric_bot && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/utils2D_UT.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/build: tests/vcn/geometric_bot/libutils2D_UT.so
.PHONY : tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/build

tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/requires: tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/utils2D_UT.c.o.requires
.PHONY : tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/requires

tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/clean:
	cd /home/victor/vcn_bots/build/tests/vcn/geometric_bot && $(CMAKE_COMMAND) -P CMakeFiles/utils2D_UT.dir/cmake_clean.cmake
.PHONY : tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/clean

tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/depend:
	cd /home/victor/vcn_bots/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victor/vcn_bots /home/victor/vcn_bots/tests/vcn/geometric_bot /home/victor/vcn_bots/build /home/victor/vcn_bots/build/tests/vcn/geometric_bot /home/victor/vcn_bots/build/tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/vcn/geometric_bot/CMakeFiles/utils2D_UT.dir/depend

