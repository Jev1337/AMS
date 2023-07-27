# Install script for directory: C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/modules/python/python3

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/release/install")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "python" OR NOT CMAKE_INSTALL_COMPONENT)
  list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
   "C:/Users/malek/AppData/Local/Programs/Python/Python310/Lib/site-packages/cv2.cp310-win_amd64.pyd")
  if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
  if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
    message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
  endif()
file(INSTALL DESTINATION "C:/Users/malek/AppData/Local/Programs/Python/Python310/Lib/site-packages" TYPE MODULE FILES "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/release/lib/python3/cv2.cp310-win_amd64.pyd")
  if(EXISTS "$ENV{DESTDIR}/C:/Users/malek/AppData/Local/Programs/Python/Python310/Lib/site-packages/cv2.cp310-win_amd64.pyd" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/C:/Users/malek/AppData/Local/Programs/Python/Python310/Lib/site-packages/cv2.cp310-win_amd64.pyd")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "C:/Qt/Qt5.9.9/Tools/mingw530_32/bin/strip.exe" "$ENV{DESTDIR}/C:/Users/malek/AppData/Local/Programs/Python/Python310/Lib/site-packages/cv2.cp310-win_amd64.pyd")
    endif()
  endif()
endif()

