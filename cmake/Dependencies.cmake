# Borrowed from https://github.com/BVLC/caffe
# This list is required for static linking and exported to CaffeConfig.cmake
set(Caffe_LINKER_LIBS "")

# ---[ Boost
#find_package(Boost 1.46 REQUIRED COMPONENTS system thread filesystem)
#include_directories(SYSTEM ${Boost_INCLUDE_DIR})
#list(APPEND Caffe_LINKER_LIBS ${Boost_LIBRARIES})

# ---[ Threads
find_package(Threads REQUIRED)
list(APPEND Caffe_LINKER_LIBS ${CMAKE_THREAD_LIBS_INIT} )

# ---[ Google-test
include("cmake/External/gtest.cmake")
include_directories(SYSTEM ${GTEST_INCLUDE_DIRS})
list(APPEND Caffe_LINKER_LIBS ${GTEST_LIBRARIES})


# ---[ Doxygen
if(BUILD_docs)
  find_package(Doxygen)
endif()