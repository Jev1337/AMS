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

# Include any dependencies generated for this target.
include modules/highgui/CMakeFiles/opencv_test_highgui.dir/depend.make

# Include the progress variables for this target.
include modules/highgui/CMakeFiles/opencv_test_highgui.dir/progress.make

# Include the compile flags for this target's objects.
include modules/highgui/CMakeFiles/opencv_test_highgui.dir/flags.make

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj: modules/highgui/CMakeFiles/opencv_test_highgui.dir/flags.make
modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj: modules/highgui/CMakeFiles/opencv_test_highgui.dir/includes_CXX.rsp
modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj: C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/modules/highgui/test/test_gui.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\opencv_test_highgui.dir\test\test_gui.cpp.obj -c C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\highgui\test\test_gui.cpp

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.i"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\highgui\test\test_gui.cpp > CMakeFiles\opencv_test_highgui.dir\test\test_gui.cpp.i

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.s"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\highgui\test\test_gui.cpp -o CMakeFiles\opencv_test_highgui.dir\test\test_gui.cpp.s

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj.requires:

.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj.requires

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj.provides: modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj.requires
	$(MAKE) -f modules\highgui\CMakeFiles\opencv_test_highgui.dir\build.make modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj.provides.build
.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj.provides

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj.provides.build: modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj


modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj: modules/highgui/CMakeFiles/opencv_test_highgui.dir/flags.make
modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj: modules/highgui/CMakeFiles/opencv_test_highgui.dir/includes_CXX.rsp
modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj: C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/modules/highgui/test/test_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\opencv_test_highgui.dir\test\test_main.cpp.obj -c C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\highgui\test\test_main.cpp

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.i"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\highgui\test\test_main.cpp > CMakeFiles\opencv_test_highgui.dir\test\test_main.cpp.i

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.s"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\highgui\test\test_main.cpp -o CMakeFiles\opencv_test_highgui.dir\test\test_main.cpp.s

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj.requires:

.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj.requires

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj.provides: modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj.requires
	$(MAKE) -f modules\highgui\CMakeFiles\opencv_test_highgui.dir\build.make modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj.provides.build
.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj.provides

modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj.provides.build: modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj


modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj: modules/highgui/CMakeFiles/opencv_test_highgui.dir/flags.make
modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj: modules/highgui/CMakeFiles/opencv_test_highgui.dir/includes_CXX.rsp
modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj: modules/highgui/opencv_test_highgui_automoc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\opencv_test_highgui.dir\opencv_test_highgui_automoc.cpp.obj -c C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui\opencv_test_highgui_automoc.cpp

modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.i"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui\opencv_test_highgui_automoc.cpp > CMakeFiles\opencv_test_highgui.dir\opencv_test_highgui_automoc.cpp.i

modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.s"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui\opencv_test_highgui_automoc.cpp -o CMakeFiles\opencv_test_highgui.dir\opencv_test_highgui_automoc.cpp.s

modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj.requires:

.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj.requires

modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj.provides: modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj.requires
	$(MAKE) -f modules\highgui\CMakeFiles\opencv_test_highgui.dir\build.make modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj.provides.build
.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj.provides

modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj.provides.build: modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj


# Object files for target opencv_test_highgui
opencv_test_highgui_OBJECTS = \
"CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj" \
"CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj" \
"CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj"

# External object files for target opencv_test_highgui
opencv_test_highgui_EXTERNAL_OBJECTS =

bin/opencv_test_highgui.exe: modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj
bin/opencv_test_highgui.exe: modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj
bin/opencv_test_highgui.exe: modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj
bin/opencv_test_highgui.exe: modules/highgui/CMakeFiles/opencv_test_highgui.dir/build.make
bin/opencv_test_highgui.exe: lib/libopencv_ts320.a
bin/opencv_test_highgui.exe: lib/libopencv_highgui320.dll.a
bin/opencv_test_highgui.exe: lib/libopencv_videoio320.dll.a
bin/opencv_test_highgui.exe: lib/libopencv_imgcodecs320.dll.a
bin/opencv_test_highgui.exe: lib/libopencv_imgproc320.dll.a
bin/opencv_test_highgui.exe: lib/libopencv_core320.dll.a
bin/opencv_test_highgui.exe: modules/highgui/CMakeFiles/opencv_test_highgui.dir/linklibs.rsp
bin/opencv_test_highgui.exe: modules/highgui/CMakeFiles/opencv_test_highgui.dir/objects1.rsp
bin/opencv_test_highgui.exe: modules/highgui/CMakeFiles/opencv_test_highgui.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ..\..\bin\opencv_test_highgui.exe"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\opencv_test_highgui.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
modules/highgui/CMakeFiles/opencv_test_highgui.dir/build: bin/opencv_test_highgui.exe

.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui.dir/build

modules/highgui/CMakeFiles/opencv_test_highgui.dir/requires: modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_gui.cpp.obj.requires
modules/highgui/CMakeFiles/opencv_test_highgui.dir/requires: modules/highgui/CMakeFiles/opencv_test_highgui.dir/test/test_main.cpp.obj.requires
modules/highgui/CMakeFiles/opencv_test_highgui.dir/requires: modules/highgui/CMakeFiles/opencv_test_highgui.dir/opencv_test_highgui_automoc.cpp.obj.requires

.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui.dir/requires

modules/highgui/CMakeFiles/opencv_test_highgui.dir/clean:
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui && $(CMAKE_COMMAND) -P CMakeFiles\opencv_test_highgui.dir\cmake_clean.cmake
.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui.dir/clean

modules/highgui/CMakeFiles/opencv_test_highgui.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\highgui C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\highgui\CMakeFiles\opencv_test_highgui.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : modules/highgui/CMakeFiles/opencv_test_highgui.dir/depend

