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
include modules/videoio/CMakeFiles/opencv_perf_videoio.dir/depend.make

# Include the progress variables for this target.
include modules/videoio/CMakeFiles/opencv_perf_videoio.dir/progress.make

# Include the compile flags for this target's objects.
include modules/videoio/CMakeFiles/opencv_perf_videoio.dir/flags.make

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/flags.make
modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/includes_CXX.rsp
modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj: C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/modules/videoio/perf/perf_input.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\opencv_perf_videoio.dir\perf\perf_input.cpp.obj -c C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\videoio\perf\perf_input.cpp

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.i"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\videoio\perf\perf_input.cpp > CMakeFiles\opencv_perf_videoio.dir\perf\perf_input.cpp.i

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.s"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\videoio\perf\perf_input.cpp -o CMakeFiles\opencv_perf_videoio.dir\perf\perf_input.cpp.s

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj.requires:

.PHONY : modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj.requires

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj.provides: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj.requires
	$(MAKE) -f modules\videoio\CMakeFiles\opencv_perf_videoio.dir\build.make modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj.provides.build
.PHONY : modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj.provides

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj.provides.build: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj


modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/flags.make
modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/includes_CXX.rsp
modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj: C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/modules/videoio/perf/perf_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\opencv_perf_videoio.dir\perf\perf_main.cpp.obj -c C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\videoio\perf\perf_main.cpp

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.i"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\videoio\perf\perf_main.cpp > CMakeFiles\opencv_perf_videoio.dir\perf\perf_main.cpp.i

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.s"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\videoio\perf\perf_main.cpp -o CMakeFiles\opencv_perf_videoio.dir\perf\perf_main.cpp.s

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj.requires:

.PHONY : modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj.requires

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj.provides: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj.requires
	$(MAKE) -f modules\videoio\CMakeFiles\opencv_perf_videoio.dir\build.make modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj.provides.build
.PHONY : modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj.provides

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj.provides.build: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj


modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/flags.make
modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/includes_CXX.rsp
modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj: C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/modules/videoio/perf/perf_output.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\opencv_perf_videoio.dir\perf\perf_output.cpp.obj -c C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\videoio\perf\perf_output.cpp

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.i"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\videoio\perf\perf_output.cpp > CMakeFiles\opencv_perf_videoio.dir\perf\perf_output.cpp.i

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.s"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\videoio\perf\perf_output.cpp -o CMakeFiles\opencv_perf_videoio.dir\perf\perf_output.cpp.s

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj.requires:

.PHONY : modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj.requires

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj.provides: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj.requires
	$(MAKE) -f modules\videoio\CMakeFiles\opencv_perf_videoio.dir\build.make modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj.provides.build
.PHONY : modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj.provides

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj.provides.build: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj


# Object files for target opencv_perf_videoio
opencv_perf_videoio_OBJECTS = \
"CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj" \
"CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj" \
"CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj"

# External object files for target opencv_perf_videoio
opencv_perf_videoio_EXTERNAL_OBJECTS =

bin/opencv_perf_videoio.exe: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj
bin/opencv_perf_videoio.exe: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj
bin/opencv_perf_videoio.exe: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj
bin/opencv_perf_videoio.exe: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/build.make
bin/opencv_perf_videoio.exe: lib/libopencv_ts320.a
bin/opencv_perf_videoio.exe: lib/libopencv_highgui320.dll.a
bin/opencv_perf_videoio.exe: lib/libopencv_videoio320.dll.a
bin/opencv_perf_videoio.exe: lib/libopencv_imgcodecs320.dll.a
bin/opencv_perf_videoio.exe: lib/libopencv_imgproc320.dll.a
bin/opencv_perf_videoio.exe: lib/libopencv_core320.dll.a
bin/opencv_perf_videoio.exe: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/linklibs.rsp
bin/opencv_perf_videoio.exe: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/objects1.rsp
bin/opencv_perf_videoio.exe: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ..\..\bin\opencv_perf_videoio.exe"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\opencv_perf_videoio.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
modules/videoio/CMakeFiles/opencv_perf_videoio.dir/build: bin/opencv_perf_videoio.exe

.PHONY : modules/videoio/CMakeFiles/opencv_perf_videoio.dir/build

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/requires: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_input.cpp.obj.requires
modules/videoio/CMakeFiles/opencv_perf_videoio.dir/requires: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_main.cpp.obj.requires
modules/videoio/CMakeFiles/opencv_perf_videoio.dir/requires: modules/videoio/CMakeFiles/opencv_perf_videoio.dir/perf/perf_output.cpp.obj.requires

.PHONY : modules/videoio/CMakeFiles/opencv_perf_videoio.dir/requires

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/clean:
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio && $(CMAKE_COMMAND) -P CMakeFiles\opencv_perf_videoio.dir\cmake_clean.cmake
.PHONY : modules/videoio/CMakeFiles/opencv_perf_videoio.dir/clean

modules/videoio/CMakeFiles/opencv_perf_videoio.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\modules\videoio C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\modules\videoio\CMakeFiles\opencv_perf_videoio.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : modules/videoio/CMakeFiles/opencv_perf_videoio.dir/depend
