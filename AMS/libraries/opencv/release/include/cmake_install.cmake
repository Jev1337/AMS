# Install script for directory: C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include

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

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv" TYPE FILE FILES
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/cv.h"
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/cv.hpp"
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/cvaux.h"
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/cvaux.hpp"
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/cvwimage.h"
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/cxcore.h"
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/cxcore.hpp"
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/cxeigen.hpp"
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/cxmisc.h"
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/highgui.h"
    "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv/ml.h"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "dev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/opencv2" TYPE FILE FILES "C:/Users/malek/Documents/GitHub/2a15-ninjahub/AMS/libraries/opencv/sources/include/opencv2/opencv.hpp")
endif()

