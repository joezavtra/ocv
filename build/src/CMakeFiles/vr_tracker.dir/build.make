# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_COMMAND = /opt/local/bin/cmake

# The command to remove a file.
RM = /opt/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/pavelzelenov/ws/vr_tracker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/pavelzelenov/ws/vr_tracker/build

# Include any dependencies generated for this target.
include src/CMakeFiles/vr_tracker.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/vr_tracker.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/vr_tracker.dir/flags.make

src/CMakeFiles/vr_tracker.dir/server.cpp.o: src/CMakeFiles/vr_tracker.dir/flags.make
src/CMakeFiles/vr_tracker.dir/server.cpp.o: ../src/server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pavelzelenov/ws/vr_tracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/vr_tracker.dir/server.cpp.o"
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vr_tracker.dir/server.cpp.o -c /Users/pavelzelenov/ws/vr_tracker/src/server.cpp

src/CMakeFiles/vr_tracker.dir/server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vr_tracker.dir/server.cpp.i"
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/pavelzelenov/ws/vr_tracker/src/server.cpp > CMakeFiles/vr_tracker.dir/server.cpp.i

src/CMakeFiles/vr_tracker.dir/server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vr_tracker.dir/server.cpp.s"
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/pavelzelenov/ws/vr_tracker/src/server.cpp -o CMakeFiles/vr_tracker.dir/server.cpp.s

src/CMakeFiles/vr_tracker.dir/server.cpp.o.requires:

.PHONY : src/CMakeFiles/vr_tracker.dir/server.cpp.o.requires

src/CMakeFiles/vr_tracker.dir/server.cpp.o.provides: src/CMakeFiles/vr_tracker.dir/server.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/vr_tracker.dir/build.make src/CMakeFiles/vr_tracker.dir/server.cpp.o.provides.build
.PHONY : src/CMakeFiles/vr_tracker.dir/server.cpp.o.provides

src/CMakeFiles/vr_tracker.dir/server.cpp.o.provides.build: src/CMakeFiles/vr_tracker.dir/server.cpp.o


src/CMakeFiles/vr_tracker.dir/tracker.cpp.o: src/CMakeFiles/vr_tracker.dir/flags.make
src/CMakeFiles/vr_tracker.dir/tracker.cpp.o: ../src/tracker.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pavelzelenov/ws/vr_tracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/vr_tracker.dir/tracker.cpp.o"
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vr_tracker.dir/tracker.cpp.o -c /Users/pavelzelenov/ws/vr_tracker/src/tracker.cpp

src/CMakeFiles/vr_tracker.dir/tracker.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vr_tracker.dir/tracker.cpp.i"
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/pavelzelenov/ws/vr_tracker/src/tracker.cpp > CMakeFiles/vr_tracker.dir/tracker.cpp.i

src/CMakeFiles/vr_tracker.dir/tracker.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vr_tracker.dir/tracker.cpp.s"
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/pavelzelenov/ws/vr_tracker/src/tracker.cpp -o CMakeFiles/vr_tracker.dir/tracker.cpp.s

src/CMakeFiles/vr_tracker.dir/tracker.cpp.o.requires:

.PHONY : src/CMakeFiles/vr_tracker.dir/tracker.cpp.o.requires

src/CMakeFiles/vr_tracker.dir/tracker.cpp.o.provides: src/CMakeFiles/vr_tracker.dir/tracker.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/vr_tracker.dir/build.make src/CMakeFiles/vr_tracker.dir/tracker.cpp.o.provides.build
.PHONY : src/CMakeFiles/vr_tracker.dir/tracker.cpp.o.provides

src/CMakeFiles/vr_tracker.dir/tracker.cpp.o.provides.build: src/CMakeFiles/vr_tracker.dir/tracker.cpp.o


src/CMakeFiles/vr_tracker.dir/monitor.cpp.o: src/CMakeFiles/vr_tracker.dir/flags.make
src/CMakeFiles/vr_tracker.dir/monitor.cpp.o: ../src/monitor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/pavelzelenov/ws/vr_tracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/vr_tracker.dir/monitor.cpp.o"
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/vr_tracker.dir/monitor.cpp.o -c /Users/pavelzelenov/ws/vr_tracker/src/monitor.cpp

src/CMakeFiles/vr_tracker.dir/monitor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/vr_tracker.dir/monitor.cpp.i"
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/pavelzelenov/ws/vr_tracker/src/monitor.cpp > CMakeFiles/vr_tracker.dir/monitor.cpp.i

src/CMakeFiles/vr_tracker.dir/monitor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/vr_tracker.dir/monitor.cpp.s"
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/pavelzelenov/ws/vr_tracker/src/monitor.cpp -o CMakeFiles/vr_tracker.dir/monitor.cpp.s

src/CMakeFiles/vr_tracker.dir/monitor.cpp.o.requires:

.PHONY : src/CMakeFiles/vr_tracker.dir/monitor.cpp.o.requires

src/CMakeFiles/vr_tracker.dir/monitor.cpp.o.provides: src/CMakeFiles/vr_tracker.dir/monitor.cpp.o.requires
	$(MAKE) -f src/CMakeFiles/vr_tracker.dir/build.make src/CMakeFiles/vr_tracker.dir/monitor.cpp.o.provides.build
.PHONY : src/CMakeFiles/vr_tracker.dir/monitor.cpp.o.provides

src/CMakeFiles/vr_tracker.dir/monitor.cpp.o.provides.build: src/CMakeFiles/vr_tracker.dir/monitor.cpp.o


# Object files for target vr_tracker
vr_tracker_OBJECTS = \
"CMakeFiles/vr_tracker.dir/server.cpp.o" \
"CMakeFiles/vr_tracker.dir/tracker.cpp.o" \
"CMakeFiles/vr_tracker.dir/monitor.cpp.o"

# External object files for target vr_tracker
vr_tracker_EXTERNAL_OBJECTS =

src/vr_tracker: src/CMakeFiles/vr_tracker.dir/server.cpp.o
src/vr_tracker: src/CMakeFiles/vr_tracker.dir/tracker.cpp.o
src/vr_tracker: src/CMakeFiles/vr_tracker.dir/monitor.cpp.o
src/vr_tracker: src/CMakeFiles/vr_tracker.dir/build.make
src/vr_tracker: /opt/local/lib/libboost_system-mt.dylib
src/vr_tracker: /usr/local/lib/libopencv_videostab.a
src/vr_tracker: /usr/local/lib/libopencv_videoio.a
src/vr_tracker: /usr/local/lib/libopencv_video.a
src/vr_tracker: /usr/local/lib/libopencv_superres.a
src/vr_tracker: /usr/local/lib/libopencv_stitching.a
src/vr_tracker: /usr/local/lib/libopencv_shape.a
src/vr_tracker: /usr/local/lib/libopencv_photo.a
src/vr_tracker: /usr/local/lib/libopencv_objdetect.a
src/vr_tracker: /usr/local/lib/libopencv_ml.a
src/vr_tracker: /usr/local/lib/libopencv_imgproc.a
src/vr_tracker: /usr/local/lib/libopencv_imgcodecs.a
src/vr_tracker: /usr/local/lib/libopencv_highgui.a
src/vr_tracker: /usr/local/lib/libopencv_flann.a
src/vr_tracker: /usr/local/lib/libopencv_features2d.a
src/vr_tracker: /usr/local/lib/libopencv_core.a
src/vr_tracker: /usr/local/lib/libopencv_calib3d.a
src/vr_tracker: /usr/local/lib/libopencv_features2d.a
src/vr_tracker: /usr/local/lib/libopencv_ml.a
src/vr_tracker: /usr/local/lib/libopencv_highgui.a
src/vr_tracker: /usr/local/lib/libopencv_videoio.a
src/vr_tracker: /usr/local/lib/libopencv_imgcodecs.a
src/vr_tracker: /usr/local/share/OpenCV/3rdparty/lib/liblibjpeg.a
src/vr_tracker: /usr/local/share/OpenCV/3rdparty/lib/liblibpng.a
src/vr_tracker: /usr/local/share/OpenCV/3rdparty/lib/liblibtiff.a
src/vr_tracker: /usr/local/share/OpenCV/3rdparty/lib/liblibjasper.a
src/vr_tracker: /usr/local/share/OpenCV/3rdparty/lib/libIlmImf.a
src/vr_tracker: /usr/local/lib/libopencv_flann.a
src/vr_tracker: /usr/local/lib/libopencv_video.a
src/vr_tracker: /usr/local/lib/libopencv_imgproc.a
src/vr_tracker: /usr/local/lib/libopencv_core.a
src/vr_tracker: /usr/local/share/OpenCV/3rdparty/lib/libzlib.a
src/vr_tracker: /usr/local/share/OpenCV/3rdparty/lib/libippicv.a
src/vr_tracker: src/CMakeFiles/vr_tracker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/pavelzelenov/ws/vr_tracker/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable vr_tracker"
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/vr_tracker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/vr_tracker.dir/build: src/vr_tracker

.PHONY : src/CMakeFiles/vr_tracker.dir/build

src/CMakeFiles/vr_tracker.dir/requires: src/CMakeFiles/vr_tracker.dir/server.cpp.o.requires
src/CMakeFiles/vr_tracker.dir/requires: src/CMakeFiles/vr_tracker.dir/tracker.cpp.o.requires
src/CMakeFiles/vr_tracker.dir/requires: src/CMakeFiles/vr_tracker.dir/monitor.cpp.o.requires

.PHONY : src/CMakeFiles/vr_tracker.dir/requires

src/CMakeFiles/vr_tracker.dir/clean:
	cd /Users/pavelzelenov/ws/vr_tracker/build/src && $(CMAKE_COMMAND) -P CMakeFiles/vr_tracker.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/vr_tracker.dir/clean

src/CMakeFiles/vr_tracker.dir/depend:
	cd /Users/pavelzelenov/ws/vr_tracker/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/pavelzelenov/ws/vr_tracker /Users/pavelzelenov/ws/vr_tracker/src /Users/pavelzelenov/ws/vr_tracker/build /Users/pavelzelenov/ws/vr_tracker/build/src /Users/pavelzelenov/ws/vr_tracker/build/src/CMakeFiles/vr_tracker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/vr_tracker.dir/depend

