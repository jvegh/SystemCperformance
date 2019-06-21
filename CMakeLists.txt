##############################################################
#  
#  This is a CMake configuration file for the SysCperf project.
#  To use it you need CMake which can be 
#  downloaded from here: 
#    http://www.cmake.org/cmake/resources/software.html
#
# The directory structure (and other docs) can be found in cmake/Docs
##############################################################
#
# @author János Végh, but used numerous public sources
if ( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR} )
    message( FATAL_ERROR "In-source builds not allowed.\
    Please make a new directory (called a build directory) and run CMake from there.\
    You may need to remove CMakeCache.txt." )
endif()

cmake_policy(SET CMP0048 NEW)
cmake_minimum_required(VERSION 3.1)

# Maintain your own version here
set(MyVersion "0.0.1")

set(CPACK_PACKAGE_NAME "syscperf")
project(SysCperf VERSION ${MyVersion} LANGUAGES C CXX)
#  Add your compile-time definitions here
#    add_definitions(-DMY_DEF)
# Create your subdirectories like this, if you need
# file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/data")

# ---[ Set up CMake options for tools
# Choose whether you need debugging support for development or making a release
  option (DEBUG_MODE "Include debug support for the program"  OFF)
# Choose whether the stand-alone unit testing is to be built
  option (BUILD_TESTS "Include unit tests for the tool"  ON)
# Choose whether the stand-alone unit testing is to be built
  option (BUILD_PROFILE "Include profiling for the tool"  ON)
# Choose whether to make documention as well
  option (BUILD_DOCS "Prepare also HTML/CHM documentation" ON)
# Choose whether to enable asserting
  option (NDEBUG_MODE "Enable to assert the tools" OFF)
# Choose whether libraries should be shared
#  option(BUILD_SHARED_LIBS "Build shared libraries" ON)
# Choose whether clocks are to be used. By default, no clock is used;
# it is time consuming
  option (CLOCKED_MODE "Enable to use clock signals")

###############################################################
# Do not edit below this line until you know what you are doing!
###############################################################

# Choose whether you need internal information to docs
option (DOXY_INTERNAL "Include internal info in the docs"  YES)
# ${DOXY_INPUT}
# @GENERATETREEVIEW@

include(cmake/Colorize.cmake)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

# Set the build type if it isn't done already
if(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE Debug)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  set(CMAKE_RUNTIME_WORK_DIRECTORY "${CMAKE_BINARY_DIR}")
  message(HIGHLIGHTED "Configuring ${PROJECT_NAME} V${PROJECT_VERSION} debug version")
else(DEBUG_MODE)
  set(CMAKE_BUILD_TYPE Release)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
  set(CMAKE_RUNTIME_WORK_DIRECTORY "${CMAKE_BINARY_DIR}")
  message(HIGHLIGHTED "Configuring ${PROJECT_NAME} V${PROJECT_VERSION} release version")
  set(CMAKE_CXX_FLAGS "-DQT_NO_DEBUG_OUTPUT" )
endif(DEBUG_MODE)

# Now decide if to enable assertions
if(NDEBUG_MODE)
else(NDEBUG_MODE)
  add_definitions(-DNDEBUG)
endif(NDEBUG_MODE)

if(CLOCKED_MODE)
    add_definitions(-DCLOCKED)
else(CLOCKED_MODE)
endif(CLOCKED_MODE)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${CPACK_PACKAGE_NAME}")
# ---[ The out-of-source build directory
set(CMAKE_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

# ---[ Using cmake scripts and modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/External)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

include(ExternalProject)
include(cmake/Summary.cmake)
include(cmake/Utils.cmake)

# ---[ Dependencies 
include(cmake/Dependencies.cmake)

#set (CMAKE_PREFIX_PATH /usr/local/systemc)
find_package(SystemCLanguage CONFIG REQUIRED)

set (CMAKE_INSTALL_PREFIX $ENV{SYSTEMC_HOME} CACHE PATH "Installation root directory" FORCE)
set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD} CACHE STRING
     "C++ standard to build all targets. Supported values are 98, 11, and 14.")
set (CMAKE_CXX_STANDARD_REQUIRED ${SystemC_CXX_STANDARD_REQUIRED} CACHE BOOL
     "The with CMAKE_CXX_STANDARD selected C++ standard is a requirement.")
set(CXXFLAGS -DCMAKE_CXX_STANDARD=${SystemC_CXX_STANDARD})

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# ---[ Flags
if(UNIX OR APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall")
endif()

if(USE_libstdcpp)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
  message("-- Warning: forcing libstdc++ (controlled by USE_libstdcpp option in cmake)")
endif()

# Copy the version and other configuration info to the source
# A good idea is to maintain a generic 'config.h' file together with the main CMakeLists.txt
  message("-- Copy config file from ${CMAKE_SOURCE_DIR}")
  configure_file(${CMAKE_SOURCE_DIR}/Config.h.in ${CMAKE_SOURCE_DIR}/modules/include/Config.h)
  add_subdirectory(modules)	# Hardware module library
  add_subdirectory(main)	# The executables
#later  add_subdirectory(ThirdParty)  # Add 3-rd party modules
#later  add_subdirectory(QtGUI)	# The Qt-based graphics routines
#

file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/docs")
if(BUILD_TESTS)
#  add_subdirectory(test)	# Tests for the tool library
endif(BUILD_TESTS)
if(BUILD_DOCS)
  find_package(Doxygen)
  add_subdirectory(doc) # The doxygen documentation
endif(BUILD_DOCS)

# ---[ Target 'package' builds everything
include(cmake/Packages.cmake)

# ---[ Print configuration summary
caffe_print_configuration_summary()
