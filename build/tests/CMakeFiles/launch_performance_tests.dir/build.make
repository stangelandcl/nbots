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
include tests/CMakeFiles/launch_performance_tests.dir/depend.make

# Include the progress variables for this target.
include tests/CMakeFiles/launch_performance_tests.dir/progress.make

# Include the compile flags for this target's objects.
include tests/CMakeFiles/launch_performance_tests.dir/flags.make

tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o: tests/CMakeFiles/launch_performance_tests.dir/flags.make
tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o: ../tests/performance_test_driver.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/victor/vcn_bots/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o"
	cd /home/victor/vcn_bots/build/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o   -c /home/victor/vcn_bots/tests/performance_test_driver.c

tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.i"
	cd /home/victor/vcn_bots/build/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/victor/vcn_bots/tests/performance_test_driver.c > CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.i

tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.s"
	cd /home/victor/vcn_bots/build/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/victor/vcn_bots/tests/performance_test_driver.c -o CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.s

tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o.requires:
.PHONY : tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o.requires

tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o.provides: tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o.requires
	$(MAKE) -f tests/CMakeFiles/launch_performance_tests.dir/build.make tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o.provides.build
.PHONY : tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o.provides

tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o.provides.build: tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o

tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o: tests/CMakeFiles/launch_performance_tests.dir/flags.make
tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o: ../tests/run_tests.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/victor/vcn_bots/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o"
	cd /home/victor/vcn_bots/build/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/launch_performance_tests.dir/run_tests.c.o   -c /home/victor/vcn_bots/tests/run_tests.c

tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/launch_performance_tests.dir/run_tests.c.i"
	cd /home/victor/vcn_bots/build/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -E /home/victor/vcn_bots/tests/run_tests.c > CMakeFiles/launch_performance_tests.dir/run_tests.c.i

tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/launch_performance_tests.dir/run_tests.c.s"
	cd /home/victor/vcn_bots/build/tests && /usr/bin/cc  $(C_DEFINES) $(C_FLAGS) -S /home/victor/vcn_bots/tests/run_tests.c -o CMakeFiles/launch_performance_tests.dir/run_tests.c.s

tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o.requires:
.PHONY : tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o.requires

tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o.provides: tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o.requires
	$(MAKE) -f tests/CMakeFiles/launch_performance_tests.dir/build.make tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o.provides.build
.PHONY : tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o.provides

tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o.provides.build: tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o

# Object files for target launch_performance_tests
launch_performance_tests_OBJECTS = \
"CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o" \
"CMakeFiles/launch_performance_tests.dir/run_tests.c.o"

# External object files for target launch_performance_tests
launch_performance_tests_EXTERNAL_OBJECTS =

tests/launch_performance_tests: tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o
tests/launch_performance_tests: tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o
tests/launch_performance_tests: tests/CMakeFiles/launch_performance_tests.dir/build.make
tests/launch_performance_tests: src/libvcn_exceptions.so
tests/launch_performance_tests: src/libvcn_statistics_bot.so
tests/launch_performance_tests: src/libvcn_math_bot.so
tests/launch_performance_tests: tests/libtest.a
tests/launch_performance_tests: src/vcn/container_bot/libvcn_container_bot.so
tests/launch_performance_tests: src/vcn/container_bot/liblist_container.so
tests/launch_performance_tests: src/vcn/container_bot/libavl_container.so
tests/launch_performance_tests: src/vcn/container_bot/libheap_container.so
tests/launch_performance_tests: src/vcn/container_bot/libhtable_container.so
tests/launch_performance_tests: tests/CMakeFiles/launch_performance_tests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking C executable launch_performance_tests"
	cd /home/victor/vcn_bots/build/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/launch_performance_tests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
tests/CMakeFiles/launch_performance_tests.dir/build: tests/launch_performance_tests
.PHONY : tests/CMakeFiles/launch_performance_tests.dir/build

tests/CMakeFiles/launch_performance_tests.dir/requires: tests/CMakeFiles/launch_performance_tests.dir/performance_test_driver.c.o.requires
tests/CMakeFiles/launch_performance_tests.dir/requires: tests/CMakeFiles/launch_performance_tests.dir/run_tests.c.o.requires
.PHONY : tests/CMakeFiles/launch_performance_tests.dir/requires

tests/CMakeFiles/launch_performance_tests.dir/clean:
	cd /home/victor/vcn_bots/build/tests && $(CMAKE_COMMAND) -P CMakeFiles/launch_performance_tests.dir/cmake_clean.cmake
.PHONY : tests/CMakeFiles/launch_performance_tests.dir/clean

tests/CMakeFiles/launch_performance_tests.dir/depend:
	cd /home/victor/vcn_bots/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/victor/vcn_bots /home/victor/vcn_bots/tests /home/victor/vcn_bots/build /home/victor/vcn_bots/build/tests /home/victor/vcn_bots/build/tests/CMakeFiles/launch_performance_tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : tests/CMakeFiles/launch_performance_tests.dir/depend

