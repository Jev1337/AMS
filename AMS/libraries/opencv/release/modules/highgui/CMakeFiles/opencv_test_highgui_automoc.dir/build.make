# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.7

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\CMake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release

# Utility rule file for opencv_test_highgui_automoc.

# Include the progress variables for this target.
include modules/highgui/CMakeFiles/opencv_test_highgui_automoc.dir/progress.make

modules/highgui/CMakeFiles/opencv_test_highgui_automoc:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic moc for target opencv_test_highgui"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && "C:\Program Files\CMake\bin\cmake.exe" -E cmake_autogen C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/release/modules/highgui/CMakeFiles/opencv_test_highgui_automoc.dir/ Release

opencv_test_highgui_automoc: modules/highgui/CMakeFiles/opencv_test_highgui_automoc
opencv_test_highgui_automoc: modules/highgui/CMakeFiles/opencv_test_highgui_automoc.dir/build.make

.PHONY : opencv_test_highgui_automoc

# Rule to build all files generated by this target.
modules/highgui/CMakeFiles/opencv_test_highgui_automoc.dir/build: opencv_test_highgui_automoc

.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui_automoc.dir/build

modules/highgui/CMakeFiles/opencv_test_highgui_automoc.dir/clean:
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && $(CMAKE_COMMAND) -P CMakeFiles\opencv_test_highgui_automoc.dir\cmake_clean.cmake
.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui_automoc.dir/clean

modules/highgui/CMakeFiles/opencv_test_highgui_automoc.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\highgui C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui\CMakeFiles\opencv_test_highgui_automoc.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui_automoc.dir/depend

