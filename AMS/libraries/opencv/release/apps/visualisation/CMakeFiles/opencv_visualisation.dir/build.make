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
include apps/visualisation/CMakeFiles/opencv_visualisation.dir/depend.make

# Include the progress variables for this target.
include apps/visualisation/CMakeFiles/opencv_visualisation.dir/progress.make

# Include the compile flags for this target's objects.
include apps/visualisation/CMakeFiles/opencv_visualisation.dir/flags.make

apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj: apps/visualisation/CMakeFiles/opencv_visualisation.dir/flags.make
apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj: apps/visualisation/CMakeFiles/opencv_visualisation.dir/includes_CXX.rsp
apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj: C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/apps/visualisation/opencv_visualisation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\apps\visualisation && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\opencv_visualisation.dir\opencv_visualisation.cpp.obj -c C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\apps\visualisation\opencv_visualisation.cpp

apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.i"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\apps\visualisation && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\apps\visualisation\opencv_visualisation.cpp > CMakeFiles\opencv_visualisation.dir\opencv_visualisation.cpp.i

apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.s"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\apps\visualisation && C:\Qt\Qt5.9.9\Tools\mingw530_32\bin\g++.exe  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\apps\visualisation\opencv_visualisation.cpp -o CMakeFiles\opencv_visualisation.dir\opencv_visualisation.cpp.s

apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj.requires:

.PHONY : apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj.requires

apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj.provides: apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj.requires
	$(MAKE) -f apps\visualisation\CMakeFiles\opencv_visualisation.dir\build.make apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj.provides.build
.PHONY : apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj.provides

apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj.provides.build: apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj


# Object files for target opencv_visualisation
opencv_visualisation_OBJECTS = \
"CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj"

# External object files for target opencv_visualisation
opencv_visualisation_EXTERNAL_OBJECTS =

bin/opencv_visualisation.exe: apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj
bin/opencv_visualisation.exe: apps/visualisation/CMakeFiles/opencv_visualisation.dir/build.make
bin/opencv_visualisation.exe: lib/libopencv_highgui320.dll.a
bin/opencv_visualisation.exe: lib/libopencv_videoio320.dll.a
bin/opencv_visualisation.exe: lib/libopencv_imgcodecs320.dll.a
bin/opencv_visualisation.exe: lib/libopencv_imgproc320.dll.a
bin/opencv_visualisation.exe: lib/libopencv_core320.dll.a
bin/opencv_visualisation.exe: apps/visualisation/CMakeFiles/opencv_visualisation.dir/linklibs.rsp
bin/opencv_visualisation.exe: apps/visualisation/CMakeFiles/opencv_visualisation.dir/objects1.rsp
bin/opencv_visualisation.exe: apps/visualisation/CMakeFiles/opencv_visualisation.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ..\..\bin\opencv_visualisation.exe"
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\apps\visualisation && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\opencv_visualisation.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
apps/visualisation/CMakeFiles/opencv_visualisation.dir/build: bin/opencv_visualisation.exe

.PHONY : apps/visualisation/CMakeFiles/opencv_visualisation.dir/build

apps/visualisation/CMakeFiles/opencv_visualisation.dir/requires: apps/visualisation/CMakeFiles/opencv_visualisation.dir/opencv_visualisation.cpp.obj.requires

.PHONY : apps/visualisation/CMakeFiles/opencv_visualisation.dir/requires

apps/visualisation/CMakeFiles/opencv_visualisation.dir/clean:
	cd /d C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\apps\visualisation && $(CMAKE_COMMAND) -P CMakeFiles\opencv_visualisation.dir\cmake_clean.cmake
.PHONY : apps/visualisation/CMakeFiles/opencv_visualisation.dir/clean

apps/visualisation/CMakeFiles/opencv_visualisation.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\sources\apps\visualisation C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\apps\visualisation C:\Users\malek\Documents\GitHub\2a15-ninjahub\AMS\libraries\opencv\release\apps\visualisation\CMakeFiles\opencv_visualisation.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : apps/visualisation/CMakeFiles/opencv_visualisation.dir/depend

