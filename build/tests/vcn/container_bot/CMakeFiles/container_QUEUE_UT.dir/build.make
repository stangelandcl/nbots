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
include tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/depend.make

# Include the progress variables for this target.
include tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/progress.make

# Include the compile flags for this target's objects.
include tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/flags.make

tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o: tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/flags.make
tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o: ../tests/vcn/container_bot/container_QUEUE_UT.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/victor/vcn_bots/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o"
	cd /home/victor/vcn_bots/build/tests/vcn/container_bot && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o   -c /home/victor/vcn_bots/tests/vcn/container_bot/container_QUEUE_UT.c

tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.i"
	cd /home/victor/vcn_bots/build/tests/vcn/container_bot && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/victor/vcn_bots/tests/vcn/container_bot/container_QUEUE_UT.c > CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.i

tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.s"
	cd /home/victor/vcn_bots/build/tests/vcn/container_bot && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/victor/vcn_bots/tests/vcn/container_bot/container_QUEUE_UT.c -o CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.s

tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o.requires:
.PHONY : tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o.requires

tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o.provides: tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o.requires
	$(MAKE) -f tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/build.make tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o.provides.build
.PHONY : tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o.provides

tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o.provides.build: tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o

# Object files for target container_QUEUE_UT
container_QUEUE_UT_OBJECTS = \
"CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o"

# External object files for target container_QUEUE_UT
container_QUEUE_UT_EXTERNAL_OBJECTS =

tests/vcn/container_bot/libcontainer_QUEUE_UT.so: tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o
tests/vcn/container_bot/libcontainer_QUEUE_UT.so: tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/build.make
tests/vcn/container_bot/libcontainer_QUEUE_UT.so: tests/libvcn_test_add.so
tests/vcn/container_bot/libcontainer_QUEUE_UT.so: src/vcn/container_bot/libvcn_container_bot.so
tests/vcn/container_bot/libcontainer_QUEUE_UT.so: src/libvcn_statistics_bot.so
tests/vcn/container_bot/libcontainer_QUEUE_UT.so: src/vcn/container_bot/liblist_container.so
tests/vcn/container_bot/libcontainer_QUEUE_UT.so: src/vcn/container_bot/libavl_container.so
tests/vcn/container_bot/libcontainer_QUEUE_UT.so: src/vcn/container_bot/libheap_container.so
tests/vcn/container_bot/libcontainer_QUEUE_UT.so: src/vcn/container_bot/libhtable_container.so
tests/vcn/container_bot/libcontainer_QUEUE_UT.so: tests/libtest.a
tests/vcn/container_bot/libcontainer_QUEUE_UT.so: tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C shared module libcontainer_QUEUE_UT.so"
	cd /home/victor/vcn_bots/build/tests/vcn/container_bot && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/container_QUEUE_UT.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/build: tests/vcn/container_bot/libcontainer_QUEUE_UT.so
.PHONY : tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/build

tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/requires: tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/container_QUEUE_UT.c.o.requires
.PHONY : tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/requires

tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/clean:
	cd /home/victor/vcn_bots/build/tests/vcn/container_bot && $(CMAKE_COMMAND) -P CMakeFiles/container_QUEUE_UT.dir/cmake_clean.cmake
.PHONY : tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/clean

tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/depend:
	cd /home/victor/vcn_bots/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victor/vcn_bots /home/victor/vcn_bots/tests/vcn/container_bot /home/victor/vcn_bots/build /home/victor/vcn_bots/build/tests/vcn/container_bot /home/victor/vcn_bots/build/tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/vcn/container_bot/CMakeFiles/container_QUEUE_UT.dir/depend

